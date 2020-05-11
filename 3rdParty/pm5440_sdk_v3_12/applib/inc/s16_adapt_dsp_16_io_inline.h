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
 *     and register accessor functions for the s16_adapt_dsp_16 block
 *****************************************************************************/
#ifndef _S16_ADAPT_DSP_16_IO_INLINE_H
#define _S16_ADAPT_DSP_16_IO_INLINE_H

#include "s16_loc.h"
#include "s16_adapt_dsp_16_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define S16_ADAPT_DSP_16_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for s16_adapt_dsp_16
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
    s16_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} s16_adapt_dsp_16_buffer_t;
static INLINE void s16_adapt_dsp_16_buffer_init( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                 s16_handle_t *h_ptr,
                                                 UINT32 A ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_buffer_init( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                 s16_handle_t *h_ptr,
                                                 UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "s16_adapt_dsp_16_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void s16_adapt_dsp_16_buffer_flush( s16_adapt_dsp_16_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_buffer_flush( s16_adapt_dsp_16_buffer_t *b_ptr )
{
    IOLOG( "s16_adapt_dsp_16_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 s16_adapt_dsp_16_reg_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                s16_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mem_type,
                                                UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                s16_handle_t *h_ptr,
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
static INLINE void s16_adapt_dsp_16_reg_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                               s16_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                               s16_handle_t *h_ptr,
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

static INLINE void s16_adapt_dsp_16_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                               s16_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 mask,
                                               UINT32 unused_mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                               s16_handle_t *h_ptr,
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

static INLINE void s16_adapt_dsp_16_action_on_write_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mem_type,
                                                               UINT32 reg,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_action_on_write_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
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

static INLINE void s16_adapt_dsp_16_burst_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                s16_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 len,
                                                UINT32 *value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_burst_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                s16_handle_t *h_ptr,
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

static INLINE void s16_adapt_dsp_16_burst_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                 s16_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 len,
                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_burst_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                 s16_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mem_type,
                                                          UINT32 reg,
                                                          UINT32 mask,
                                                          UINT32 value,
                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                          UINT32 max_count,
                                                          UINT32 *num_failed_polls,
                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
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
 *  register access functions for s16_adapt_dsp_16
 * ==================================================================================
 */

static INLINE void s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL,
                                mask,
                                PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_1_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_1_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_ADAPT_CTRL_1_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_CTRL_1,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_CTRL_1,
                                mask,
                                PMC_ADSP_REG_ADAPT_CTRL_1_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_ADAPT_CTRL_1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_2_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_2_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_ADAPT_CTRL_2_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_CTRL_2,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_2_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_2_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_ADAPT_CTRL_2_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_CTRL_2,
                                mask,
                                PMC_ADSP_REG_ADAPT_CTRL_2_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_CTRL_2_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_CTRL_2_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_ADAPT_CTRL_2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_CTRL_2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_3_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_3_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_ADAPT_CTRL_3_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_CTRL_3,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_3_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_CTRL_3_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_ADAPT_CTRL_3_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_CTRL_3,
                                mask,
                                PMC_ADSP_REG_ADAPT_CTRL_3_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_CTRL_3_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_CTRL_3_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_ADAPT_CTRL_3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_CTRL_3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_DSS_CTRL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_CTRL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_DSS_CTRL_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_CTRL,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_DSS_CTRL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_CTRL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_DSS_CTRL_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_CTRL,
                                mask,
                                PMC_ADSP_REG_DSS_CTRL_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_CTRL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_CTRL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_DSS_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_DSS_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_1_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_1_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_DSS_TAPS_1_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_TAPS_1,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_TAPS_1,
                                mask,
                                PMC_ADSP_REG_DSS_TAPS_1_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_TAPS_1_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_TAPS_1_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_DSS_TAPS_1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_DSS_TAPS_1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_2_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_2_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_DSS_TAPS_2_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_TAPS_2,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_2_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_2_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_DSS_TAPS_2_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_TAPS_2,
                                mask,
                                PMC_ADSP_REG_DSS_TAPS_2_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_TAPS_2_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_TAPS_2_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_DSS_TAPS_2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_DSS_TAPS_2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_3_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_3_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_DSS_TAPS_3_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_TAPS_3,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_TAPS_3,
                                mask,
                                PMC_ADSP_REG_DSS_TAPS_3_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_TAPS_3_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_TAPS_3_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_DSS_TAPS_3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_DSS_TAPS_3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_4_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_4_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_DSS_TAPS_4_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_TAPS_4,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_DSS_TAPS_4,
                                mask,
                                PMC_ADSP_REG_DSS_TAPS_4_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_TAPS_4_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_DSS_TAPS_4_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_DSS_TAPS_4);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_DSS_TAPS_4_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_LMS_MU_FACTORS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_LMS_MU_FACTORS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_LMS_MU_FACTORS_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_LMS_MU_FACTORS,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_LMS_MU_FACTORS,
                                mask,
                                PMC_ADSP_REG_LMS_MU_FACTORS_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_LMS_MU_FACTORS);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ROUND_ROBIN_PERIODS,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ROUND_ROBIN_PERIODS,
                                mask,
                                PMC_ADSP_REG_ROUND_ROBIN_PERIODS_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_ROUND_ROBIN_PERIODS);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1,
                                mask,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2,
                                mask,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3,
                                mask,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4,
                                mask,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5,
                                mask,
                                PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_CLIP_RATE_MON_PARAMS,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_CLIP_RATE_MON_PARAMS,
                                mask,
                                PMC_ADSP_REG_CLIP_RATE_MON_PARAMS_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_CLIP_RATE_MON_PARAMS);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_FORCE_VAL,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_ADAPT_FORCE_VAL,
                                mask,
                                PMC_ADSP_REG_ADAPT_FORCE_VAL_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_ADAPT_FORCE_VAL);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_FETX_TTIU_CTRL,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_FETX_TTIU_CTRL,
                                mask,
                                PMC_ADSP_REG_FETX_TTIU_CTRL_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_FETX_TTIU_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_FETX_CTRL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_FETX_CTRL_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_FETX_CTRL_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_FETX_CTRL,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_FETX_CTRL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_FETX_CTRL_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_FETX_CTRL_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_FETX_CTRL,
                                mask,
                                PMC_ADSP_REG_FETX_CTRL_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_FETX_CTRL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_FETX_CTRL_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_FETX_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_FETX_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_CONFIG,
                                PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN,
                                mask,
                                PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_CONFIG,
                                           PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_STAT_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_STAT_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_ADAPT_STAT_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_ADAPT_STAT,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_STAT_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_STAT_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_ADAPT_STAT_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_ADAPT_STAT,
                                mask,
                                PMC_ADSP_REG_ADAPT_STAT_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_STAT_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_STAT_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_ADAPT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                 s16_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                 s16_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST,
                                mask,
                                PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS,
                                mask,
                                PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_FETX_ADJ_STATUS,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_FETX_ADJ_STATUS,
                                mask,
                                PMC_ADSP_REG_FETX_ADJ_STATUS_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_FETX_ADJ_STATUS);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read", reg_value, A);
    return reg_value;
}

static INLINE void s16_adapt_dsp_16_reg_STAT_INT_EVENT_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_STAT_INT_EVENT_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_STAT_INT_EVENT_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_STAT_INT_EVENT,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_STAT_INT_EVENT_action_on_write_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                  s16_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_STAT_INT_EVENT_action_on_write_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                  s16_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_STAT_INT_EVENT_action_on_write_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_action_on_write_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                MEM_TYPE_STATUS,
                                                PMC_ADSP_REG_STAT_INT_EVENT,
                                                mask,
                                                ofs,
                                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_STAT_INT_EVENT_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_STAT_INT_EVENT_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_STAT_INT_EVENT);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_STAT_INT_EVENT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_reg_STAT_INT_EVENT_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_reg_STAT_INT_EVENT_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "s16_adapt_dsp_16_reg_STAT_INT_EVENT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return s16_adapt_dsp_16_poll( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_STAT_INT_EVENT,
                                  mask,
                                  value,
                                  cmp,
                                  max_count,
                                  num_failed_polls,
                                  delay_between_polls_in_microseconds);

}


static INLINE void s16_adapt_dsp_16_reg_STAT_INT_EN_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_STAT_INT_EN_write( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "s16_adapt_dsp_16_reg_STAT_INT_EN_write", value );
    s16_adapt_dsp_16_reg_write( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_STAT_INT_EN,
                                value);
}

static INLINE void s16_adapt_dsp_16_reg_STAT_INT_EN_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_reg_STAT_INT_EN_field_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "s16_adapt_dsp_16_reg_STAT_INT_EN_field_set", A, mask, ofs, value );
    s16_adapt_dsp_16_field_set( b_ptr,
                                h_ptr,
                                A,
                                MEM_TYPE_STATUS,
                                PMC_ADSP_REG_STAT_INT_EN,
                                mask,
                                PMC_ADSP_REG_STAT_INT_EN_UNUSED_MASK,
                                ofs,
                                value);

}

static INLINE UINT32 s16_adapt_dsp_16_reg_STAT_INT_EN_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_STAT_INT_EN_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_STAT_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_STAT_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_adapt_dsp_16_reg_STAT_INT_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_STAT_INT_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_STAT_INT);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_STAT_INT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_reg_STAT_INT_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_reg_STAT_INT_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "s16_adapt_dsp_16_reg_STAT_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return s16_adapt_dsp_16_poll( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_STAT_INT,
                                  mask,
                                  value,
                                  cmp,
                                  max_count,
                                  num_failed_polls,
                                  delay_between_polls_in_microseconds);

}


static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 s16_adapt_dsp_16_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = s16_adapt_dsp_16_reg_read( b_ptr,
                                           h_ptr,
                                           A,
                                           MEM_TYPE_STATUS,
                                           PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS);

    IOLOG( "%s -> 0x%08x; A=%d", "s16_adapt_dsp_16_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void s16_adapt_dsp_16_field_ADAPT_CLK_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADAPT_CLK_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_CLK_ENB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADAPT_CLK_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_CLK_ENB_set", A, value );

    /* (0x00000300 bits 0) field ADAPT_CLK_ENB of register PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL */
    s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_MSK,
                                                       ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_OFF,
                                                       value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_CLK_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_CLK_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_CLK_ENB_get", A, 15);
    /* (0x00000300 bits 0) field ADAPT_CLK_ENB of register PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_DSP_CLK_CTRL_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_MSK) >> ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_CLK_ENB_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_set", A, value );

    /* (0x00000304 bits 17) field EQ_ADAPT_CTRL_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_MSK,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_get", A, 15);
    /* (0x00000304 bits 17) field EQ_ADAPT_CTRL_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_EQ_ADAPT_CTRL_ENB_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_INV_DFE_OUT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_INV_DFE_OUT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_INV_DFE_OUT_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_INV_DFE_OUT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_INV_DFE_OUT_set", A, value );

    /* (0x00000304 bits 15) field INV_DFE_OUT of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_MSK,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_INV_DFE_OUT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_INV_DFE_OUT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_INV_DFE_OUT_get", A, 15);
    /* (0x00000304 bits 15) field INV_DFE_OUT of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_INV_DFE_OUT_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_ADAPT_INITB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADAPT_INITB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_INITB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADAPT_INITB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_INITB_set", A, value );

    /* (0x00000304 bits 14) field ADAPT_INITB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_MSK,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_INITB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_INITB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_INITB_get", A, 15);
    /* (0x00000304 bits 14) field ADAPT_INITB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_INITB_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_PRELOAD_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PRELOAD_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PRELOAD_ENB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PRELOAD_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PRELOAD_ENB_set", A, value );

    /* (0x00000304 bits 11) field PRELOAD_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_MSK,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PRELOAD_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PRELOAD_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PRELOAD_ENB_get", A, 15);
    /* (0x00000304 bits 11) field PRELOAD_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PRELOAD_ENB_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_ADAPT_DELAY_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADAPT_DELAY_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_DELAY_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADAPT_DELAY_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_DELAY_set", A, value );

    /* (0x00000304 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_DELAY_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_DELAY_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_DELAY_get", A, 15);
    /* (0x00000304 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_DELAY_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_ADAPT_DELAY_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_ADAPT_DELAY_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADAPT_DELAY_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADAPT_DELAY_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADAPT_DELAY_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADAPT_DELAY_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000304 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
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
        /* (0x00000304 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
        s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF + subfield_offset),
                                                     ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_ADAPT_DELAY_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADAPT_DELAY_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADAPT_DELAY_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADAPT_DELAY_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADAPT_DELAY_get", stop_bit, 2 );
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
    /* (0x00000304 bits 10:8) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK)
                  >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADAPT_DELAY_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_ADAPT_LOOPS_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADAPT_LOOPS_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_LOOPS_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADAPT_LOOPS_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_LOOPS_set", A, value );

    /* (0x00000304 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_LOOPS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_LOOPS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_LOOPS_get", A, 15);
    /* (0x00000304 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_LOOPS_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_ADAPT_LOOPS_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_ADAPT_LOOPS_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADAPT_LOOPS_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADAPT_LOOPS_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADAPT_LOOPS_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADAPT_LOOPS_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000304 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
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
        /* (0x00000304 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
        s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF + subfield_offset),
                                                     ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_ADAPT_LOOPS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADAPT_LOOPS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADAPT_LOOPS_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADAPT_LOOPS_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADAPT_LOOPS_get", stop_bit, 3 );
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
    /* (0x00000304 bits 7:4) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK)
                  >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADAPT_LOOPS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_ADAPT_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADAPT_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_ENB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADAPT_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_ENB_set", A, value );

    /* (0x00000304 bits 3) field ADAPT_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_MSK,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_ENB_get", A, 15);
    /* (0x00000304 bits 3) field ADAPT_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_ENB_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_LMS_DATA_SEL_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_LMS_DATA_SEL_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_LMS_DATA_SEL_set", A, 15);
    if (value > 262143)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_LMS_DATA_SEL_set", value, 262143);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_LMS_DATA_SEL_set", A, value );

    /* (0x00000308 bits 17:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK,
                                                 ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_LMS_DATA_SEL_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_LMS_DATA_SEL_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_LMS_DATA_SEL_get", A, 15);
    /* (0x00000308 bits 17:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK) >> ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_LMS_DATA_SEL_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_LMS_DATA_SEL_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_LMS_DATA_SEL_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_LMS_DATA_SEL_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_LMS_DATA_SEL_set", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_LMS_DATA_SEL_set", stop_bit, 17 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_LMS_DATA_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000308 bits 17:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
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
        if (stop_bit < 17) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 17;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000308 bits 17:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
        s16_adapt_dsp_16_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF + subfield_offset),
                                                     ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_LMS_DATA_SEL_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_LMS_DATA_SEL_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_LMS_DATA_SEL_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_LMS_DATA_SEL_get", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_LMS_DATA_SEL_get", stop_bit, 17 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 17) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 17;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000308 bits 17:0) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_2_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK)
                  >> ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK, ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_LMS_DATA_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_OBJECTS_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_OBJECTS_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_OBJECTS_EN_set", A, 15);
    if (value > 262143)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_OBJECTS_EN_set", value, 262143);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_OBJECTS_EN_set", A, value );

    /* (0x0000030c bits 17:0) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_3 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_3_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_MSK,
                                                 ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_OBJECTS_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_OBJECTS_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_OBJECTS_EN_get", A, 15);
    /* (0x0000030c bits 17:0) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_3 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_3_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_MSK) >> ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_OBJECTS_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_OBJECTS_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_OBJECTS_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_OBJECTS_EN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_OBJECTS_EN_set", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_OBJECTS_EN_set", stop_bit, 17 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_OBJECTS_EN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000030c bits 17:0) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_3 */
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
        if (stop_bit < 17) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 17;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000030c bits 17:0) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_3 */
        s16_adapt_dsp_16_reg_ADAPT_CTRL_3_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     subfield_mask << (ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_OFF + subfield_offset),
                                                     ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_OBJECTS_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_OBJECTS_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_OBJECTS_EN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_OBJECTS_EN_get", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_OBJECTS_EN_get", stop_bit, 17 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 17) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 17;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000030c bits 17:0) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_3 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_3_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_MSK)
                  >> ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_MSK, ADSP_REG_ADAPT_CTRL_3_BIT_OBJECTS_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_OBJECTS_EN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_EXPECTED_EQ2_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_EXPECTED_EQ2_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_EXPECTED_EQ2_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_EXPECTED_EQ2_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_EXPECTED_EQ2_set", A, value );

    /* (0x00000334 bits 31:28) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK,
                                             ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF,
                                             value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_EXPECTED_EQ2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_EXPECTED_EQ2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_EXPECTED_EQ2_get", A, 15);
    /* (0x00000334 bits 31:28) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK) >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_EXPECTED_EQ2_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_EXPECTED_EQ2_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_EXPECTED_EQ2_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ2_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ2_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ2_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_EXPECTED_EQ2_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000334 bits 31:28) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x00000334 bits 31:28) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
        s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF + subfield_offset),
                                                 ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_EXPECTED_EQ2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_EXPECTED_EQ2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ2_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ2_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ2_get", stop_bit, 3 );
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
    /* (0x00000334 bits 31:28) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_EXPECTED_EQ2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_EXPECTED_EQ1_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_EXPECTED_EQ1_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_EXPECTED_EQ1_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_EXPECTED_EQ1_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_EXPECTED_EQ1_set", A, value );

    /* (0x00000334 bits 27:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK,
                                             ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF,
                                             value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_EXPECTED_EQ1_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_EXPECTED_EQ1_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_EXPECTED_EQ1_get", A, 15);
    /* (0x00000334 bits 27:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK) >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_EXPECTED_EQ1_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_EXPECTED_EQ1_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_EXPECTED_EQ1_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ1_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ1_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ1_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_EXPECTED_EQ1_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000334 bits 27:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x00000334 bits 27:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
        s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF + subfield_offset),
                                                 ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_EXPECTED_EQ1_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_EXPECTED_EQ1_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ1_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ1_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_EXPECTED_EQ1_get", stop_bit, 3 );
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
    /* (0x00000334 bits 27:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_EXPECTED_EQ1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_H_LIMIT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_H_LIMIT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_H_LIMIT_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_H_LIMIT_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_H_LIMIT_set", A, value );

    /* (0x00000334 bits 23:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK,
                                             ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF,
                                             value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_H_LIMIT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_H_LIMIT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_H_LIMIT_get", A, 15);
    /* (0x00000334 bits 23:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK) >> ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_H_LIMIT_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_H_LIMIT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_H_LIMIT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_H_LIMIT_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_H_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_H_LIMIT_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_H_LIMIT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000334 bits 23:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x00000334 bits 23:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
        s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF + subfield_offset),
                                                 ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_H_LIMIT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_H_LIMIT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_H_LIMIT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_H_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_H_LIMIT_get", stop_bit, 7 );
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
    /* (0x00000334 bits 23:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK, ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_H_LIMIT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_S_LIMIT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_S_LIMIT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_S_LIMIT_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_S_LIMIT_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_S_LIMIT_set", A, value );

    /* (0x00000334 bits 15:8) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK,
                                             ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF,
                                             value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_S_LIMIT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_S_LIMIT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_S_LIMIT_get", A, 15);
    /* (0x00000334 bits 15:8) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK) >> ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_S_LIMIT_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_S_LIMIT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_S_LIMIT_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_S_LIMIT_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_S_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_S_LIMIT_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_S_LIMIT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000334 bits 15:8) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x00000334 bits 15:8) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
        s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF + subfield_offset),
                                                 ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_S_LIMIT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_S_LIMIT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_S_LIMIT_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_S_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_S_LIMIT_get", stop_bit, 7 );
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
    /* (0x00000334 bits 15:8) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK, ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_S_LIMIT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_OFL_DECAY_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_OFL_DECAY_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_OFL_DECAY_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_OFL_DECAY_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_OFL_DECAY_set", A, value );

    /* (0x00000334 bits 7:2) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK,
                                             ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF,
                                             value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_OFL_DECAY_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_OFL_DECAY_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_OFL_DECAY_get", A, 15);
    /* (0x00000334 bits 7:2) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK) >> ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_OFL_DECAY_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_OFL_DECAY_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_OFL_DECAY_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_OFL_DECAY_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_OFL_DECAY_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_OFL_DECAY_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_OFL_DECAY_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000334 bits 7:2) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000334 bits 7:2) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
        s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF + subfield_offset),
                                                 ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_OFL_DECAY_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_OFL_DECAY_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_OFL_DECAY_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_OFL_DECAY_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_OFL_DECAY_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000334 bits 7:2) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK, ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_OFL_DECAY_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_set", A, value );

    /* (0x00000334 bits 1) field PATH_PATTERN_CHK_ENB of register PMC_ADSP_REG_DSS_CTRL */
    s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_MSK,
                                             ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_OFF,
                                             value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_get", A, 15);
    /* (0x00000334 bits 1) field PATH_PATTERN_CHK_ENB of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_MSK) >> ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PATH_PATTERN_CHK_ENB_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DSS_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DSS_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DSS_ENB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DSS_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DSS_ENB_set", A, value );

    /* (0x00000334 bits 0) field DSS_ENB of register PMC_ADSP_REG_DSS_CTRL */
    s16_adapt_dsp_16_reg_DSS_CTRL_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             ADSP_REG_DSS_CTRL_BIT_DSS_ENB_MSK,
                                             ADSP_REG_DSS_CTRL_BIT_DSS_ENB_OFF,
                                             value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DSS_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DSS_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DSS_ENB_get", A, 15);
    /* (0x00000334 bits 0) field DSS_ENB of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = s16_adapt_dsp_16_reg_DSS_CTRL_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_DSS_ENB_MSK) >> ADSP_REG_DSS_CTRL_BIT_DSS_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DSS_ENB_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_set", A, value );

    /* (0x00000338 bits 30) field DFE_COEF5_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_get", A, 15);
    /* (0x00000338 bits 30) field DFE_COEF5_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_set", A, value );

    /* (0x00000338 bits 29) field DFE_COEF4_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_get", A, 15);
    /* (0x00000338 bits 29) field DFE_COEF4_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_set", A, value );

    /* (0x00000338 bits 28) field DFE_COEF3_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_get", A, 15);
    /* (0x00000338 bits 28) field DFE_COEF3_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_set", A, value );

    /* (0x00000338 bits 27) field DFE_COEF2_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_get", A, 15);
    /* (0x00000338 bits 27) field DFE_COEF2_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_set", A, value );

    /* (0x00000338 bits 26) field DFE_COEF1_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_get", A, 15);
    /* (0x00000338 bits 26) field DFE_COEF1_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_set", A, value );

    /* (0x00000338 bits 25) field DFE_COEF0_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_get", A, 15);
    /* (0x00000338 bits 25) field DFE_COEF0_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_set", A, value );

    /* (0x00000338 bits 24) field TARGET_AMP_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_get", A, 15);
    /* (0x00000338 bits 24) field TARGET_AMP_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_TARGET_AMP_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF5_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF5_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_set", A, value );

    /* (0x00000338 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF5_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF5_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_get", A, 15);
    /* (0x00000338 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000338 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000338 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_get", stop_bit, 3 );
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
    /* (0x00000338 bits 23:20) field DFE_COEF5_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF5_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF5_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF4_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF4_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_set", A, value );

    /* (0x00000338 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF4_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF4_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_get", A, 15);
    /* (0x00000338 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000338 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000338 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_get", stop_bit, 3 );
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
    /* (0x00000338 bits 19:16) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF4_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF3_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF3_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_set", A, value );

    /* (0x00000338 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF3_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF3_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_get", A, 15);
    /* (0x00000338 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000338 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000338 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_get", stop_bit, 3 );
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
    /* (0x00000338 bits 15:12) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF3_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF2_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF2_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_set", A, value );

    /* (0x00000338 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF2_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF2_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_get", A, 15);
    /* (0x00000338 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000338 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000338 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_get", stop_bit, 3 );
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
    /* (0x00000338 bits 11:8) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF2_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF1_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF1_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_set", A, value );

    /* (0x00000338 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF1_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF1_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_get", A, 15);
    /* (0x00000338 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000338 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000338 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_get", stop_bit, 3 );
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
    /* (0x00000338 bits 7:4) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF1_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF0_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF0_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_set", A, value );

    /* (0x00000338 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF0_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF0_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_get", A, 15);
    /* (0x00000338 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000338 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000338 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        s16_adapt_dsp_16_reg_DSS_TAPS_1_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_get", stop_bit, 3 );
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
    /* (0x00000338 bits 3:0) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_1_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF0_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_GAIN_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_GAIN_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                        s16_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_GAIN_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_GAIN_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_GAIN_TAP_set", A, value );

    /* (0x0000033c bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    s16_adapt_dsp_16_reg_DSS_TAPS_2_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_GAIN_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_GAIN_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_GAIN_TAP_get", A, 15);
    /* (0x0000033c bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_2_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK) >> ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_GAIN_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_GAIN_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_GAIN_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_GAIN_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_GAIN_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_GAIN_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_GAIN_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000033c bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
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
        /* (0x0000033c bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
        s16_adapt_dsp_16_reg_DSS_TAPS_2_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_GAIN_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_GAIN_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_GAIN_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_GAIN_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_GAIN_TAP_get", stop_bit, 3 );
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
    /* (0x0000033c bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_2_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_MSK, ADSP_REG_DSS_TAPS_2_BIT_GAIN_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_GAIN_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TAP_OFL2_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TAP_OFL2_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TAP_OFL2_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TAP_OFL2_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TAP_OFL2_EN_set", A, value );

    /* (0x00000340 bits 30) field TAP_OFL2_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_TAP_OFL2_EN_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_TAP_OFL2_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TAP_OFL2_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TAP_OFL2_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TAP_OFL2_EN_get", A, 15);
    /* (0x00000340 bits 30) field TAP_OFL2_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TAP_OFL2_EN_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_TAP_OFL2_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TAP_OFL2_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TAP2_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TAP2_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                    s16_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TAP2_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TAP2_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TAP2_set", A, value );

    /* (0x00000340 bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TAP2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TAP2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                      s16_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TAP2_get", A, 15);
    /* (0x00000340 bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TAP2_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_TAP2_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_TAP2_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TAP2_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TAP2_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TAP2_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TAP2_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000340 bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        /* (0x00000340 bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
        s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_TAP2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TAP2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TAP2_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TAP2_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TAP2_get", stop_bit, 2 );
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
    /* (0x00000340 bits 29:27) field TAP2 of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_TAP2_MSK, ADSP_REG_DSS_TAPS_3_BIT_TAP2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TAP2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_set", A, value );

    /* (0x00000340 bits 26) field DP_FFE_C_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_get", A, 15);
    /* (0x00000340 bits 26) field DP_FFE_C_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_set", A, value );

    /* (0x00000340 bits 25) field DP_FFE_B_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_get", A, 15);
    /* (0x00000340 bits 25) field DP_FFE_B_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_set", A, value );

    /* (0x00000340 bits 24) field DP_FFE_A_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_get", A, 15);
    /* (0x00000340 bits 24) field DP_FFE_A_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_C_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_C_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_set", A, value );

    /* (0x00000340 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_C_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_C_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_get", A, 15);
    /* (0x00000340 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000340 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        /* (0x00000340 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_get", stop_bit, 3 );
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
    /* (0x00000340 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_C_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_C_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_B_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_B_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_set", A, value );

    /* (0x00000340 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_B_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_B_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_get", A, 15);
    /* (0x00000340 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000340 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        /* (0x00000340 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_get", stop_bit, 3 );
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
    /* (0x00000340 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_B_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_B_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_A_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_A_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_set", A, value );

    /* (0x00000340 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_A_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_A_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_get", A, 15);
    /* (0x00000340 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000340 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        /* (0x00000340 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_get", stop_bit, 3 );
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
    /* (0x00000340 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_DP_FFE_A_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_A_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TARGET_AMP_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TARGET_AMP_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_set", A, value );

    /* (0x00000340 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TARGET_AMP_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TARGET_AMP_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_get", A, 15);
    /* (0x00000340 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000340 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        /* (0x00000340 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_get", stop_bit, 3 );
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
    /* (0x00000340 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_TARGET_AMP_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TARGET_AMP_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_ADC_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADC_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                       s16_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADC_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADC_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADC_TAP_set", A, value );

    /* (0x00000340 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADC_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADC_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                         s16_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADC_TAP_get", A, 15);
    /* (0x00000340 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADC_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_ADC_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_ADC_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADC_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADC_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADC_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADC_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000340 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        /* (0x00000340 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_ADC_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADC_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADC_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADC_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADC_TAP_get", stop_bit, 3 );
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
    /* (0x00000340 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_ADC_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADC_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_OFFSET_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_OFFSET_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_OFFSET_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_OFFSET_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_OFFSET_TAP_set", A, value );

    /* (0x00000340 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_OFFSET_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_OFFSET_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_OFFSET_TAP_get", A, 15);
    /* (0x00000340 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK) >> ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_OFFSET_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_OFFSET_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_OFFSET_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_OFFSET_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_OFFSET_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_OFFSET_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_OFFSET_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000340 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
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
        /* (0x00000340 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
        s16_adapt_dsp_16_reg_DSS_TAPS_3_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_OFFSET_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_OFFSET_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_OFFSET_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_OFFSET_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_OFFSET_TAP_get", stop_bit, 3 );
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
    /* (0x00000340 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_3 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_3_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_MSK, ADSP_REG_DSS_TAPS_3_BIT_OFFSET_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_OFFSET_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_set", A, value );

    /* (0x00000344 bits 14) field FETX_POST_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_4 */
    s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_get", A, 15);
    /* (0x00000344 bits 14) field FETX_POST_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_set", A, value );

    /* (0x00000344 bits 13) field FETX_CTR_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_4 */
    s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_get", A, 15);
    /* (0x00000344 bits 13) field FETX_CTR_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_set", A, value );

    /* (0x00000344 bits 12) field FETX_PRE_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_4 */
    s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFL1_EN_MSK,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFL1_EN_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_get", A, 15);
    /* (0x00000344 bits 12) field FETX_PRE_TAP_OFL1_EN of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFL1_EN_MSK) >> ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFL1_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_TAP_OFL1_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_POST_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_POST_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_POST_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_TAP_set", A, value );

    /* (0x00000344 bits 11:8) field FETX_POST_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_POST_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_POST_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_POST_TAP_get", A, 15);
    /* (0x00000344 bits 11:8) field FETX_POST_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_MSK) >> ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FETX_POST_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FETX_POST_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_POST_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_POST_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_POST_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_POST_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000344 bits 11:8) field FETX_POST_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
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
        /* (0x00000344 bits 11:8) field FETX_POST_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
        s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_POST_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_POST_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_POST_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_POST_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_POST_TAP_get", stop_bit, 3 );
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
    /* (0x00000344 bits 11:8) field FETX_POST_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_MSK, ADSP_REG_DSS_TAPS_4_BIT_FETX_POST_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_POST_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_CTR_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_CTR_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_CTR_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_TAP_set", A, value );

    /* (0x00000344 bits 7:4) field FETX_CTR_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_CTR_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_CTR_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_CTR_TAP_get", A, 15);
    /* (0x00000344 bits 7:4) field FETX_CTR_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_MSK) >> ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FETX_CTR_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FETX_CTR_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_CTR_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_CTR_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_CTR_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_CTR_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000344 bits 7:4) field FETX_CTR_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
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
        /* (0x00000344 bits 7:4) field FETX_CTR_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
        s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_CTR_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_CTR_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_CTR_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_CTR_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_CTR_TAP_get", stop_bit, 3 );
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
    /* (0x00000344 bits 7:4) field FETX_CTR_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_MSK, ADSP_REG_DSS_TAPS_4_BIT_FETX_CTR_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_CTR_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_PRE_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_PRE_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_PRE_TAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_TAP_set", A, value );

    /* (0x00000344 bits 3:0) field FETX_PRE_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_MSK,
                                               ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_PRE_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_PRE_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_PRE_TAP_get", A, 15);
    /* (0x00000344 bits 3:0) field FETX_PRE_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_MSK) >> ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_TAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FETX_PRE_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FETX_PRE_TAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_PRE_TAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_PRE_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_PRE_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_PRE_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000344 bits 3:0) field FETX_PRE_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
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
        /* (0x00000344 bits 3:0) field FETX_PRE_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
        s16_adapt_dsp_16_reg_DSS_TAPS_4_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFF + subfield_offset),
                                                   ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_PRE_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_PRE_TAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_PRE_TAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_PRE_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_PRE_TAP_get", stop_bit, 3 );
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
    /* (0x00000344 bits 3:0) field FETX_PRE_TAP of register PMC_ADSP_REG_DSS_TAPS_4 */
    reg_value = s16_adapt_dsp_16_reg_DSS_TAPS_4_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_MSK, ADSP_REG_DSS_TAPS_4_BIT_FETX_PRE_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_PRE_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_set", A, value );

    /* (0x00000348 bits 13) field LSB_HYSTERESIS_EN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_LSB_HYSTERESIS_EN_MSK,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_LSB_HYSTERESIS_EN_OFF,
                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_get", A, 15);
    /* (0x00000348 bits 13) field LSB_HYSTERESIS_EN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_LSB_HYSTERESIS_EN_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_LSB_HYSTERESIS_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_LSB_HYSTERESIS_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DELTA_H_RND_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DELTA_H_RND_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DELTA_H_RND_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DELTA_H_RND_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DELTA_H_RND_EN_set", A, value );

    /* (0x00000348 bits 12) field DELTA_H_RND_EN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_DELTA_H_RND_EN_MSK,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_DELTA_H_RND_EN_OFF,
                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DELTA_H_RND_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DELTA_H_RND_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DELTA_H_RND_EN_get", A, 15);
    /* (0x00000348 bits 12) field DELTA_H_RND_EN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_DELTA_H_RND_EN_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_DELTA_H_RND_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DELTA_H_RND_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_MU_STEP_ADC_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_MU_STEP_ADC_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_MU_STEP_ADC_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_ADC_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_ADC_set", A, value );

    /* (0x00000348 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF,
                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_MU_STEP_ADC_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_MU_STEP_ADC_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_MU_STEP_ADC_get", A, 15);
    /* (0x00000348 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_ADC_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_MU_STEP_ADC_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_MU_STEP_ADC_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_MU_STEP_ADC_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_MU_STEP_ADC_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_MU_STEP_ADC_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_MU_STEP_ADC_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000348 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000348 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
        s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF + subfield_offset),
                                                       ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_MU_STEP_ADC_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_MU_STEP_ADC_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_MU_STEP_ADC_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_MU_STEP_ADC_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_MU_STEP_ADC_get", stop_bit, 2 );
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
    /* (0x00000348 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_MU_STEP_ADC_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_MU_STEP_COEF_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_MU_STEP_COEF_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_MU_STEP_COEF_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_COEF_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_COEF_set", A, value );

    /* (0x00000348 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF,
                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_MU_STEP_COEF_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_MU_STEP_COEF_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_MU_STEP_COEF_get", A, 15);
    /* (0x00000348 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_COEF_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_MU_STEP_COEF_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_MU_STEP_COEF_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_MU_STEP_COEF_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_MU_STEP_COEF_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_MU_STEP_COEF_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_MU_STEP_COEF_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000348 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000348 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
        s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF + subfield_offset),
                                                       ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_MU_STEP_COEF_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_MU_STEP_COEF_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_MU_STEP_COEF_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_MU_STEP_COEF_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_MU_STEP_COEF_get", stop_bit, 2 );
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
    /* (0x00000348 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_MU_STEP_COEF_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_MU_STEP_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_MU_STEP_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_MU_STEP_OFFSET_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_OFFSET_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_OFFSET_set", A, value );

    /* (0x00000348 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF,
                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_MU_STEP_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_MU_STEP_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_MU_STEP_OFFSET_get", A, 15);
    /* (0x00000348 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_OFFSET_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000348 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000348 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
        s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF + subfield_offset),
                                                       ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_get", stop_bit, 2 );
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
    /* (0x00000348 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_MU_STEP_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_MU_STEP_GAIN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_MU_STEP_GAIN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_MU_STEP_GAIN_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_GAIN_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_GAIN_set", A, value );

    /* (0x00000348 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK,
                                                   ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF,
                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_MU_STEP_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_MU_STEP_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_MU_STEP_GAIN_get", A, 15);
    /* (0x00000348 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_MU_STEP_GAIN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_MU_STEP_GAIN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_MU_STEP_GAIN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_MU_STEP_GAIN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_MU_STEP_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_MU_STEP_GAIN_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_MU_STEP_GAIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000348 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000348 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
        s16_adapt_dsp_16_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF + subfield_offset),
                                                       ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_MU_STEP_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_MU_STEP_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_MU_STEP_GAIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_MU_STEP_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_MU_STEP_GAIN_get", stop_bit, 2 );
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
    /* (0x00000348 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = s16_adapt_dsp_16_reg_LMS_MU_FACTORS_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_MU_STEP_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_FETX_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_FETX_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_FETX_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_FETX_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_FETX_set", A, value );

    /* (0x0000034c bits 23:20) field RR_PERIOD_FETX of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_MSK,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_OFF,
                                                        value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_FETX_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_FETX_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_FETX_get", A, 15);
    /* (0x0000034c bits 23:20) field RR_PERIOD_FETX of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_FETX_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000034c bits 23:20) field RR_PERIOD_FETX of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000034c bits 23:20) field RR_PERIOD_FETX of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_OFF + subfield_offset),
                                                            ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_get", stop_bit, 3 );
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
    /* (0x0000034c bits 23:20) field RR_PERIOD_FETX of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_FETX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_FETX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_DSS_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_DSS_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_DSS_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_DSS_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_DSS_set", A, value );

    /* (0x0000034c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF,
                                                        value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_DSS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_DSS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_DSS_get", A, 15);
    /* (0x0000034c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_DSS_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000034c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000034c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF + subfield_offset),
                                                            ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_get", stop_bit, 3 );
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
    /* (0x0000034c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_DSS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_set", A, value );

    /* (0x0000034c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF,
                                                        value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_get", A, 15);
    /* (0x0000034c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_ADC_MAP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000034c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000034c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF + subfield_offset),
                                                            ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_get", stop_bit, 3 );
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
    /* (0x0000034c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_ADC_MAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_set", A, value );

    /* (0x0000034c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF,
                                                        value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_get", A, 15);
    /* (0x0000034c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_OFFSET_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000034c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000034c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF + subfield_offset),
                                                            ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_get", stop_bit, 3 );
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
    /* (0x0000034c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_GAIN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_GAIN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_GAIN_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_GAIN_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_GAIN_set", A, value );

    /* (0x0000034c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF,
                                                        value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_GAIN_get", A, 15);
    /* (0x0000034c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_GAIN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000034c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000034c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF + subfield_offset),
                                                            ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_get", stop_bit, 3 );
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
    /* (0x0000034c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_COEF_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_RR_PERIOD_COEF_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_COEF_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_COEF_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_COEF_set", A, value );

    /* (0x0000034c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK,
                                                        ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF,
                                                        value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_COEF_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_RR_PERIOD_COEF_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_RR_PERIOD_COEF_get", A, 15);
    /* (0x0000034c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_RR_PERIOD_COEF_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000034c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000034c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF + subfield_offset),
                                                            ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_get", stop_bit, 3 );
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
    /* (0x0000034c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = s16_adapt_dsp_16_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_RR_PERIOD_COEF_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_set", A, value );

    /* (0x00000350 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_get", A, 15);
    /* (0x00000350 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TR_FFE_B_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000350 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000350 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_get", stop_bit, 4 );
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
    /* (0x00000350 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TR_FFE_B_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_set", A, value );

    /* (0x00000350 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_get", A, 15);
    /* (0x00000350 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TR_FFE_A_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000350 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000350 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_get", stop_bit, 4 );
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
    /* (0x00000350 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TR_FFE_A_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_set", A, value );

    /* (0x00000350 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_get", A, 15);
    /* (0x00000350 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000350 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000350 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000350 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_C_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_set", A, value );

    /* (0x00000350 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_get", A, 15);
    /* (0x00000350 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000350 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000350 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000350 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_B_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_set", A, value );

    /* (0x00000350 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_get", A, 15);
    /* (0x00000350 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000350 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000350 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000350 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_A_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_set", A, 15);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_set", A, value );

    /* (0x00000354 bits 26:18) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_get", A, 15);
    /* (0x00000354 bits 26:18) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000354 bits 26:18) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x00000354 bits 26:18) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_get", stop_bit, 8 );
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
    /* (0x00000354 bits 26:18) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF2_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_set", A, 15);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_set", A, value );

    /* (0x00000354 bits 17:9) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_get", A, 15);
    /* (0x00000354 bits 17:9) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000354 bits 17:9) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x00000354 bits 17:9) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_get", stop_bit, 8 );
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
    /* (0x00000354 bits 17:9) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF1_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_set", A, 15);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_set", A, value );

    /* (0x00000354 bits 8:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_get", A, 15);
    /* (0x00000354 bits 8:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000354 bits 8:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x00000354 bits 8:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_get", stop_bit, 8 );
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
    /* (0x00000354 bits 8:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF0_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_set", A, 15);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_set", A, value );

    /* (0x00000358 bits 26:18) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_get", A, 15);
    /* (0x00000358 bits 26:18) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000358 bits 26:18) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000358 bits 26:18) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_get", stop_bit, 8 );
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
    /* (0x00000358 bits 26:18) field DFE_COEF5_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF5_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF5_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_set", A, 15);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_set", A, value );

    /* (0x00000358 bits 17:9) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_get", A, 15);
    /* (0x00000358 bits 17:9) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000358 bits 17:9) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000358 bits 17:9) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_get", stop_bit, 8 );
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
    /* (0x00000358 bits 17:9) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF4_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_set", A, 15);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_set", value, 511);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_set", A, value );

    /* (0x00000358 bits 8:0) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_get", A, 15);
    /* (0x00000358 bits 8:0) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_set", stop_bit, 8 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000358 bits 8:0) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000358 bits 8:0) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_get", stop_bit, 8 );
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
    /* (0x00000358 bits 8:0) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF3_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF3_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_set", A, value );

    /* (0x0000035c bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_get", A, 15);
    /* (0x0000035c bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADC_MAP_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000035c bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
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
        /* (0x0000035c bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_get", stop_bit, 3 );
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
    /* (0x0000035c bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_ADC_MAP_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADC_MAP_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_set", A, value );

    /* (0x0000035c bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_get", A, 15);
    /* (0x0000035c bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000035c bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000035c bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000035c bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_TARGET_AMP_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TARGET_AMP_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_set", A, 15);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_set", A, value );

    /* (0x0000035c bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_get", A, 15);
    /* (0x0000035c bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PGA_OFFSET_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000035c bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
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
        /* (0x0000035c bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_get", stop_bit, 7 );
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
    /* (0x0000035c bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_OFFSET_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_PGA_OFFSET_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_set", A, 15);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_set", A, value );

    /* (0x0000035c bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_get", A, 15);
    /* (0x0000035c bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PGA_GAIN_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000035c bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
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
        /* (0x0000035c bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_get", stop_bit, 4 );
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
    /* (0x0000035c bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_4 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_4_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_4_BIT_PGA_GAIN_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_PGA_GAIN_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_POST_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_POST_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_POST_PRELOAD_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_PRELOAD_set", A, value );

    /* (0x00000360 bits 20:14) field FETX_POST_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_POST_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_POST_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_POST_PRELOAD_get", A, 15);
    /* (0x00000360 bits 20:14) field FETX_POST_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000360 bits 20:14) field FETX_POST_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
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
        /* (0x00000360 bits 20:14) field FETX_POST_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000360 bits 20:14) field FETX_POST_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_POST_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_POST_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_set", A, value );

    /* (0x00000360 bits 13:7) field FETX_CTR_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_get", A, 15);
    /* (0x00000360 bits 13:7) field FETX_CTR_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000360 bits 13:7) field FETX_CTR_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
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
        /* (0x00000360 bits 13:7) field FETX_CTR_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000360 bits 13:7) field FETX_CTR_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_CTR_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_CTR_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_set", A, value );

    /* (0x00000360 bits 6:0) field FETX_PRE_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_MSK,
                                                         ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_get", A, 15);
    /* (0x00000360 bits 6:0) field FETX_PRE_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_PRELOAD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000360 bits 6:0) field FETX_PRE_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
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
        /* (0x00000360 bits 6:0) field FETX_PRE_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
        s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_OFF + subfield_offset),
                                                             ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000360 bits 6:0) field FETX_PRE_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_5 */
    reg_value = s16_adapt_dsp_16_reg_OBJECT_PRELOAD_VAL_5_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_5_BIT_FETX_PRE_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_PRE_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_PROG_REF_OVR_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PROG_REF_OVR_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_OVR_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_OVR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_OVR_EN_set", A, value );

    /* (0x00000364 bits 19) field PROG_REF_OVR_EN of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_MSK,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_OVR_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_OVR_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_OVR_EN_get", A, 15);
    /* (0x00000364 bits 19) field PROG_REF_OVR_EN of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_OVR_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_PROG_REF_OVR_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PROG_REF_OVR_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_OVR_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_OVR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_OVR_set", A, value );

    /* (0x00000364 bits 18) field PROG_REF_OVR of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_MSK,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_OVR_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_OVR_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_OVR_get", A, 15);
    /* (0x00000364 bits 18) field PROG_REF_OVR of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_OVR_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_set", A, 15);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_set", A, value );

    /* (0x00000364 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_get", A, 15);
    /* (0x00000364 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_PERIOD_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000364 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x00000364 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF + subfield_offset),
                                                             ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                               s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_get", stop_bit, 3 );
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
    /* (0x00000364 bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_PROG_REF_TIMEOUT_PERIOD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_CLIP_RATE_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_CLIP_RATE_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CLIP_RATE_THRESH_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_CLIP_RATE_THRESH_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_CLIP_RATE_THRESH_set", A, value );

    /* (0x00000364 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_CLIP_RATE_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CLIP_RATE_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CLIP_RATE_THRESH_get", A, 15);
    /* (0x00000364 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CLIP_RATE_THRESH_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000364 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x00000364 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF + subfield_offset),
                                                             ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_get", stop_bit, 2 );
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
    /* (0x00000364 bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_CLIP_RATE_THRESH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_CLIP_RATE_AMP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_CLIP_RATE_AMP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CLIP_RATE_AMP_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_CLIP_RATE_AMP_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_CLIP_RATE_AMP_set", A, value );

    /* (0x00000364 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_CLIP_RATE_AMP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CLIP_RATE_AMP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CLIP_RATE_AMP_get", A, 15);
    /* (0x00000364 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CLIP_RATE_AMP_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000364 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000364 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF + subfield_offset),
                                                             ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000364 bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_CLIP_RATE_AMP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_GAIN_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_GAIN_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_GAIN_OFFSET_set", A, 15);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_GAIN_OFFSET_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_GAIN_OFFSET_set", A, value );

    /* (0x00000364 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_GAIN_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_GAIN_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_GAIN_OFFSET_get", A, 15);
    /* (0x00000364 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_GAIN_OFFSET_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_GAIN_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_GAIN_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_GAIN_OFFSET_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_GAIN_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_GAIN_OFFSET_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_GAIN_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000364 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x00000364 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF + subfield_offset),
                                                             ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_GAIN_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_GAIN_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_GAIN_OFFSET_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_GAIN_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_GAIN_OFFSET_get", stop_bit, 2 );
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
    /* (0x00000364 bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_GAIN_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_ADC_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADC_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADC_OFFSET_set", A, 15);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADC_OFFSET_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADC_OFFSET_set", A, value );

    /* (0x00000364 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK,
                                                         ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF,
                                                         value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADC_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADC_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADC_OFFSET_get", A, 15);
    /* (0x00000364 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADC_OFFSET_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_ADC_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_ADC_OFFSET_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADC_OFFSET_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADC_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADC_OFFSET_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADC_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000364 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x00000364 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF + subfield_offset),
                                                             ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_ADC_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADC_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADC_OFFSET_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADC_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADC_OFFSET_get", stop_bit, 1 );
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
    /* (0x00000364 bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADC_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_set", A, 15);
    if (value > 262143)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_set", value, 262143);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_set", A, value );

    /* (0x00000370 bits 17:0) field FORCE_ADAPT_VALUE_EN of register PMC_ADSP_REG_ADAPT_FORCE_VAL */
    s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_MSK,
                                                    ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_OFF,
                                                    value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_get", A, 15);
    /* (0x00000370 bits 17:0) field FORCE_ADAPT_VALUE_EN of register PMC_ADSP_REG_ADAPT_FORCE_VAL */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_MSK) >> ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FORCE_ADAPT_VALUE_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_set", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_set", stop_bit, 17 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000370 bits 17:0) field FORCE_ADAPT_VALUE_EN of register PMC_ADSP_REG_ADAPT_FORCE_VAL */
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
        if (stop_bit < 17) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 17;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000370 bits 17:0) field FORCE_ADAPT_VALUE_EN of register PMC_ADSP_REG_ADAPT_FORCE_VAL */
        s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        subfield_mask << (ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_OFF + subfield_offset),
                                                        ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_get", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_get", stop_bit, 17 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 17) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 17;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000370 bits 17:0) field FORCE_ADAPT_VALUE_EN of register PMC_ADSP_REG_ADAPT_FORCE_VAL */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_FORCE_VAL_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_MSK)
                  >> ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_MSK, ADSP_REG_ADAPT_FORCE_VAL_BIT_FORCE_ADAPT_VALUE_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FORCE_ADAPT_VALUE_EN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TTIU_REJECT_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TTIU_REJECT_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TTIU_REJECT_EN_set", A, 15);
    if (value > 262143)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TTIU_REJECT_EN_set", value, 262143);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TTIU_REJECT_EN_set", A, value );

    /* (0x00000374 bits 17:0) field TTIU_REJECT_EN of register PMC_ADSP_REG_FETX_TTIU_CTRL */
    s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_MSK,
                                                   ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_OFF,
                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_REJECT_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_REJECT_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TTIU_REJECT_EN_get", A, 15);
    /* (0x00000374 bits 17:0) field TTIU_REJECT_EN of register PMC_ADSP_REG_FETX_TTIU_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_MSK) >> ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TTIU_REJECT_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_set", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_set", stop_bit, 17 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000374 bits 17:0) field TTIU_REJECT_EN of register PMC_ADSP_REG_FETX_TTIU_CTRL */
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
        if (stop_bit < 17) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 17;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000374 bits 17:0) field TTIU_REJECT_EN of register PMC_ADSP_REG_FETX_TTIU_CTRL */
        s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_field_set( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       subfield_mask << (ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_OFF + subfield_offset),
                                                       ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_get", stop_bit, start_bit );
    if (stop_bit > 17) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_get", stop_bit, 17 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 17) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 17;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000374 bits 17:0) field TTIU_REJECT_EN of register PMC_ADSP_REG_FETX_TTIU_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_TTIU_CTRL_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_MSK)
                  >> ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_MSK, ADSP_REG_FETX_TTIU_CTRL_BIT_TTIU_REJECT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TTIU_REJECT_EN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TTIU_COUNT_MAX_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TTIU_COUNT_MAX_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TTIU_COUNT_MAX_set", A, 15);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TTIU_COUNT_MAX_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TTIU_COUNT_MAX_set", A, value );

    /* (0x00000378 bits 30:25) field TTIU_COUNT_MAX of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_COUNT_MAX_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_COUNT_MAX_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TTIU_COUNT_MAX_get", A, 15);
    /* (0x00000378 bits 30:25) field TTIU_COUNT_MAX of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_MSK) >> ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TTIU_COUNT_MAX_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000378 bits 30:25) field TTIU_COUNT_MAX of register PMC_ADSP_REG_FETX_CTRL */
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
        if (stop_bit < 5) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 5;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000378 bits 30:25) field TTIU_COUNT_MAX of register PMC_ADSP_REG_FETX_CTRL */
        s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_OFF + subfield_offset),
                                                  ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000378 bits 30:25) field TTIU_COUNT_MAX of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_MSK)
                  >> ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_MSK, ADSP_REG_FETX_CTRL_BIT_TTIU_COUNT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TTIU_COUNT_MAX_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_set", A, value );

    /* (0x00000378 bits 24) field FETX_UPDATE_REQ_CLR_EN of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_UPDATE_REQ_CLR_EN_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_UPDATE_REQ_CLR_EN_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_get", A, 15);
    /* (0x00000378 bits 24) field FETX_UPDATE_REQ_CLR_EN of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_FETX_UPDATE_REQ_CLR_EN_MSK) >> ADSP_REG_FETX_CTRL_BIT_FETX_UPDATE_REQ_CLR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_UPDATE_REQ_CLR_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_set", A, value );

    /* (0x00000378 bits 23) field FETX_READY_CHECK_ENB of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_READY_CHECK_ENB_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_READY_CHECK_ENB_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_get", A, 15);
    /* (0x00000378 bits 23) field FETX_READY_CHECK_ENB of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_FETX_READY_CHECK_ENB_MSK) >> ADSP_REG_FETX_CTRL_BIT_FETX_READY_CHECK_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_READY_CHECK_ENB_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_set", A, value );

    /* (0x00000378 bits 22) field SWAP_DEC_INC_EN of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_SWAP_DEC_INC_EN_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_SWAP_DEC_INC_EN_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_get", A, 15);
    /* (0x00000378 bits 22) field SWAP_DEC_INC_EN of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_SWAP_DEC_INC_EN_MSK) >> ADSP_REG_FETX_CTRL_BIT_SWAP_DEC_INC_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_SWAP_DEC_INC_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_set", A, value );

    /* (0x00000378 bits 21) field HANDLE_MIN_FETX_PRE_EN of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_PRE_EN_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_PRE_EN_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_get", A, 15);
    /* (0x00000378 bits 21) field HANDLE_MIN_FETX_PRE_EN of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_PRE_EN_MSK) >> ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_PRE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_PRE_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_set", A, value );

    /* (0x00000378 bits 20) field HANDLE_MAX_FETX_PRE_EN of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_PRE_EN_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_PRE_EN_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_get", A, 15);
    /* (0x00000378 bits 20) field HANDLE_MAX_FETX_PRE_EN of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_PRE_EN_MSK) >> ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_PRE_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_PRE_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_set", A, value );

    /* (0x00000378 bits 19) field HANDLE_MIN_FETX_CTR_EN of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_CTR_EN_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_CTR_EN_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_get", A, 15);
    /* (0x00000378 bits 19) field HANDLE_MIN_FETX_CTR_EN of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_CTR_EN_MSK) >> ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_CTR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_CTR_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_set", A, value );

    /* (0x00000378 bits 18) field HANDLE_MAX_FETX_CTR_EN of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_CTR_EN_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_CTR_EN_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_get", A, 15);
    /* (0x00000378 bits 18) field HANDLE_MAX_FETX_CTR_EN of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_CTR_EN_MSK) >> ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_CTR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_CTR_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_set", A, value );

    /* (0x00000378 bits 17) field HANDLE_MIN_FETX_POST_EN of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_POST_EN_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_POST_EN_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_get", A, 15);
    /* (0x00000378 bits 17) field HANDLE_MIN_FETX_POST_EN of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_POST_EN_MSK) >> ADSP_REG_FETX_CTRL_BIT_HANDLE_MIN_FETX_POST_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MIN_FETX_POST_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_set", A, value );

    /* (0x00000378 bits 16) field HANDLE_MAX_FETX_POST_EN of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_POST_EN_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_POST_EN_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_get", A, 15);
    /* (0x00000378 bits 16) field HANDLE_MAX_FETX_POST_EN of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_POST_EN_MSK) >> ADSP_REG_FETX_CTRL_BIT_HANDLE_MAX_FETX_POST_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_HANDLE_MAX_FETX_POST_EN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_set", A, value );

    /* (0x00000378 bits 15) field FETX_COEF_SET_SEL of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_COEF_SET_SEL_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_COEF_SET_SEL_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_get", A, 15);
    /* (0x00000378 bits 15) field FETX_COEF_SET_SEL of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_FETX_COEF_SET_SEL_MSK) >> ADSP_REG_FETX_CTRL_BIT_FETX_COEF_SET_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_COEF_SET_SEL_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_set", A, value );

    /* (0x00000378 bits 14:8) field FETX_ADJ_NEG_THRESH of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_get", A, 15);
    /* (0x00000378 bits 14:8) field FETX_ADJ_NEG_THRESH of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_MSK) >> ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_NEG_THRESH_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000378 bits 14:8) field FETX_ADJ_NEG_THRESH of register PMC_ADSP_REG_FETX_CTRL */
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
        /* (0x00000378 bits 14:8) field FETX_ADJ_NEG_THRESH of register PMC_ADSP_REG_FETX_CTRL */
        s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_OFF + subfield_offset),
                                                  ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_get", stop_bit, 6 );
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
    /* (0x00000378 bits 14:8) field FETX_ADJ_NEG_THRESH of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_MSK)
                  >> ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_MSK, ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_NEG_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_ADJ_NEG_THRESH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                   s16_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_set", A, 15);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_set", A, value );

    /* (0x00000378 bits 7:1) field FETX_ADJ_POS_THRESH of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_get", A, 15);
    /* (0x00000378 bits 7:1) field FETX_ADJ_POS_THRESH of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_MSK) >> ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_POS_THRESH_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000378 bits 7:1) field FETX_ADJ_POS_THRESH of register PMC_ADSP_REG_FETX_CTRL */
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
        /* (0x00000378 bits 7:1) field FETX_ADJ_POS_THRESH of register PMC_ADSP_REG_FETX_CTRL */
        s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  subfield_mask << (ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_OFF + subfield_offset),
                                                  ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_get", stop_bit, 6 );
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
    /* (0x00000378 bits 7:1) field FETX_ADJ_POS_THRESH of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    field_value = (reg_value & ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_MSK)
                  >> ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_MSK, ADSP_REG_FETX_CTRL_BIT_FETX_ADJ_POS_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_ADJ_POS_THRESH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_ADAPT_MODE_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_ADAPT_MODE_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_ADAPT_MODE_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_ADAPT_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_ADAPT_MODE_set", A, value );

    /* (0x00000378 bits 0) field FETX_ADAPT_MODE of register PMC_ADSP_REG_FETX_CTRL */
    s16_adapt_dsp_16_reg_FETX_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_ADAPT_MODE_MSK,
                                              ADSP_REG_FETX_CTRL_BIT_FETX_ADAPT_MODE_OFF,
                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_ADAPT_MODE_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_ADAPT_MODE_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_ADAPT_MODE_get", A, 15);
    /* (0x00000378 bits 0) field FETX_ADAPT_MODE of register PMC_ADSP_REG_FETX_CTRL */
    reg_value = s16_adapt_dsp_16_reg_FETX_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_FETX_CTRL_BIT_FETX_ADAPT_MODE_MSK) >> ADSP_REG_FETX_CTRL_BIT_FETX_ADAPT_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_ADAPT_MODE_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_TTIU_FM_PATTERN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_TTIU_FM_PATTERN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TTIU_FM_PATTERN_set", A, 15);
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_TTIU_FM_PATTERN_set", value, 16383);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_TTIU_FM_PATTERN_set", A, value );

    /* (0x0000037c bits 13:0) field TTIU_FM_PATTERN of register PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN */
    s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_MSK,
                                                              ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_OFF,
                                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_FM_PATTERN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_FM_PATTERN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TTIU_FM_PATTERN_get", A, 15);
    /* (0x0000037c bits 13:0) field TTIU_FM_PATTERN of register PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN */
    reg_value = s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_MSK) >> ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TTIU_FM_PATTERN_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_set", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_set", stop_bit, 13 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000037c bits 13:0) field TTIU_FM_PATTERN of register PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN */
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
        if (stop_bit < 13) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 13;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000037c bits 13:0) field TTIU_FM_PATTERN of register PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN */
        s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  subfield_mask << (ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_OFF + subfield_offset),
                                                                  ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_get", stop_bit, 13 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 13) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 13;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000037c bits 13:0) field TTIU_FM_PATTERN of register PMC_ADSP_REG_TTIU_FRAME_MARKER_PATTERN */
    reg_value = s16_adapt_dsp_16_reg_TTIU_FRAME_MARKER_PATTERN_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_MSK)
                  >> ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_MSK, ADSP_REG_TTIU_FRAME_MARKER_PATTERN_BIT_TTIU_FM_PATTERN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TTIU_FM_PATTERN_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void s16_adapt_dsp_16_field_ADAPT_START_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_ADAPT_START_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_START_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_ADAPT_START_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_START_set", A, value );

    /* (0x00000304 bits 18) field ADAPT_START of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    s16_adapt_dsp_16_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_START_MSK,
                                                 ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_START_OFF,
                                                 value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_START_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_START_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                             s16_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_START_get", A, 15);
    /* (0x00000304 bits 18) field ADAPT_START of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_CTRL_1_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_START_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_START_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_START_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                            s16_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_set", A, value );

    /* (0x00000310 bits 0) field CAPTURE_ADAPT_STATUS_REQUEST of register PMC_ADSP_REG_ADAPT_STAT */
    s16_adapt_dsp_16_reg_ADAPT_STAT_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_MSK,
                                               ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_OFF,
                                               value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_get", A, 15);
    /* (0x00000310 bits 0) field CAPTURE_ADAPT_STATUS_REQUEST of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_STAT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_MSK) >> ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_ADAPT_STATUS_REQUEST_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                       s16_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_set", A, value );

    /* (0x00000314 bits 0) field CAPTURE_RESULTS_REQUEST of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST */
    s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_MSK,
                                                                  ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_OFF,
                                                                  value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                         s16_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_get", A, 15);
    /* (0x00000314 bits 0) field CAPTURE_RESULTS_REQUEST of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( b_ptr,
                                                                         h_ptr,
                                                                         A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_RESULTS_REQUEST_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", A, value );

    /* (0x00000368 bits 9) field CAPTURE_GAIN_CLIP_FLAG_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_MSK,
                                                              ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_OFF,
                                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get", A, 15);
    /* (0x00000368 bits 9) field CAPTURE_GAIN_CLIP_FLAG_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                              s16_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", A, value );

    /* (0x00000368 bits 0) field CAPTURE_ADC_CLIP_FLAGS_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_MSK,
                                                              ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_OFF,
                                                              value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get", A, 15);
    /* (0x00000368 bits 0) field CAPTURE_ADC_CLIP_FLAGS_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_FETX_ADJ_CLR_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_FETX_ADJ_CLR_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_ADJ_CLR_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_CLR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_CLR_set", A, value );

    /* (0x00000380 bits 1) field FETX_ADJ_CLR of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_ADJ_CLR_MSK,
                                                    ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_ADJ_CLR_OFF,
                                                    value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_FETX_ADJ_CLR_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_ADJ_CLR_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_ADJ_CLR_get", A, 15);
    /* (0x00000380 bits 1) field FETX_ADJ_CLR of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_ADJ_CLR_MSK) >> ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_ADJ_CLR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_ADJ_CLR_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_set", A, value );

    /* (0x00000380 bits 0) field CAPTURE_FETX_ADJ_REQUEST of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    ADSP_REG_FETX_ADJ_STATUS_BIT_CAPTURE_FETX_ADJ_REQUEST_MSK,
                                                    ADSP_REG_FETX_ADJ_STATUS_BIT_CAPTURE_FETX_ADJ_REQUEST_OFF,
                                                    value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                          s16_handle_t *h_ptr,
                                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_get", A, 15);
    /* (0x00000380 bits 0) field CAPTURE_FETX_ADJ_REQUEST of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_CAPTURE_FETX_ADJ_REQUEST_MSK) >> ADSP_REG_FETX_ADJ_STATUS_BIT_CAPTURE_FETX_ADJ_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CAPTURE_FETX_ADJ_REQUEST_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void s16_adapt_dsp_16_field_PROG_REF_DONE_I_set_to_clear( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PROG_REF_DONE_I_set_to_clear( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                        s16_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_DONE_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_I_set_to_clear", A, value );

    /* (0x0000032c bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    s16_adapt_dsp_16_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   A,
                                                                   ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK,
                                                                   ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF,
                                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_DONE_I_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_DONE_I_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_DONE_I_get", A, 15);
    /* (0x0000032c bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_EVENT_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_PROG_REF_DONE_I_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_PROG_REF_DONE_I_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_I_poll", A, value );

    /* (0x0000032c bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return s16_adapt_dsp_16_reg_STAT_INT_EVENT_poll( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK,
                                                     (value<<ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_set_to_clear( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_set_to_clear( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                           s16_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_set_to_clear", A, value );

    /* (0x0000032c bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    s16_adapt_dsp_16_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   A,
                                                                   ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK,
                                                                   ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF,
                                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_get", A, 15);
    /* (0x0000032c bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_EVENT_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_I_poll", A, value );

    /* (0x0000032c bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return s16_adapt_dsp_16_reg_STAT_INT_EVENT_poll( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK,
                                                     (value<<ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE void s16_adapt_dsp_16_field_STOP_ADAPT_I_set_to_clear( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_STOP_ADAPT_I_set_to_clear( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_STOP_ADAPT_I_set_to_clear", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_I_set_to_clear", A, value );

    /* (0x0000032c bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    s16_adapt_dsp_16_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   A,
                                                                   ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK,
                                                                   ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF,
                                                                   value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_STOP_ADAPT_I_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_STOP_ADAPT_I_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_STOP_ADAPT_I_get", A, 15);
    /* (0x0000032c bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_EVENT_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_STOP_ADAPT_I_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_STOP_ADAPT_I_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_I_poll", A, value );

    /* (0x0000032c bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return s16_adapt_dsp_16_reg_STAT_INT_EVENT_poll( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK,
                                                     (value<<ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void s16_adapt_dsp_16_field_PROG_REF_DONE_E_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PROG_REF_DONE_E_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_DONE_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_E_set", A, value );

    /* (0x00000328 bits 9) field PROG_REF_DONE_E of register PMC_ADSP_REG_STAT_INT_EN */
    s16_adapt_dsp_16_reg_STAT_INT_EN_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_MSK,
                                                ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_OFF,
                                                value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_DONE_E_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_DONE_E_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_DONE_E_get", A, 15);
    /* (0x00000328 bits 9) field PROG_REF_DONE_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_EN_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_E_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_set", A, value );

    /* (0x00000328 bits 8) field PROG_REF_TIMEOUT_E of register PMC_ADSP_REG_STAT_INT_EN */
    s16_adapt_dsp_16_reg_STAT_INT_EN_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_MSK,
                                                ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_OFF,
                                                value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_get", A, 15);
    /* (0x00000328 bits 8) field PROG_REF_TIMEOUT_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_EN_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_E_get", A, value );

    return value;
}
static INLINE void s16_adapt_dsp_16_field_STOP_ADAPT_E_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void s16_adapt_dsp_16_field_STOP_ADAPT_E_set( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_STOP_ADAPT_E_set", A, 15);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_E_set", A, value );

    /* (0x00000328 bits 0) field STOP_ADAPT_E of register PMC_ADSP_REG_STAT_INT_EN */
    s16_adapt_dsp_16_reg_STAT_INT_EN_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_MSK,
                                                ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_OFF,
                                                value);
}

static INLINE UINT32 s16_adapt_dsp_16_field_STOP_ADAPT_E_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_STOP_ADAPT_E_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_STOP_ADAPT_E_get", A, 15);
    /* (0x00000328 bits 0) field STOP_ADAPT_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_EN_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_DONE_V_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_DONE_V_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_DONE_V_get", A, 15);
    /* (0x00000330 bits 9) field PROG_REF_DONE_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_MSK) >> ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_PROG_REF_DONE_V_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_PROG_REF_DONE_V_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                s16_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_DONE_V_poll", A, value );

    /* (0x00000330 bits 9) field PROG_REF_DONE_V of register PMC_ADSP_REG_STAT_INT */
    return s16_adapt_dsp_16_reg_STAT_INT_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_MSK,
                                               (value<<ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_V_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_V_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_V_get", A, 15);
    /* (0x00000330 bits 8) field PROG_REF_TIMEOUT_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_MSK) >> ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_V_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_V_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                                   s16_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_adapt_dsp_16_field_PROG_REF_TIMEOUT_V_poll", A, value );

    /* (0x00000330 bits 8) field PROG_REF_TIMEOUT_V of register PMC_ADSP_REG_STAT_INT */
    return s16_adapt_dsp_16_reg_STAT_INT_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_MSK,
                                               (value<<ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 s16_adapt_dsp_16_field_STOP_ADAPT_V_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_STOP_ADAPT_V_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_STOP_ADAPT_V_get", A, 15);
    /* (0x00000330 bits 0) field STOP_ADAPT_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = s16_adapt_dsp_16_reg_STAT_INT_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_MSK) >> ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_STOP_ADAPT_V_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE s16_adapt_dsp_16_field_STOP_ADAPT_V_poll( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                             s16_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "s16_adapt_dsp_16_field_STOP_ADAPT_V_poll", A, value );

    /* (0x00000330 bits 0) field STOP_ADAPT_V of register PMC_ADSP_REG_STAT_INT */
    return s16_adapt_dsp_16_reg_STAT_INT_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_MSK,
                                               (value<<ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_OFF),
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
static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_STATUS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADAPT_STATUS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADAPT_STATUS_get", A, 15);
    /* (0x00000310 bits 17:1) field ADAPT_STATUS of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_STAT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK) >> ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADAPT_STATUS_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADAPT_STATUS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADAPT_STATUS_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADAPT_STATUS_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADAPT_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 16) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADAPT_STATUS_get", stop_bit, 16 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 16) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 16;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000310 bits 17:1) field ADAPT_STATUS of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_STAT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK)
                  >> ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK, ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADAPT_STATUS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_TR_FFE_B_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TR_FFE_B_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TR_FFE_B_get", A, 15);
    /* (0x00000318 bits 30:26) field TR_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TR_FFE_B_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_TR_FFE_B_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TR_FFE_B_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TR_FFE_B_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TR_FFE_B_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TR_FFE_B_get", stop_bit, 4 );
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
    /* (0x00000318 bits 30:26) field TR_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TR_FFE_B_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_TR_FFE_A_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TR_FFE_A_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TR_FFE_A_get", A, 15);
    /* (0x00000318 bits 25:21) field TR_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TR_FFE_A_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_TR_FFE_A_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TR_FFE_A_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TR_FFE_A_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TR_FFE_A_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TR_FFE_A_get", stop_bit, 4 );
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
    /* (0x00000318 bits 25:21) field TR_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TR_FFE_A_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_C_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_C_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_C_get", A, 15);
    /* (0x00000318 bits 20:14) field DP_FFE_C of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_C_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_C_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_C_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_C_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_C_get", stop_bit, 6 );
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
    /* (0x00000318 bits 20:14) field DP_FFE_C of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_C_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_B_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_B_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_B_get", A, 15);
    /* (0x00000318 bits 13:7) field DP_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_B_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_B_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_B_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_B_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_B_get", stop_bit, 6 );
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
    /* (0x00000318 bits 13:7) field DP_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_B_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_A_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DP_FFE_A_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DP_FFE_A_get", A, 15);
    /* (0x00000318 bits 6:0) field DP_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DP_FFE_A_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_A_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DP_FFE_A_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DP_FFE_A_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DP_FFE_A_get", stop_bit, 6 );
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
    /* (0x00000318 bits 6:0) field DP_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DP_FFE_A_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF2_get", A, 15);
    /* (0x0000031c bits 26:18) field DFE_COEF2 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF2_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF2_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF2_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF2_get", stop_bit, 8 );
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
    /* (0x0000031c bits 26:18) field DFE_COEF2 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF1_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF1_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF1_get", A, 15);
    /* (0x0000031c bits 17:9) field DFE_COEF1 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF1_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF1_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF1_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF1_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF1_get", stop_bit, 8 );
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
    /* (0x0000031c bits 17:9) field DFE_COEF1 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF0_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF0_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF0_get", A, 15);
    /* (0x0000031c bits 8:0) field DFE_COEF0 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF0_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF0_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF0_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF0_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF0_get", stop_bit, 8 );
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
    /* (0x0000031c bits 8:0) field DFE_COEF0 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF5_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF5_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF5_get", A, 15);
    /* (0x00000320 bits 26:18) field DFE_COEF5 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF5_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF5_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF5_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF5_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF5_get", stop_bit, 8 );
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
    /* (0x00000320 bits 26:18) field DFE_COEF5 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF5_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF4_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF4_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF4_get", A, 15);
    /* (0x00000320 bits 17:9) field DFE_COEF4 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF4_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF4_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF4_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF4_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF4_get", stop_bit, 8 );
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
    /* (0x00000320 bits 17:9) field DFE_COEF4 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF4_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF3_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_DFE_COEF3_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                           s16_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_DFE_COEF3_get", A, 15);
    /* (0x00000320 bits 8:0) field DFE_COEF3 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF3_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_DFE_COEF3_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF3_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_DFE_COEF3_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                 s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_DFE_COEF3_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_DFE_COEF3_get", stop_bit, 8 );
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
    /* (0x00000320 bits 8:0) field DFE_COEF3 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF3_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF3_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_DFE_COEF3_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_ADC_MAP_SCALE_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_ADC_MAP_SCALE_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_ADC_MAP_SCALE_get", A, 15);
    /* (0x00000324 bits 22:19) field ADC_MAP_SCALE of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_ADC_MAP_SCALE_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADC_MAP_SCALE_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_ADC_MAP_SCALE_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_ADC_MAP_SCALE_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_ADC_MAP_SCALE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_ADC_MAP_SCALE_get", stop_bit, 3 );
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
    /* (0x00000324 bits 22:19) field ADC_MAP_SCALE of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_ADC_MAP_SCALE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_ADC_MAP_SCALE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_TARGET_AMP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TARGET_AMP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TARGET_AMP_get", A, 15);
    /* (0x00000324 bits 18:13) field TARGET_AMP of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TARGET_AMP_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_TARGET_AMP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_TARGET_AMP_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_TARGET_AMP_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_TARGET_AMP_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000324 bits 18:13) field TARGET_AMP of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_TARGET_AMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_TARGET_AMP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_PGA_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PGA_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                            s16_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PGA_OFFSET_get", A, 15);
    /* (0x00000324 bits 12:5) field PGA_OFFSET of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PGA_OFFSET_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_PGA_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_PGA_OFFSET_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_PGA_OFFSET_get", stop_bit, 7 );
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
    /* (0x00000324 bits 12:5) field PGA_OFFSET of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_PGA_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_PGA_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_PGA_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                          s16_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_PGA_GAIN_get", A, 15);
    /* (0x00000324 bits 4:0) field PGA_GAIN of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_PGA_GAIN_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_PGA_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_PGA_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_PGA_GAIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_PGA_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_PGA_GAIN_get", stop_bit, 4 );
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
    /* (0x00000324 bits 4:0) field PGA_GAIN of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_4 */
    reg_value = s16_adapt_dsp_16_reg_ADAPT_RESULTS_CAPTURE_4_read( b_ptr,
                                                                   h_ptr,
                                                                   A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_4_BIT_PGA_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_PGA_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_CLIP_FLAG_ADC_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CLIP_FLAG_ADC_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CLIP_FLAG_ADC_get", A, 15);
    /* (0x00000368 bits 7:2) field CLIP_FLAG_ADC of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CLIP_FLAG_ADC_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_CLIP_FLAG_ADC_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_CLIP_FLAG_ADC_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_CLIP_FLAG_ADC_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_CLIP_FLAG_ADC_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_CLIP_FLAG_ADC_get", stop_bit, 5 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 5) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 5;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000368 bits 7:2) field CLIP_FLAG_ADC of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                                     h_ptr,
                                                                     A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK, ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_CLIP_FLAG_ADC_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_CLIP_FLAG_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_CLIP_FLAG_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                s16_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_CLIP_FLAG_GAIN_get", A, 15);
    /* (0x0000036c bits 31:0) field CLIP_FLAG_GAIN of register PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK) >> ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_CLIP_FLAG_GAIN_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_CLIP_FLAG_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_CLIP_FLAG_GAIN_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                      s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_CLIP_FLAG_GAIN_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_CLIP_FLAG_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_CLIP_FLAG_GAIN_get", stop_bit, 31 );
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
    /* (0x0000036c bits 31:0) field CLIP_FLAG_GAIN of register PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS */
    reg_value = s16_adapt_dsp_16_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK, ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_CLIP_FLAG_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_FM_CST_STAT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_FM_CST_STAT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                  s16_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TTIU_FM_CST_STAT_get", A, 15);
    /* (0x00000380 bits 30) field TTIU_FM_CST_STAT of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_TTIU_FM_CST_STAT_MSK) >> ADSP_REG_FETX_ADJ_STATUS_BIT_TTIU_FM_CST_STAT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TTIU_FM_CST_STAT_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_FM_STAT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_TTIU_FM_STAT_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_TTIU_FM_STAT_get", A, 15);
    /* (0x00000380 bits 29) field TTIU_FM_STAT of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_TTIU_FM_STAT_MSK) >> ADSP_REG_FETX_ADJ_STATUS_BIT_TTIU_FM_STAT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_TTIU_FM_STAT_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_POST_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_POST_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                               s16_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_POST_ADJ_get", A, 15);
    /* (0x00000380 bits 28:20) field FETX_POST_ADJ of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_POST_ADJ_MSK) >> ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_POST_ADJ_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_POST_ADJ_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_POST_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_POST_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                     s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_POST_ADJ_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_POST_ADJ_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_POST_ADJ_get", stop_bit, 8 );
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
    /* (0x00000380 bits 28:20) field FETX_POST_ADJ of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_POST_ADJ_MSK)
                  >> ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_POST_ADJ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_POST_ADJ_MSK, ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_POST_ADJ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_POST_ADJ_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_CTR_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_CTR_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_CTR_ADJ_get", A, 15);
    /* (0x00000380 bits 19:11) field FETX_CTR_ADJ of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_CTR_ADJ_MSK) >> ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_CTR_ADJ_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_CTR_ADJ_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_CTR_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_CTR_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_CTR_ADJ_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_CTR_ADJ_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_CTR_ADJ_get", stop_bit, 8 );
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
    /* (0x00000380 bits 19:11) field FETX_CTR_ADJ of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_CTR_ADJ_MSK)
                  >> ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_CTR_ADJ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_CTR_ADJ_MSK, ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_CTR_ADJ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_CTR_ADJ_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_PRE_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_FETX_PRE_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                              s16_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 15)
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_FETX_PRE_ADJ_get", A, 15);
    /* (0x00000380 bits 10:2) field FETX_PRE_ADJ of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_PRE_ADJ_MSK) >> ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_PRE_ADJ_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "s16_adapt_dsp_16_field_FETX_PRE_ADJ_get", A, value );

    return value;
}
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_PRE_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 s16_adapt_dsp_16_field_range_FETX_PRE_ADJ_get( s16_adapt_dsp_16_buffer_t *b_ptr,
                                                                    s16_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "s16_adapt_dsp_16_field_range_FETX_PRE_ADJ_get", A, 15);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "s16_adapt_dsp_16_field_range_FETX_PRE_ADJ_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "s16_adapt_dsp_16_field_range_FETX_PRE_ADJ_get", stop_bit, 8 );
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
    /* (0x00000380 bits 10:2) field FETX_PRE_ADJ of register PMC_ADSP_REG_FETX_ADJ_STATUS */
    reg_value = s16_adapt_dsp_16_reg_FETX_ADJ_STATUS_read( b_ptr,
                                                           h_ptr,
                                                           A);
    field_value = (reg_value & ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_PRE_ADJ_MSK)
                  >> ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_PRE_ADJ_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_PRE_ADJ_MSK, ADSP_REG_FETX_ADJ_STATUS_BIT_FETX_PRE_ADJ_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "s16_adapt_dsp_16_field_range_FETX_PRE_ADJ_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _S16_ADAPT_DSP_16_IO_INLINE_H */
