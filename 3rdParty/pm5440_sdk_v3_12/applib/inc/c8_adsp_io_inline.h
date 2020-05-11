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
 *     and register accessor functions for the c8_adsp block
 *****************************************************************************/
#ifndef _C8_ADSP_IO_INLINE_H
#define _C8_ADSP_IO_INLINE_H

#include "c8.h"
#include "c8_adsp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define C8_ADSP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for c8_adsp
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
    c8_handle_t * h_ptr;
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} c8_adsp_buffer_t;
static INLINE void c8_adsp_buffer_init( c8_adsp_buffer_t *b_ptr,
                                        c8_handle_t *h_ptr,
                                        UINT32 A ) ALWAYS_INLINE;
static INLINE void c8_adsp_buffer_init( c8_adsp_buffer_t *b_ptr,
                                        c8_handle_t *h_ptr,
                                        UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x01000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "c8_adsp_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void c8_adsp_buffer_flush( c8_adsp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void c8_adsp_buffer_flush( c8_adsp_buffer_t *b_ptr )
{
    IOLOG( "c8_adsp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 c8_adsp_reg_read( c8_adsp_buffer_t *b_ptr,
                                       c8_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_read( c8_adsp_buffer_t *b_ptr,
                                       c8_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x01000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x01000) + reg);
    return value;
}
static INLINE void c8_adsp_reg_write( c8_adsp_buffer_t *b_ptr,
                                      c8_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_write( c8_adsp_buffer_t *b_ptr,
                                      c8_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x01000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x01000) + reg,
                         value);
}

static INLINE void c8_adsp_field_set( c8_adsp_buffer_t *b_ptr,
                                      c8_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_set( c8_adsp_buffer_t *b_ptr,
                                      c8_handle_t *h_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x01000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x01000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x01000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void c8_adsp_action_on_write_field_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_action_on_write_field_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x01000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x01000) + reg,
                         value<<ofs);
}

static INLINE void c8_adsp_burst_read( c8_adsp_buffer_t *b_ptr,
                                       c8_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void c8_adsp_burst_read( c8_adsp_buffer_t *b_ptr,
                                       c8_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x01000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x01000) + reg,
                          len,
                          value);
}

static INLINE void c8_adsp_burst_write( c8_adsp_buffer_t *b_ptr,
                                        c8_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void c8_adsp_burst_write( c8_adsp_buffer_t *b_ptr,
                                        c8_handle_t *h_ptr,
                                        UINT32 A,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x01000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x01000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE c8_adsp_poll( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_poll( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x01000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x01000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for c8_adsp
 * ==================================================================================
 */

static INLINE void c8_adsp_reg_ADAPT_DSP_CLK_CTRL_write( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_DSP_CLK_CTRL_write( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_ADAPT_DSP_CLK_CTRL_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL,
                       value);
}

static INLINE void c8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL,
                       mask,
                       PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_ADAPT_DSP_CLK_CTRL_read( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ADAPT_DSP_CLK_CTRL_read( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ADAPT_DSP_CLK_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_ADAPT_CTRL_1_write( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_CTRL_1_write( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_ADAPT_CTRL_1_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_CTRL_1,
                       value);
}

static INLINE void c8_adsp_reg_ADAPT_CTRL_1_field_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_CTRL_1_field_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_ADAPT_CTRL_1_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_CTRL_1,
                       mask,
                       PMC_ADSP_REG_ADAPT_CTRL_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_ADAPT_CTRL_1_read( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ADAPT_CTRL_1_read( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_ADAPT_CTRL_1);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ADAPT_CTRL_1_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_ADAPT_CTRL_2_write( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_CTRL_2_write( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_ADAPT_CTRL_2_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_CTRL_2,
                       value);
}

static INLINE void c8_adsp_reg_ADAPT_CTRL_2_field_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_CTRL_2_field_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_ADAPT_CTRL_2_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ADAPT_CTRL_2,
                       mask,
                       PMC_ADSP_REG_ADAPT_CTRL_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_ADAPT_CTRL_2_read( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ADAPT_CTRL_2_read( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_ADAPT_CTRL_2);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ADAPT_CTRL_2_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_DSS_CTRL_write( c8_adsp_buffer_t *b_ptr,
                                               c8_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_DSS_CTRL_write( c8_adsp_buffer_t *b_ptr,
                                               c8_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_DSS_CTRL_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_CTRL,
                       value);
}

static INLINE void c8_adsp_reg_DSS_CTRL_field_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_DSS_CTRL_field_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_DSS_CTRL_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_CTRL,
                       mask,
                       PMC_ADSP_REG_DSS_CTRL_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_DSS_CTRL_read( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_DSS_CTRL_read( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_DSS_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_DSS_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_DSS_TAPS_1_write( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_DSS_TAPS_1_write( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_DSS_TAPS_1_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_1,
                       value);
}

static INLINE void c8_adsp_reg_DSS_TAPS_1_field_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_DSS_TAPS_1_field_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_DSS_TAPS_1_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_1,
                       mask,
                       PMC_ADSP_REG_DSS_TAPS_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_DSS_TAPS_1_read( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_DSS_TAPS_1_read( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_DSS_TAPS_1);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_DSS_TAPS_1_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_DSS_TAPS_2_write( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_DSS_TAPS_2_write( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_DSS_TAPS_2_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_2,
                       value);
}

static INLINE void c8_adsp_reg_DSS_TAPS_2_field_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_DSS_TAPS_2_field_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_DSS_TAPS_2_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_DSS_TAPS_2,
                       mask,
                       PMC_ADSP_REG_DSS_TAPS_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_DSS_TAPS_2_read( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_DSS_TAPS_2_read( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_DSS_TAPS_2);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_DSS_TAPS_2_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_LMS_MU_FACTORS_write( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_LMS_MU_FACTORS_write( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_LMS_MU_FACTORS_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_LMS_MU_FACTORS,
                       value);
}

static INLINE void c8_adsp_reg_LMS_MU_FACTORS_field_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_LMS_MU_FACTORS_field_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_LMS_MU_FACTORS_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_LMS_MU_FACTORS,
                       mask,
                       PMC_ADSP_REG_LMS_MU_FACTORS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_LMS_MU_FACTORS_read( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_LMS_MU_FACTORS_read( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_LMS_MU_FACTORS);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_LMS_MU_FACTORS_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_ROUND_ROBIN_PERIODS_write( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ROUND_ROBIN_PERIODS_write( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_ROUND_ROBIN_PERIODS_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ROUND_ROBIN_PERIODS,
                       value);
}

static INLINE void c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_ROUND_ROBIN_PERIODS,
                       mask,
                       PMC_ADSP_REG_ROUND_ROBIN_PERIODS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_ROUND_ROBIN_PERIODS_read( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ROUND_ROBIN_PERIODS_read( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_ROUND_ROBIN_PERIODS);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ROUND_ROBIN_PERIODS_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_1_write( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_1_write( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_OBJECT_PRELOAD_VAL_1_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1,
                       value);
}

static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1,
                       mask,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_2_write( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_2_write( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_OBJECT_PRELOAD_VAL_2_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2,
                       value);
}

static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2,
                       mask,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_3_write( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_3_write( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_OBJECT_PRELOAD_VAL_3_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3,
                       value);
}

static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3,
                       mask,
                       PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_CLIP_RATE_MON_PARAMS_write( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_CLIP_RATE_MON_PARAMS_write( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_CLIP_RATE_MON_PARAMS_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_CLIP_RATE_MON_PARAMS,
                       value);
}

static INLINE void c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_CONFIG,
                       PMC_ADSP_REG_CLIP_RATE_MON_PARAMS,
                       mask,
                       PMC_ADSP_REG_CLIP_RATE_MON_PARAMS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_CONFIG,
                                  PMC_ADSP_REG_CLIP_RATE_MON_PARAMS);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_CLIP_RATE_MON_PARAMS_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_ADAPT_STAT_write( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_STAT_write( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_ADAPT_STAT_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_ADAPT_STAT,
                       value);
}

static INLINE void c8_adsp_reg_ADAPT_STAT_field_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_STAT_field_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_ADAPT_STAT_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_ADAPT_STAT,
                       mask,
                       PMC_ADSP_REG_ADAPT_STAT_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_ADAPT_STAT_read( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ADAPT_STAT_read( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ADAPT_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST,
                       value);
}

static INLINE void c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( c8_adsp_buffer_t *b_ptr,
                                                                        c8_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( c8_adsp_buffer_t *b_ptr,
                                                                        c8_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST,
                       mask,
                       PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( c8_adsp_buffer_t *b_ptr,
                                                                     c8_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( c8_adsp_buffer_t *b_ptr,
                                                                     c8_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_write( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_write( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS,
                       value);
}

static INLINE void c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS,
                       mask,
                       PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( c8_adsp_buffer_t *b_ptr,
                                                                 c8_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( c8_adsp_buffer_t *b_ptr,
                                                                 c8_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read", reg_value, A);
    return reg_value;
}

static INLINE void c8_adsp_reg_STAT_INT_EVENT_write( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_STAT_INT_EVENT_write( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_STAT_INT_EVENT_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_STAT_INT_EVENT,
                       value);
}

static INLINE void c8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( c8_adsp_buffer_t *b_ptr,
                                                                         c8_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( c8_adsp_buffer_t *b_ptr,
                                                                         c8_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set", A, mask, ofs, value );
    c8_adsp_action_on_write_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_ADSP_REG_STAT_INT_EVENT,
                                       mask,
                                       ofs,
                                       value);

}

static INLINE UINT32 c8_adsp_reg_STAT_INT_EVENT_read( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_STAT_INT_EVENT_read( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_STAT_INT_EVENT);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_STAT_INT_EVENT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_reg_STAT_INT_EVENT_poll( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_reg_STAT_INT_EVENT_poll( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "c8_adsp_reg_STAT_INT_EVENT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return c8_adsp_poll( b_ptr,
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


static INLINE void c8_adsp_reg_STAT_INT_EN_write( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_STAT_INT_EN_write( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "c8_adsp_reg_STAT_INT_EN_write", value );
    c8_adsp_reg_write( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_STAT_INT_EN,
                       value);
}

static INLINE void c8_adsp_reg_STAT_INT_EN_field_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_reg_STAT_INT_EN_field_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "c8_adsp_reg_STAT_INT_EN_field_set", A, mask, ofs, value );
    c8_adsp_field_set( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_ADSP_REG_STAT_INT_EN,
                       mask,
                       PMC_ADSP_REG_STAT_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 c8_adsp_reg_STAT_INT_EN_read( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_STAT_INT_EN_read( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_STAT_INT_EN);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_STAT_INT_EN_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 c8_adsp_reg_STAT_INT_read( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_STAT_INT_read( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_STAT_INT);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_STAT_INT_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_reg_STAT_INT_poll( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_reg_STAT_INT_poll( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "c8_adsp_reg_STAT_INT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return c8_adsp_poll( b_ptr,
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


static INLINE UINT32 c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 c8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( c8_adsp_buffer_t *b_ptr,
                                                                      c8_handle_t *h_ptr,
                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( c8_adsp_buffer_t *b_ptr,
                                                                      c8_handle_t *h_ptr,
                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = c8_adsp_reg_read( b_ptr,
                                  h_ptr,
                                  A,
                                  MEM_TYPE_STATUS,
                                  PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS);

    IOLOG( "%s -> 0x%08x; A=%d", "c8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void c8_adsp_field_ADAPT_CLK_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_ADAPT_CLK_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_CLK_ENB_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_ADAPT_CLK_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_ADAPT_CLK_ENB_set", A, value );

    /* (0x00000800 bits 0) field ADAPT_CLK_ENB of register PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL */
    c8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_MSK,
                                              ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_OFF,
                                              value);
}

static INLINE UINT32 c8_adsp_field_ADAPT_CLK_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADAPT_CLK_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_CLK_ENB_get", A, 7);
    /* (0x00000800 bits 0) field ADAPT_CLK_ENB of register PMC_ADSP_REG_ADAPT_DSP_CLK_CTRL */
    reg_value = c8_adsp_reg_ADAPT_DSP_CLK_CTRL_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_MSK) >> ADSP_REG_ADAPT_DSP_CLK_CTRL_BIT_ADAPT_CLK_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADAPT_CLK_ENB_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_EQ_ADAPT_CTRL_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_EQ_ADAPT_CTRL_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_EQ_ADAPT_CTRL_ENB_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_EQ_ADAPT_CTRL_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_EQ_ADAPT_CTRL_ENB_set", A, value );

    /* (0x00000804 bits 14) field EQ_ADAPT_CTRL_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_EQ_ADAPT_CTRL_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_EQ_ADAPT_CTRL_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_EQ_ADAPT_CTRL_ENB_get", A, 7);
    /* (0x00000804 bits 14) field EQ_ADAPT_CTRL_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_EQ_ADAPT_CTRL_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_EQ_ADAPT_CTRL_ENB_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_PCIE_FFE_MODE_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PCIE_FFE_MODE_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PCIE_FFE_MODE_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PCIE_FFE_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PCIE_FFE_MODE_set", A, value );

    /* (0x00000804 bits 13) field PCIE_FFE_MODE of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_PCIE_FFE_MODE_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_PCIE_FFE_MODE_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_PCIE_FFE_MODE_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PCIE_FFE_MODE_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PCIE_FFE_MODE_get", A, 7);
    /* (0x00000804 bits 13) field PCIE_FFE_MODE of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_PCIE_FFE_MODE_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_PCIE_FFE_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PCIE_FFE_MODE_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_INV_DFE_OUT_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_INV_DFE_OUT_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_INV_DFE_OUT_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_INV_DFE_OUT_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_INV_DFE_OUT_set", A, value );

    /* (0x00000804 bits 12) field INV_DFE_OUT of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_INV_DFE_OUT_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_INV_DFE_OUT_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_INV_DFE_OUT_get", A, 7);
    /* (0x00000804 bits 12) field INV_DFE_OUT of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_INV_DFE_OUT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_INV_DFE_OUT_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_ADAPT_INITB_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_ADAPT_INITB_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_INITB_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_ADAPT_INITB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_ADAPT_INITB_set", A, value );

    /* (0x00000804 bits 11) field ADAPT_INITB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_ADAPT_INITB_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADAPT_INITB_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_INITB_get", A, 7);
    /* (0x00000804 bits 11) field ADAPT_INITB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_INITB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADAPT_INITB_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_DP_ADAPT_ORDER_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DP_ADAPT_ORDER_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_ADAPT_ORDER_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DP_ADAPT_ORDER_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DP_ADAPT_ORDER_set", A, value );

    /* (0x00000804 bits 10) field DP_ADAPT_ORDER of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_DP_ADAPT_ORDER_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_DP_ADAPT_ORDER_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_DP_ADAPT_ORDER_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_ADAPT_ORDER_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_ADAPT_ORDER_get", A, 7);
    /* (0x00000804 bits 10) field DP_ADAPT_ORDER of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_DP_ADAPT_ORDER_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_DP_ADAPT_ORDER_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_ADAPT_ORDER_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_FFE_CONSTRAINED_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_FFE_CONSTRAINED_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_FFE_CONSTRAINED_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_FFE_CONSTRAINED_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_FFE_CONSTRAINED_set", A, value );

    /* (0x00000804 bits 9) field FFE_CONSTRAINED of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_FFE_CONSTRAINED_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_FFE_CONSTRAINED_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_FFE_CONSTRAINED_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_FFE_CONSTRAINED_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_FFE_CONSTRAINED_get", A, 7);
    /* (0x00000804 bits 9) field FFE_CONSTRAINED of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_FFE_CONSTRAINED_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_FFE_CONSTRAINED_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_FFE_CONSTRAINED_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_PRELOAD_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PRELOAD_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PRELOAD_ENB_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PRELOAD_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PRELOAD_ENB_set", A, value );

    /* (0x00000804 bits 8) field PRELOAD_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_PRELOAD_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PRELOAD_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PRELOAD_ENB_get", A, 7);
    /* (0x00000804 bits 8) field PRELOAD_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_PRELOAD_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PRELOAD_ENB_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_ADAPT_DELAY_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_ADAPT_DELAY_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_DELAY_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_ADAPT_DELAY_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_ADAPT_DELAY_set", A, value );

    /* (0x00000804 bits 7:5) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_ADAPT_DELAY_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADAPT_DELAY_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_DELAY_get", A, 7);
    /* (0x00000804 bits 7:5) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADAPT_DELAY_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_ADAPT_DELAY_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_ADAPT_DELAY_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADAPT_DELAY_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADAPT_DELAY_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADAPT_DELAY_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADAPT_DELAY_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000804 bits 7:5) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
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
        /* (0x00000804 bits 7:5) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
        c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_ADAPT_DELAY_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_ADAPT_DELAY_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADAPT_DELAY_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADAPT_DELAY_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADAPT_DELAY_get", stop_bit, 2 );
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
    /* (0x00000804 bits 7:5) field ADAPT_DELAY of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK)
                  >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_MSK, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_DELAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADAPT_DELAY_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_ADAPT_LOOPS_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_ADAPT_LOOPS_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_LOOPS_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_ADAPT_LOOPS_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_ADAPT_LOOPS_set", A, value );

    /* (0x00000804 bits 4:1) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_ADAPT_LOOPS_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADAPT_LOOPS_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_LOOPS_get", A, 7);
    /* (0x00000804 bits 4:1) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADAPT_LOOPS_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_ADAPT_LOOPS_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_ADAPT_LOOPS_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADAPT_LOOPS_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADAPT_LOOPS_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADAPT_LOOPS_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADAPT_LOOPS_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000804 bits 4:1) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
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
        /* (0x00000804 bits 4:1) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
        c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_ADAPT_LOOPS_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_ADAPT_LOOPS_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADAPT_LOOPS_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADAPT_LOOPS_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADAPT_LOOPS_get", stop_bit, 3 );
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
    /* (0x00000804 bits 4:1) field ADAPT_LOOPS of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK)
                  >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_MSK, ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_LOOPS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADAPT_LOOPS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_ADAPT_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_ADAPT_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_ENB_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_ADAPT_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_ADAPT_ENB_set", A, value );

    /* (0x00000804 bits 0) field ADAPT_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    c8_adsp_reg_ADAPT_CTRL_1_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_MSK,
                                        ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_ADAPT_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADAPT_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_ENB_get", A, 7);
    /* (0x00000804 bits 0) field ADAPT_ENB of register PMC_ADSP_REG_ADAPT_CTRL_1 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_MSK) >> ADSP_REG_ADAPT_CTRL_1_BIT_ADAPT_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADAPT_ENB_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_OBJECTS_EN_set( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_OBJECTS_EN_set( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_OBJECTS_EN_set", A, 7);
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_OBJECTS_EN_set", value, 16383);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_OBJECTS_EN_set", A, value );

    /* (0x00000808 bits 31:18) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    c8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_MSK,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_OBJECTS_EN_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_OBJECTS_EN_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_OBJECTS_EN_get", A, 7);
    /* (0x00000808 bits 31:18) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_MSK) >> ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_OBJECTS_EN_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_OBJECTS_EN_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_OBJECTS_EN_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_OBJECTS_EN_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_OBJECTS_EN_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_OBJECTS_EN_set", stop_bit, 13 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_OBJECTS_EN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000808 bits 31:18) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
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
        /* (0x00000808 bits 31:18) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
        c8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_OBJECTS_EN_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_OBJECTS_EN_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_OBJECTS_EN_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_OBJECTS_EN_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_OBJECTS_EN_get", stop_bit, 13 );
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
    /* (0x00000808 bits 31:18) field OBJECTS_EN of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_MSK)
                  >> ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_MSK, ADSP_REG_ADAPT_CTRL_2_BIT_OBJECTS_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_OBJECTS_EN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_LMS_DATA_SEL_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_LMS_DATA_SEL_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_LMS_DATA_SEL_set", A, 7);
    if (value > 16383)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_LMS_DATA_SEL_set", value, 16383);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_LMS_DATA_SEL_set", A, value );

    /* (0x00000808 bits 17:4) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    c8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_LMS_DATA_SEL_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_LMS_DATA_SEL_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_LMS_DATA_SEL_get", A, 7);
    /* (0x00000808 bits 17:4) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK) >> ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_LMS_DATA_SEL_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_LMS_DATA_SEL_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_LMS_DATA_SEL_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_LMS_DATA_SEL_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_LMS_DATA_SEL_set", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_LMS_DATA_SEL_set", stop_bit, 13 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_LMS_DATA_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000808 bits 17:4) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
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
        /* (0x00000808 bits 17:4) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
        c8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_LMS_DATA_SEL_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_LMS_DATA_SEL_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_LMS_DATA_SEL_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_LMS_DATA_SEL_get", stop_bit, start_bit );
    if (stop_bit > 13) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_LMS_DATA_SEL_get", stop_bit, 13 );
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
    /* (0x00000808 bits 17:4) field LMS_DATA_SEL of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK)
                  >> ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_MSK, ADSP_REG_ADAPT_CTRL_2_BIT_LMS_DATA_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_LMS_DATA_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_SEL_LMS_CLIP_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_SEL_LMS_CLIP_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_SEL_LMS_CLIP_set", A, 7);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_SEL_LMS_CLIP_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_SEL_LMS_CLIP_set", A, value );

    /* (0x00000808 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    c8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_MSK,
                                        ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_OFF,
                                        value);
}

static INLINE UINT32 c8_adsp_field_SEL_LMS_CLIP_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_SEL_LMS_CLIP_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_SEL_LMS_CLIP_get", A, 7);
    /* (0x00000808 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_MSK) >> ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_SEL_LMS_CLIP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_SEL_LMS_CLIP_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_SEL_LMS_CLIP_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_SEL_LMS_CLIP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_SEL_LMS_CLIP_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_SEL_LMS_CLIP_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_SEL_LMS_CLIP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000808 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_2 */
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
        /* (0x00000808 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_2 */
        c8_adsp_reg_ADAPT_CTRL_2_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_OFF + subfield_offset),
                                            ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_SEL_LMS_CLIP_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_SEL_LMS_CLIP_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_SEL_LMS_CLIP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_SEL_LMS_CLIP_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_SEL_LMS_CLIP_get", stop_bit, 1 );
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
    /* (0x00000808 bits 1:0) field SEL_LMS_CLIP of register PMC_ADSP_REG_ADAPT_CTRL_2 */
    reg_value = c8_adsp_reg_ADAPT_CTRL_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_MSK)
                  >> ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_MSK, ADSP_REG_ADAPT_CTRL_2_BIT_SEL_LMS_CLIP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_SEL_LMS_CLIP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_EXPECTED_EQ2_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_EXPECTED_EQ2_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_EXPECTED_EQ2_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_EXPECTED_EQ2_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_EXPECTED_EQ2_set", A, value );

    /* (0x0000082c bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF,
                                    value);
}

static INLINE UINT32 c8_adsp_field_EXPECTED_EQ2_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_EXPECTED_EQ2_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_EXPECTED_EQ2_get", A, 7);
    /* (0x0000082c bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK) >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_EXPECTED_EQ2_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_EXPECTED_EQ2_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_EXPECTED_EQ2_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_EXPECTED_EQ2_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_EXPECTED_EQ2_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_EXPECTED_EQ2_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_EXPECTED_EQ2_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000082c bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x0000082c bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
        c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_EXPECTED_EQ2_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_EXPECTED_EQ2_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_EXPECTED_EQ2_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_EXPECTED_EQ2_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_EXPECTED_EQ2_get", stop_bit, 2 );
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
    /* (0x0000082c bits 29:27) field EXPECTED_EQ2 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_MSK, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_EXPECTED_EQ2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_EXPECTED_EQ1_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_EXPECTED_EQ1_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_EXPECTED_EQ1_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_EXPECTED_EQ1_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_EXPECTED_EQ1_set", A, value );

    /* (0x0000082c bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF,
                                    value);
}

static INLINE UINT32 c8_adsp_field_EXPECTED_EQ1_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_EXPECTED_EQ1_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_EXPECTED_EQ1_get", A, 7);
    /* (0x0000082c bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK) >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_EXPECTED_EQ1_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_EXPECTED_EQ1_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_EXPECTED_EQ1_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_EXPECTED_EQ1_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_EXPECTED_EQ1_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_EXPECTED_EQ1_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_EXPECTED_EQ1_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000082c bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x0000082c bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
        c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_EXPECTED_EQ1_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_EXPECTED_EQ1_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_EXPECTED_EQ1_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_EXPECTED_EQ1_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_EXPECTED_EQ1_get", stop_bit, 2 );
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
    /* (0x0000082c bits 26:24) field EXPECTED_EQ1 of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_MSK, ADSP_REG_DSS_CTRL_BIT_EXPECTED_EQ1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_EXPECTED_EQ1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_H_LIMIT_set( c8_adsp_buffer_t *b_ptr,
                                              c8_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_H_LIMIT_set( c8_adsp_buffer_t *b_ptr,
                                              c8_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_H_LIMIT_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_H_LIMIT_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_H_LIMIT_set", A, value );

    /* (0x0000082c bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF,
                                    value);
}

static INLINE UINT32 c8_adsp_field_H_LIMIT_get( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_H_LIMIT_get( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_H_LIMIT_get", A, 7);
    /* (0x0000082c bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK) >> ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_H_LIMIT_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_H_LIMIT_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_H_LIMIT_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_H_LIMIT_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_H_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_H_LIMIT_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_H_LIMIT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000082c bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x0000082c bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
        c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_H_LIMIT_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_H_LIMIT_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_H_LIMIT_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_H_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_H_LIMIT_get", stop_bit, 6 );
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
    /* (0x0000082c bits 22:16) field H_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_H_LIMIT_MSK, ADSP_REG_DSS_CTRL_BIT_H_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_H_LIMIT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_S_LIMIT_set( c8_adsp_buffer_t *b_ptr,
                                              c8_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_S_LIMIT_set( c8_adsp_buffer_t *b_ptr,
                                              c8_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_S_LIMIT_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_S_LIMIT_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_S_LIMIT_set", A, value );

    /* (0x0000082c bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF,
                                    value);
}

static INLINE UINT32 c8_adsp_field_S_LIMIT_get( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_S_LIMIT_get( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_S_LIMIT_get", A, 7);
    /* (0x0000082c bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK) >> ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_S_LIMIT_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_S_LIMIT_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_S_LIMIT_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_S_LIMIT_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_S_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_S_LIMIT_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_S_LIMIT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000082c bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x0000082c bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
        c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_S_LIMIT_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_S_LIMIT_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_S_LIMIT_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_S_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_S_LIMIT_get", stop_bit, 6 );
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
    /* (0x0000082c bits 15:9) field S_LIMIT of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_S_LIMIT_MSK, ADSP_REG_DSS_CTRL_BIT_S_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_S_LIMIT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_OFL_DECAY_set( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_OFL_DECAY_set( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_OFL_DECAY_set", A, 7);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_OFL_DECAY_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_OFL_DECAY_set", A, value );

    /* (0x0000082c bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF,
                                    value);
}

static INLINE UINT32 c8_adsp_field_OFL_DECAY_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_OFL_DECAY_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_OFL_DECAY_get", A, 7);
    /* (0x0000082c bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK) >> ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_OFL_DECAY_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_OFL_DECAY_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_OFL_DECAY_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_OFL_DECAY_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_OFL_DECAY_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_OFL_DECAY_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_OFL_DECAY_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000082c bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
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
        /* (0x0000082c bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
        c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        subfield_mask << (ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF + subfield_offset),
                                        ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF + subfield_offset,
                                        value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_OFL_DECAY_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_OFL_DECAY_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_OFL_DECAY_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_OFL_DECAY_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_OFL_DECAY_get", stop_bit, 4 );
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
    /* (0x0000082c bits 7:3) field OFL_DECAY of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK)
                  >> ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_MSK, ADSP_REG_DSS_CTRL_BIT_OFL_DECAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_OFL_DECAY_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_PATH_PATTERN_CHK_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PATH_PATTERN_CHK_ENB_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PATH_PATTERN_CHK_ENB_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PATH_PATTERN_CHK_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PATH_PATTERN_CHK_ENB_set", A, value );

    /* (0x0000082c bits 1) field PATH_PATTERN_CHK_ENB of register PMC_ADSP_REG_DSS_CTRL */
    c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_OFF,
                                    value);
}

static INLINE UINT32 c8_adsp_field_PATH_PATTERN_CHK_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PATH_PATTERN_CHK_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PATH_PATTERN_CHK_ENB_get", A, 7);
    /* (0x0000082c bits 1) field PATH_PATTERN_CHK_ENB of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_MSK) >> ADSP_REG_DSS_CTRL_BIT_PATH_PATTERN_CHK_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PATH_PATTERN_CHK_ENB_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_DSS_ENB_set( c8_adsp_buffer_t *b_ptr,
                                              c8_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DSS_ENB_set( c8_adsp_buffer_t *b_ptr,
                                              c8_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DSS_ENB_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DSS_ENB_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DSS_ENB_set", A, value );

    /* (0x0000082c bits 0) field DSS_ENB of register PMC_ADSP_REG_DSS_CTRL */
    c8_adsp_reg_DSS_CTRL_field_set( b_ptr,
                                    h_ptr,
                                    A,
                                    ADSP_REG_DSS_CTRL_BIT_DSS_ENB_MSK,
                                    ADSP_REG_DSS_CTRL_BIT_DSS_ENB_OFF,
                                    value);
}

static INLINE UINT32 c8_adsp_field_DSS_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DSS_ENB_get( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DSS_ENB_get", A, 7);
    /* (0x0000082c bits 0) field DSS_ENB of register PMC_ADSP_REG_DSS_CTRL */
    reg_value = c8_adsp_reg_DSS_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_CTRL_BIT_DSS_ENB_MSK) >> ADSP_REG_DSS_CTRL_BIT_DSS_ENB_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DSS_ENB_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF4_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF4_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF4_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF4_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF4_TAP_set", A, value );

    /* (0x00000830 bits 23:20) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF4_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF4_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF4_TAP_get", A, 7);
    /* (0x00000830 bits 23:20) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF4_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF4_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF4_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF4_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF4_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF4_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF4_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000830 bits 23:20) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000830 bits 23:20) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF4_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF4_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF4_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF4_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF4_TAP_get", stop_bit, 3 );
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
    /* (0x00000830 bits 23:20) field DFE_COEF4_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF4_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF4_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF3_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF3_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF3_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF3_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF3_TAP_set", A, value );

    /* (0x00000830 bits 19:16) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF3_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF3_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF3_TAP_get", A, 7);
    /* (0x00000830 bits 19:16) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF3_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF3_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF3_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF3_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF3_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF3_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF3_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000830 bits 19:16) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000830 bits 19:16) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF3_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF3_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF3_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF3_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF3_TAP_get", stop_bit, 3 );
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
    /* (0x00000830 bits 19:16) field DFE_COEF3_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF3_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF3_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF2_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF2_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF2_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF2_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF2_TAP_set", A, value );

    /* (0x00000830 bits 15:12) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF2_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF2_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF2_TAP_get", A, 7);
    /* (0x00000830 bits 15:12) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF2_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF2_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF2_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF2_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF2_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF2_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF2_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000830 bits 15:12) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000830 bits 15:12) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF2_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF2_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF2_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF2_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF2_TAP_get", stop_bit, 3 );
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
    /* (0x00000830 bits 15:12) field DFE_COEF2_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF2_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF2_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF1_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF1_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF1_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF1_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF1_TAP_set", A, value );

    /* (0x00000830 bits 11:8) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF1_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF1_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF1_TAP_get", A, 7);
    /* (0x00000830 bits 11:8) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF1_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF1_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF1_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF1_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF1_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF1_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF1_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000830 bits 11:8) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000830 bits 11:8) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF1_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF1_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF1_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF1_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF1_TAP_get", stop_bit, 3 );
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
    /* (0x00000830 bits 11:8) field DFE_COEF1_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF1_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF1_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF0_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF0_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF0_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF0_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF0_TAP_set", A, value );

    /* (0x00000830 bits 7:4) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF0_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF0_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF0_TAP_get", A, 7);
    /* (0x00000830 bits 7:4) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF0_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF0_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF0_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF0_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF0_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF0_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF0_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000830 bits 7:4) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000830 bits 7:4) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF0_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF0_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF0_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF0_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF0_TAP_get", stop_bit, 3 );
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
    /* (0x00000830 bits 7:4) field DFE_COEF0_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_DFE_COEF0_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF0_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_GAIN_TAP_set( c8_adsp_buffer_t *b_ptr,
                                               c8_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_GAIN_TAP_set( c8_adsp_buffer_t *b_ptr,
                                               c8_handle_t *h_ptr,
                                               UINT32 A,
                                               UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_GAIN_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_GAIN_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_GAIN_TAP_set", A, value );

    /* (0x00000830 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_GAIN_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_GAIN_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_GAIN_TAP_get", A, 7);
    /* (0x00000830 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_MSK) >> ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_GAIN_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_GAIN_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_GAIN_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_GAIN_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_GAIN_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_GAIN_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_GAIN_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000830 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
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
        /* (0x00000830 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
        c8_adsp_reg_DSS_TAPS_1_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_GAIN_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_GAIN_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_GAIN_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_GAIN_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_GAIN_TAP_get", stop_bit, 3 );
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
    /* (0x00000830 bits 3:0) field GAIN_TAP of register PMC_ADSP_REG_DSS_TAPS_1 */
    reg_value = c8_adsp_reg_DSS_TAPS_1_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_MSK, ADSP_REG_DSS_TAPS_1_BIT_GAIN_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_GAIN_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DP_FFE_C_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DP_FFE_C_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_C_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DP_FFE_C_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DP_FFE_C_TAP_set", A, value );

    /* (0x00000834 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_DP_FFE_C_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_C_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_C_TAP_get", A, 7);
    /* (0x00000834 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_MSK) >> ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_C_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DP_FFE_C_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DP_FFE_C_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_C_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_C_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_C_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_C_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000834 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
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
        /* (0x00000834 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
        c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DP_FFE_C_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_C_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_C_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_C_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_C_TAP_get", stop_bit, 3 );
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
    /* (0x00000834 bits 23:20) field DP_FFE_C_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_MSK, ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_C_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_C_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DP_FFE_B_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DP_FFE_B_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_B_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DP_FFE_B_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DP_FFE_B_TAP_set", A, value );

    /* (0x00000834 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_DP_FFE_B_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_B_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_B_TAP_get", A, 7);
    /* (0x00000834 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_MSK) >> ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_B_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DP_FFE_B_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DP_FFE_B_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_B_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_B_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_B_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_B_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000834 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
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
        /* (0x00000834 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
        c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DP_FFE_B_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_B_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_B_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_B_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_B_TAP_get", stop_bit, 3 );
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
    /* (0x00000834 bits 19:16) field DP_FFE_B_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_MSK, ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_B_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_B_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DP_FFE_A_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DP_FFE_A_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_A_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DP_FFE_A_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DP_FFE_A_TAP_set", A, value );

    /* (0x00000834 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_DP_FFE_A_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_A_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_A_TAP_get", A, 7);
    /* (0x00000834 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_MSK) >> ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_A_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DP_FFE_A_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DP_FFE_A_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_A_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_A_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_A_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_A_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000834 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
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
        /* (0x00000834 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
        c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DP_FFE_A_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_A_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_A_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_A_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_A_TAP_get", stop_bit, 3 );
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
    /* (0x00000834 bits 15:12) field DP_FFE_A_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_MSK, ADSP_REG_DSS_TAPS_2_BIT_DP_FFE_A_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_A_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_TARGET_AMP_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_TARGET_AMP_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TARGET_AMP_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_TARGET_AMP_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_TARGET_AMP_TAP_set", A, value );

    /* (0x00000834 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_TARGET_AMP_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_TARGET_AMP_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TARGET_AMP_TAP_get", A, 7);
    /* (0x00000834 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_MSK) >> ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_TARGET_AMP_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_TARGET_AMP_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_TARGET_AMP_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TARGET_AMP_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TARGET_AMP_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TARGET_AMP_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TARGET_AMP_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000834 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
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
        /* (0x00000834 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
        c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_TARGET_AMP_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_TARGET_AMP_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TARGET_AMP_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TARGET_AMP_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TARGET_AMP_TAP_get", stop_bit, 3 );
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
    /* (0x00000834 bits 11:8) field TARGET_AMP_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_MSK, ADSP_REG_DSS_TAPS_2_BIT_TARGET_AMP_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TARGET_AMP_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_ADC_TAP_set( c8_adsp_buffer_t *b_ptr,
                                              c8_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_ADC_TAP_set( c8_adsp_buffer_t *b_ptr,
                                              c8_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADC_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_ADC_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_ADC_TAP_set", A, value );

    /* (0x00000834 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_ADC_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADC_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                c8_handle_t *h_ptr,
                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADC_TAP_get", A, 7);
    /* (0x00000834 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_MSK) >> ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADC_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_ADC_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_ADC_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADC_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADC_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADC_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADC_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000834 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
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
        /* (0x00000834 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
        c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_ADC_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_ADC_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADC_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADC_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADC_TAP_get", stop_bit, 3 );
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
    /* (0x00000834 bits 7:4) field ADC_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_MSK, ADSP_REG_DSS_TAPS_2_BIT_ADC_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADC_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_OFFSET_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_OFFSET_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_OFFSET_TAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_OFFSET_TAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_OFFSET_TAP_set", A, value );

    /* (0x00000834 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_MSK,
                                      ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_OFFSET_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_OFFSET_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_OFFSET_TAP_get", A, 7);
    /* (0x00000834 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_MSK) >> ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_OFFSET_TAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_OFFSET_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_OFFSET_TAP_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_OFFSET_TAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_OFFSET_TAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_OFFSET_TAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_OFFSET_TAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000834 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
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
        /* (0x00000834 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
        c8_adsp_reg_DSS_TAPS_2_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          subfield_mask << (ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_OFF + subfield_offset),
                                          ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_OFFSET_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_OFFSET_TAP_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_OFFSET_TAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_OFFSET_TAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_OFFSET_TAP_get", stop_bit, 3 );
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
    /* (0x00000834 bits 3:0) field OFFSET_TAP of register PMC_ADSP_REG_DSS_TAPS_2 */
    reg_value = c8_adsp_reg_DSS_TAPS_2_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_MSK)
                  >> ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_MSK, ADSP_REG_DSS_TAPS_2_BIT_OFFSET_TAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_OFFSET_TAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_MU_LEAK_ADC_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_MU_LEAK_ADC_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_LEAK_ADC_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_MU_LEAK_ADC_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_MU_LEAK_ADC_set", A, value );

    /* (0x00000838 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF,
                                          value);
}

static INLINE UINT32 c8_adsp_field_MU_LEAK_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_MU_LEAK_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_LEAK_ADC_get", A, 7);
    /* (0x00000838 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_MU_LEAK_ADC_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_MU_LEAK_ADC_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_MU_LEAK_ADC_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_LEAK_ADC_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_LEAK_ADC_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_LEAK_ADC_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_LEAK_ADC_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000838 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000838 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
        c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_MU_LEAK_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_MU_LEAK_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_LEAK_ADC_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_LEAK_ADC_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_LEAK_ADC_get", stop_bit, 3 );
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
    /* (0x00000838 bits 31:28) field MU_LEAK_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_ADC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_LEAK_ADC_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_MU_LEAK_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_MU_LEAK_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_LEAK_COEF_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_MU_LEAK_COEF_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_MU_LEAK_COEF_set", A, value );

    /* (0x00000838 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF,
                                          value);
}

static INLINE UINT32 c8_adsp_field_MU_LEAK_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_MU_LEAK_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_LEAK_COEF_get", A, 7);
    /* (0x00000838 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_MU_LEAK_COEF_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_MU_LEAK_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_MU_LEAK_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_LEAK_COEF_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_LEAK_COEF_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_LEAK_COEF_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_LEAK_COEF_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000838 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000838 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
        c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_MU_LEAK_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_MU_LEAK_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_LEAK_COEF_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_LEAK_COEF_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_LEAK_COEF_get", stop_bit, 3 );
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
    /* (0x00000838 bits 27:24) field MU_LEAK_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_COEF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_LEAK_COEF_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_MU_LEAK_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_MU_LEAK_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_LEAK_OFFSET_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_MU_LEAK_OFFSET_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_MU_LEAK_OFFSET_set", A, value );

    /* (0x00000838 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF,
                                          value);
}

static INLINE UINT32 c8_adsp_field_MU_LEAK_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_MU_LEAK_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_LEAK_OFFSET_get", A, 7);
    /* (0x00000838 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_MU_LEAK_OFFSET_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_MU_LEAK_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_MU_LEAK_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_LEAK_OFFSET_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_LEAK_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_LEAK_OFFSET_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_LEAK_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000838 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000838 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
        c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_MU_LEAK_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_MU_LEAK_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_LEAK_OFFSET_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_LEAK_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_LEAK_OFFSET_get", stop_bit, 3 );
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
    /* (0x00000838 bits 23:20) field MU_LEAK_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_LEAK_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_MU_LEAK_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_MU_LEAK_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_LEAK_GAIN_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_MU_LEAK_GAIN_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_MU_LEAK_GAIN_set", A, value );

    /* (0x00000838 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF,
                                          value);
}

static INLINE UINT32 c8_adsp_field_MU_LEAK_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_MU_LEAK_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_LEAK_GAIN_get", A, 7);
    /* (0x00000838 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_MU_LEAK_GAIN_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_MU_LEAK_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_MU_LEAK_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_LEAK_GAIN_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_LEAK_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_LEAK_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_LEAK_GAIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000838 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000838 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
        c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_MU_LEAK_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_MU_LEAK_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_LEAK_GAIN_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_LEAK_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_LEAK_GAIN_get", stop_bit, 3 );
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
    /* (0x00000838 bits 19:16) field MU_LEAK_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_LEAK_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_LEAK_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_MU_STEP_ADC_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_MU_STEP_ADC_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_STEP_ADC_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_MU_STEP_ADC_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_MU_STEP_ADC_set", A, value );

    /* (0x00000838 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF,
                                          value);
}

static INLINE UINT32 c8_adsp_field_MU_STEP_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_MU_STEP_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_STEP_ADC_get", A, 7);
    /* (0x00000838 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_MU_STEP_ADC_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_MU_STEP_ADC_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_MU_STEP_ADC_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_STEP_ADC_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_STEP_ADC_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_STEP_ADC_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_STEP_ADC_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000838 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000838 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
        c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_MU_STEP_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_MU_STEP_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_STEP_ADC_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_STEP_ADC_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_STEP_ADC_get", stop_bit, 2 );
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
    /* (0x00000838 bits 11:9) field MU_STEP_ADC of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_ADC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_STEP_ADC_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_MU_STEP_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_MU_STEP_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_STEP_COEF_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_MU_STEP_COEF_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_MU_STEP_COEF_set", A, value );

    /* (0x00000838 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF,
                                          value);
}

static INLINE UINT32 c8_adsp_field_MU_STEP_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_MU_STEP_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_STEP_COEF_get", A, 7);
    /* (0x00000838 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_MU_STEP_COEF_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_MU_STEP_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_MU_STEP_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_STEP_COEF_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_STEP_COEF_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_STEP_COEF_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_STEP_COEF_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000838 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000838 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
        c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_MU_STEP_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_MU_STEP_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_STEP_COEF_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_STEP_COEF_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_STEP_COEF_get", stop_bit, 2 );
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
    /* (0x00000838 bits 8:6) field MU_STEP_COEF of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_COEF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_STEP_COEF_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_MU_STEP_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_MU_STEP_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_STEP_OFFSET_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_MU_STEP_OFFSET_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_MU_STEP_OFFSET_set", A, value );

    /* (0x00000838 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF,
                                          value);
}

static INLINE UINT32 c8_adsp_field_MU_STEP_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_MU_STEP_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_STEP_OFFSET_get", A, 7);
    /* (0x00000838 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_MU_STEP_OFFSET_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_MU_STEP_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_MU_STEP_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_STEP_OFFSET_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_STEP_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_STEP_OFFSET_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_STEP_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000838 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000838 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
        c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_MU_STEP_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_MU_STEP_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_STEP_OFFSET_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_STEP_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_STEP_OFFSET_get", stop_bit, 2 );
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
    /* (0x00000838 bits 5:3) field MU_STEP_OFFSET of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_STEP_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_MU_STEP_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_MU_STEP_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_STEP_GAIN_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_MU_STEP_GAIN_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_MU_STEP_GAIN_set", A, value );

    /* (0x00000838 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                          h_ptr,
                                          A,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK,
                                          ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF,
                                          value);
}

static INLINE UINT32 c8_adsp_field_MU_STEP_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_MU_STEP_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_MU_STEP_GAIN_get", A, 7);
    /* (0x00000838 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK) >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_MU_STEP_GAIN_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_MU_STEP_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_MU_STEP_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_STEP_GAIN_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_STEP_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_STEP_GAIN_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_STEP_GAIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000838 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
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
        /* (0x00000838 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
        c8_adsp_reg_LMS_MU_FACTORS_field_set( b_ptr,
                                              h_ptr,
                                              A,
                                              subfield_mask << (ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF + subfield_offset),
                                              ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_MU_STEP_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_MU_STEP_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_MU_STEP_GAIN_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_MU_STEP_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_MU_STEP_GAIN_get", stop_bit, 2 );
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
    /* (0x00000838 bits 2:0) field MU_STEP_GAIN of register PMC_ADSP_REG_LMS_MU_FACTORS */
    reg_value = c8_adsp_reg_LMS_MU_FACTORS_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK)
                  >> ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_MSK, ADSP_REG_LMS_MU_FACTORS_BIT_MU_STEP_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_MU_STEP_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_RR_PERIOD_DSS_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_RR_PERIOD_DSS_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_DSS_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_RR_PERIOD_DSS_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_DSS_set", A, value );

    /* (0x0000083c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF,
                                               value);
}

static INLINE UINT32 c8_adsp_field_RR_PERIOD_DSS_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_RR_PERIOD_DSS_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_DSS_get", A, 7);
    /* (0x0000083c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_DSS_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_RR_PERIOD_DSS_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_RR_PERIOD_DSS_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_DSS_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_DSS_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_DSS_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_DSS_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000083c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000083c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_DSS_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_DSS_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_DSS_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_DSS_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_DSS_get", stop_bit, 3 );
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
    /* (0x0000083c bits 19:16) field RR_PERIOD_DSS of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_DSS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_DSS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_RR_PERIOD_ADC_MAP_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_RR_PERIOD_ADC_MAP_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_ADC_MAP_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_RR_PERIOD_ADC_MAP_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_ADC_MAP_set", A, value );

    /* (0x0000083c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF,
                                               value);
}

static INLINE UINT32 c8_adsp_field_RR_PERIOD_ADC_MAP_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_RR_PERIOD_ADC_MAP_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_ADC_MAP_get", A, 7);
    /* (0x0000083c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_ADC_MAP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_RR_PERIOD_ADC_MAP_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_RR_PERIOD_ADC_MAP_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_ADC_MAP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_ADC_MAP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_ADC_MAP_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_ADC_MAP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000083c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000083c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_ADC_MAP_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_ADC_MAP_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_ADC_MAP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_ADC_MAP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_ADC_MAP_get", stop_bit, 3 );
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
    /* (0x0000083c bits 15:12) field RR_PERIOD_ADC_MAP of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_ADC_MAP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_ADC_MAP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_RR_PERIOD_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_RR_PERIOD_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_OFFSET_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_RR_PERIOD_OFFSET_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_OFFSET_set", A, value );

    /* (0x0000083c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF,
                                               value);
}

static INLINE UINT32 c8_adsp_field_RR_PERIOD_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_RR_PERIOD_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_OFFSET_get", A, 7);
    /* (0x0000083c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_OFFSET_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_RR_PERIOD_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_RR_PERIOD_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_OFFSET_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_OFFSET_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000083c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000083c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_OFFSET_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_OFFSET_get", stop_bit, 3 );
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
    /* (0x0000083c bits 11:8) field RR_PERIOD_OFFSET of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_RR_PERIOD_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_RR_PERIOD_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_GAIN_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_RR_PERIOD_GAIN_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_GAIN_set", A, value );

    /* (0x0000083c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF,
                                               value);
}

static INLINE UINT32 c8_adsp_field_RR_PERIOD_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_RR_PERIOD_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_GAIN_get", A, 7);
    /* (0x0000083c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_GAIN_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_RR_PERIOD_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_RR_PERIOD_GAIN_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_GAIN_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_GAIN_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000083c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000083c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_GAIN_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_GAIN_get", stop_bit, 3 );
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
    /* (0x0000083c bits 7:4) field RR_PERIOD_GAIN of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_RR_PERIOD_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_RR_PERIOD_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_COEF_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_RR_PERIOD_COEF_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_COEF_set", A, value );

    /* (0x0000083c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK,
                                               ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF,
                                               value);
}

static INLINE UINT32 c8_adsp_field_RR_PERIOD_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_RR_PERIOD_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_RR_PERIOD_COEF_get", A, 7);
    /* (0x0000083c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK) >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_RR_PERIOD_COEF_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_RR_PERIOD_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_RR_PERIOD_COEF_set( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_COEF_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_COEF_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_COEF_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_COEF_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000083c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
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
        /* (0x0000083c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
        c8_adsp_reg_ROUND_ROBIN_PERIODS_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF + subfield_offset),
                                                   ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_RR_PERIOD_COEF_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_RR_PERIOD_COEF_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_RR_PERIOD_COEF_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_RR_PERIOD_COEF_get", stop_bit, 3 );
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
    /* (0x0000083c bits 3:0) field RR_PERIOD_COEF of register PMC_ADSP_REG_ROUND_ROBIN_PERIODS */
    reg_value = c8_adsp_reg_ROUND_ROBIN_PERIODS_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK)
                  >> ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_MSK, ADSP_REG_ROUND_ROBIN_PERIODS_BIT_RR_PERIOD_COEF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_RR_PERIOD_COEF_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_TR_FFE_B_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_TR_FFE_B_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TR_FFE_B_PRELOAD_set", A, 7);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_TR_FFE_B_PRELOAD_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_TR_FFE_B_PRELOAD_set", A, value );

    /* (0x00000840 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_TR_FFE_B_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_TR_FFE_B_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TR_FFE_B_PRELOAD_get", A, 7);
    /* (0x00000840 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_TR_FFE_B_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_TR_FFE_B_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_TR_FFE_B_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TR_FFE_B_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TR_FFE_B_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TR_FFE_B_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TR_FFE_B_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000840 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000840 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_TR_FFE_B_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_TR_FFE_B_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TR_FFE_B_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TR_FFE_B_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TR_FFE_B_PRELOAD_get", stop_bit, 4 );
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
    /* (0x00000840 bits 30:26) field TR_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_B_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TR_FFE_B_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_TR_FFE_A_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_TR_FFE_A_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TR_FFE_A_PRELOAD_set", A, 7);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_TR_FFE_A_PRELOAD_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_TR_FFE_A_PRELOAD_set", A, value );

    /* (0x00000840 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_TR_FFE_A_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_TR_FFE_A_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TR_FFE_A_PRELOAD_get", A, 7);
    /* (0x00000840 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_TR_FFE_A_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_TR_FFE_A_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_TR_FFE_A_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TR_FFE_A_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TR_FFE_A_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TR_FFE_A_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TR_FFE_A_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000840 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000840 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_TR_FFE_A_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_TR_FFE_A_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TR_FFE_A_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TR_FFE_A_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TR_FFE_A_PRELOAD_get", stop_bit, 4 );
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
    /* (0x00000840 bits 25:21) field TR_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_TR_FFE_A_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TR_FFE_A_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DP_FFE_C_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DP_FFE_C_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_C_PRELOAD_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DP_FFE_C_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DP_FFE_C_PRELOAD_set", A, value );

    /* (0x00000840 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_DP_FFE_C_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_C_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_C_PRELOAD_get", A, 7);
    /* (0x00000840 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_C_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DP_FFE_C_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DP_FFE_C_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_C_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_C_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_C_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_C_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000840 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000840 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DP_FFE_C_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_C_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_C_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_C_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_C_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000840 bits 20:14) field DP_FFE_C_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_C_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_C_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DP_FFE_B_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DP_FFE_B_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_B_PRELOAD_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DP_FFE_B_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DP_FFE_B_PRELOAD_set", A, value );

    /* (0x00000840 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_DP_FFE_B_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_B_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_B_PRELOAD_get", A, 7);
    /* (0x00000840 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_B_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DP_FFE_B_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DP_FFE_B_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_B_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_B_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_B_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_B_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000840 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000840 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DP_FFE_B_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_B_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_B_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_B_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_B_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000840 bits 13:7) field DP_FFE_B_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_B_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_B_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DP_FFE_A_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DP_FFE_A_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_A_PRELOAD_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DP_FFE_A_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DP_FFE_A_PRELOAD_set", A, value );

    /* (0x00000840 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_DP_FFE_A_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_A_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_A_PRELOAD_get", A, 7);
    /* (0x00000840 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_A_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DP_FFE_A_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DP_FFE_A_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_A_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_A_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_A_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_A_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000840 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
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
        /* (0x00000840 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DP_FFE_A_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_A_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_A_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_A_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_A_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000840 bits 6:0) field DP_FFE_A_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_1 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_1_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_1_BIT_DP_FFE_A_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_A_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF3_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF3_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF3_PRELOAD_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF3_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF3_PRELOAD_set", A, value );

    /* (0x00000844 bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF3_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF3_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF3_PRELOAD_get", A, 7);
    /* (0x00000844 bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF3_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF3_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF3_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF3_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF3_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF3_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF3_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000844 bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x00000844 bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF3_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF3_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF3_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF3_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF3_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000844 bits 27:21) field DFE_COEF3_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF3_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF3_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF2_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF2_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF2_PRELOAD_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF2_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF2_PRELOAD_set", A, value );

    /* (0x00000844 bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF2_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF2_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF2_PRELOAD_get", A, 7);
    /* (0x00000844 bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF2_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF2_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF2_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF2_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF2_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF2_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF2_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000844 bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x00000844 bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF2_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF2_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF2_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF2_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF2_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000844 bits 20:14) field DFE_COEF2_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF2_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF2_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF1_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF1_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF1_PRELOAD_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF1_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF1_PRELOAD_set", A, value );

    /* (0x00000844 bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF1_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF1_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF1_PRELOAD_get", A, 7);
    /* (0x00000844 bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF1_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF1_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF1_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF1_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF1_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF1_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF1_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000844 bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x00000844 bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF1_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF1_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF1_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF1_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF1_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000844 bits 13:7) field DFE_COEF1_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF1_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF1_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF0_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF0_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF0_PRELOAD_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF0_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF0_PRELOAD_set", A, value );

    /* (0x00000844 bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF0_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF0_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF0_PRELOAD_get", A, 7);
    /* (0x00000844 bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF0_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF0_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF0_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF0_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF0_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF0_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF0_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000844 bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
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
        /* (0x00000844 bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF0_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF0_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF0_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF0_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF0_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000844 bits 6:0) field DFE_COEF0_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_2 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_2_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_2_BIT_DFE_COEF0_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF0_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_DFE_COEF4_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_DFE_COEF4_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF4_PRELOAD_set", A, 7);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_DFE_COEF4_PRELOAD_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_DFE_COEF4_PRELOAD_set", A, value );

    /* (0x00000848 bits 29:23) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_DFE_COEF4_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF4_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF4_PRELOAD_get", A, 7);
    /* (0x00000848 bits 29:23) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF4_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_DFE_COEF4_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_DFE_COEF4_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF4_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF4_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF4_PRELOAD_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF4_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000848 bits 29:23) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000848 bits 29:23) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_DFE_COEF4_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF4_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF4_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF4_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF4_PRELOAD_get", stop_bit, 6 );
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
    /* (0x00000848 bits 29:23) field DFE_COEF4_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_DFE_COEF4_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF4_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_ADC_MAP_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_ADC_MAP_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADC_MAP_PRELOAD_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_ADC_MAP_PRELOAD_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_ADC_MAP_PRELOAD_set", A, value );

    /* (0x00000848 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_ADC_MAP_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADC_MAP_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADC_MAP_PRELOAD_get", A, 7);
    /* (0x00000848 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADC_MAP_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_ADC_MAP_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_ADC_MAP_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADC_MAP_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADC_MAP_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADC_MAP_PRELOAD_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADC_MAP_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000848 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000848 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_ADC_MAP_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_ADC_MAP_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADC_MAP_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADC_MAP_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADC_MAP_PRELOAD_get", stop_bit, 3 );
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
    /* (0x00000848 bits 22:19) field ADC_MAP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_ADC_MAP_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADC_MAP_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_TARGET_AMP_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_TARGET_AMP_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TARGET_AMP_PRELOAD_set", A, 7);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_TARGET_AMP_PRELOAD_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_TARGET_AMP_PRELOAD_set", A, value );

    /* (0x00000848 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_TARGET_AMP_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_TARGET_AMP_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TARGET_AMP_PRELOAD_get", A, 7);
    /* (0x00000848 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_TARGET_AMP_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_TARGET_AMP_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_TARGET_AMP_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TARGET_AMP_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TARGET_AMP_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TARGET_AMP_PRELOAD_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TARGET_AMP_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000848 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000848 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_TARGET_AMP_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                 c8_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_TARGET_AMP_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                 c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TARGET_AMP_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TARGET_AMP_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TARGET_AMP_PRELOAD_get", stop_bit, 5 );
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
    /* (0x00000848 bits 18:13) field TARGET_AMP_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_TARGET_AMP_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TARGET_AMP_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_PGA_OFFSET_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PGA_OFFSET_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PGA_OFFSET_PRELOAD_set", A, 7);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PGA_OFFSET_PRELOAD_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PGA_OFFSET_PRELOAD_set", A, value );

    /* (0x00000848 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_PGA_OFFSET_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PGA_OFFSET_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PGA_OFFSET_PRELOAD_get", A, 7);
    /* (0x00000848 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PGA_OFFSET_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_PGA_OFFSET_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_PGA_OFFSET_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_PGA_OFFSET_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_PGA_OFFSET_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_PGA_OFFSET_PRELOAD_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_PGA_OFFSET_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000848 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000848 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_PGA_OFFSET_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                 c8_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_PGA_OFFSET_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                                 c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_PGA_OFFSET_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_PGA_OFFSET_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_PGA_OFFSET_PRELOAD_get", stop_bit, 7 );
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
    /* (0x00000848 bits 12:5) field PGA_OFFSET_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_OFFSET_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_PGA_OFFSET_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_PGA_GAIN_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PGA_GAIN_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PGA_GAIN_PRELOAD_set", A, 7);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PGA_GAIN_PRELOAD_set", value, 31);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PGA_GAIN_PRELOAD_set", A, value );

    /* (0x00000848 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_MSK,
                                                ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_PGA_GAIN_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PGA_GAIN_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PGA_GAIN_PRELOAD_get", A, 7);
    /* (0x00000848 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_MSK) >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PGA_GAIN_PRELOAD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_PGA_GAIN_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_PGA_GAIN_PRELOAD_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_PGA_GAIN_PRELOAD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_PGA_GAIN_PRELOAD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_PGA_GAIN_PRELOAD_set", stop_bit, 4 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_PGA_GAIN_PRELOAD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000848 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
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
        /* (0x00000848 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
        c8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_OFF + subfield_offset),
                                                    ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_PGA_GAIN_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_PGA_GAIN_PRELOAD_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_PGA_GAIN_PRELOAD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_PGA_GAIN_PRELOAD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_PGA_GAIN_PRELOAD_get", stop_bit, 4 );
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
    /* (0x00000848 bits 4:0) field PGA_GAIN_PRELOAD of register PMC_ADSP_REG_OBJECT_PRELOAD_VAL_3 */
    reg_value = c8_adsp_reg_OBJECT_PRELOAD_VAL_3_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_MSK)
                  >> ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_MSK, ADSP_REG_OBJECT_PRELOAD_VAL_3_BIT_PGA_GAIN_PRELOAD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_PGA_GAIN_PRELOAD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_PROG_REF_OVR_EN_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PROG_REF_OVR_EN_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_OVR_EN_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PROG_REF_OVR_EN_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PROG_REF_OVR_EN_set", A, value );

    /* (0x0000084c bits 19) field PROG_REF_OVR_EN of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_PROG_REF_OVR_EN_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_OVR_EN_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_OVR_EN_get", A, 7);
    /* (0x0000084c bits 19) field PROG_REF_OVR_EN of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_EN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_OVR_EN_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_PROG_REF_OVR_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PROG_REF_OVR_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_OVR_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PROG_REF_OVR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PROG_REF_OVR_set", A, value );

    /* (0x0000084c bits 18) field PROG_REF_OVR of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_PROG_REF_OVR_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_OVR_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_OVR_get", A, 7);
    /* (0x0000084c bits 18) field PROG_REF_OVR of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_OVR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_OVR_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set", A, 7);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set", A, value );

    /* (0x0000084c bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get", A, 7);
    /* (0x0000084c bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000084c bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x0000084c bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get( c8_adsp_buffer_t *b_ptr,
                                                                      c8_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get( c8_adsp_buffer_t *b_ptr,
                                                                      c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get", stop_bit, 3 );
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
    /* (0x0000084c bits 17:14) field PROG_REF_TIMEOUT_PERIOD of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_PROG_REF_TIMEOUT_PERIOD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_CLIP_RATE_THRESH_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_CLIP_RATE_THRESH_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CLIP_RATE_THRESH_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_CLIP_RATE_THRESH_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_CLIP_RATE_THRESH_set", A, value );

    /* (0x0000084c bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_CLIP_RATE_THRESH_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_CLIP_RATE_THRESH_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CLIP_RATE_THRESH_get", A, 7);
    /* (0x0000084c bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_CLIP_RATE_THRESH_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_CLIP_RATE_THRESH_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_CLIP_RATE_THRESH_set( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_CLIP_RATE_THRESH_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_CLIP_RATE_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_CLIP_RATE_THRESH_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_CLIP_RATE_THRESH_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000084c bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x0000084c bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_CLIP_RATE_THRESH_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_CLIP_RATE_THRESH_get( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_CLIP_RATE_THRESH_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_CLIP_RATE_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_CLIP_RATE_THRESH_get", stop_bit, 2 );
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
    /* (0x0000084c bits 13:11) field CLIP_RATE_THRESH of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_CLIP_RATE_THRESH_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_CLIP_RATE_AMP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_CLIP_RATE_AMP_set( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CLIP_RATE_AMP_set", A, 7);
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_CLIP_RATE_AMP_set", value, 63);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_CLIP_RATE_AMP_set", A, value );

    /* (0x0000084c bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_CLIP_RATE_AMP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_CLIP_RATE_AMP_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CLIP_RATE_AMP_get", A, 7);
    /* (0x0000084c bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_CLIP_RATE_AMP_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_CLIP_RATE_AMP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_CLIP_RATE_AMP_set( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_CLIP_RATE_AMP_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_CLIP_RATE_AMP_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_CLIP_RATE_AMP_set", stop_bit, 5 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_CLIP_RATE_AMP_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000084c bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x0000084c bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_CLIP_RATE_AMP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_CLIP_RATE_AMP_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_CLIP_RATE_AMP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_CLIP_RATE_AMP_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_CLIP_RATE_AMP_get", stop_bit, 5 );
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
    /* (0x0000084c bits 10:5) field CLIP_RATE_AMP of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_CLIP_RATE_AMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_CLIP_RATE_AMP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_GAIN_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_GAIN_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_GAIN_OFFSET_set", A, 7);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_GAIN_OFFSET_set", value, 7);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_GAIN_OFFSET_set", A, value );

    /* (0x0000084c bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_GAIN_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_GAIN_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                    c8_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_GAIN_OFFSET_get", A, 7);
    /* (0x0000084c bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_GAIN_OFFSET_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_GAIN_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_GAIN_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_GAIN_OFFSET_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_GAIN_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_GAIN_OFFSET_set", stop_bit, 2 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_GAIN_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000084c bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x0000084c bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_GAIN_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_GAIN_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                          c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_GAIN_OFFSET_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_GAIN_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_GAIN_OFFSET_get", stop_bit, 2 );
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
    /* (0x0000084c bits 4:2) field GAIN_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_GAIN_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_GAIN_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void c8_adsp_field_ADC_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_ADC_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADC_OFFSET_set", A, 7);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_ADC_OFFSET_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_ADC_OFFSET_set", A, value );

    /* (0x0000084c bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK,
                                                ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF,
                                                value);
}

static INLINE UINT32 c8_adsp_field_ADC_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADC_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADC_OFFSET_get", A, 7);
    /* (0x0000084c bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK) >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADC_OFFSET_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_range_ADC_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_range_ADC_OFFSET_set( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADC_OFFSET_set", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADC_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADC_OFFSET_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADC_OFFSET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000084c bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
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
        /* (0x0000084c bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
        c8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set( b_ptr,
                                                    h_ptr,
                                                    A,
                                                    subfield_mask << (ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF + subfield_offset),
                                                    ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 c8_adsp_field_range_ADC_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_ADC_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADC_OFFSET_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADC_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADC_OFFSET_get", stop_bit, 1 );
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
    /* (0x0000084c bits 1:0) field ADC_OFFSET of register PMC_ADSP_REG_CLIP_RATE_MON_PARAMS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PARAMS_read( b_ptr,
                                                       h_ptr,
                                                       A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_MSK, ADSP_REG_CLIP_RATE_MON_PARAMS_BIT_ADC_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADC_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set( c8_adsp_buffer_t *b_ptr,
                                                                   c8_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set( c8_adsp_buffer_t *b_ptr,
                                                                   c8_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set", A, value );

    /* (0x0000080c bits 0) field CAPTURE_ADAPT_STATUS_REQUEST of register PMC_ADSP_REG_ADAPT_STAT */
    c8_adsp_reg_ADAPT_STAT_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_MSK,
                                      ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_OFF,
                                      value);
}

static INLINE UINT32 c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get( c8_adsp_buffer_t *b_ptr,
                                                                     c8_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get( c8_adsp_buffer_t *b_ptr,
                                                                     c8_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get", A, 7);
    /* (0x0000080c bits 0) field CAPTURE_ADAPT_STATUS_REQUEST of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = c8_adsp_reg_ADAPT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_MSK) >> ADSP_REG_ADAPT_STAT_BIT_CAPTURE_ADAPT_STATUS_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_CAPTURE_RESULTS_REQUEST_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_CAPTURE_RESULTS_REQUEST_set( c8_adsp_buffer_t *b_ptr,
                                                              c8_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CAPTURE_RESULTS_REQUEST_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_CAPTURE_RESULTS_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_CAPTURE_RESULTS_REQUEST_set", A, value );

    /* (0x00000810 bits 0) field CAPTURE_RESULTS_REQUEST of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST */
    c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_MSK,
                                                         ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_OFF,
                                                         value);
}

static INLINE UINT32 c8_adsp_field_CAPTURE_RESULTS_REQUEST_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_CAPTURE_RESULTS_REQUEST_get( c8_adsp_buffer_t *b_ptr,
                                                                c8_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CAPTURE_RESULTS_REQUEST_get", A, 7);
    /* (0x00000810 bits 0) field CAPTURE_RESULTS_REQUEST of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_REQUEST_BIT_CAPTURE_RESULTS_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_CAPTURE_RESULTS_REQUEST_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set( c8_adsp_buffer_t *b_ptr,
                                                                     c8_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set( c8_adsp_buffer_t *b_ptr,
                                                                     c8_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set", A, value );

    /* (0x00000850 bits 9) field CAPTURE_GAIN_CLIP_FLAG_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_MSK,
                                                     ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_OFF,
                                                     value);
}

static INLINE UINT32 c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get( c8_adsp_buffer_t *b_ptr,
                                                                       c8_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get( c8_adsp_buffer_t *b_ptr,
                                                                       c8_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get", A, 7);
    /* (0x00000850 bits 9) field CAPTURE_GAIN_CLIP_FLAG_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_GAIN_CLIP_FLAG_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set( c8_adsp_buffer_t *b_ptr,
                                                                     c8_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set( c8_adsp_buffer_t *b_ptr,
                                                                     c8_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set", A, value );

    /* (0x00000850 bits 0) field CAPTURE_ADC_CLIP_FLAGS_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_MSK,
                                                     ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_OFF,
                                                     value);
}

static INLINE UINT32 c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get( c8_adsp_buffer_t *b_ptr,
                                                                       c8_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get( c8_adsp_buffer_t *b_ptr,
                                                                       c8_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get", A, 7);
    /* (0x00000850 bits 0) field CAPTURE_ADC_CLIP_FLAGS_REQUEST of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CAPTURE_ADC_CLIP_FLAGS_REQUEST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_rising
 * ==================================================================================
 */
static INLINE void c8_adsp_field_PROG_REF_DONE_I_set_to_clear( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PROG_REF_DONE_I_set_to_clear( c8_adsp_buffer_t *b_ptr,
                                                               c8_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_DONE_I_set_to_clear", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PROG_REF_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PROG_REF_DONE_I_set_to_clear", A, value );

    /* (0x00000824 bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    c8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF,
                                                          value);
}

static INLINE UINT32 c8_adsp_field_PROG_REF_DONE_I_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_DONE_I_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_DONE_I_get", A, 7);
    /* (0x00000824 bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = c8_adsp_reg_STAT_INT_EVENT_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_DONE_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_PROG_REF_DONE_I_poll( c8_adsp_buffer_t *b_ptr,
                                                                       c8_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_PROG_REF_DONE_I_poll( c8_adsp_buffer_t *b_ptr,
                                                                       c8_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "c8_adsp_field_PROG_REF_DONE_I_poll", A, value );

    /* (0x00000824 bits 9) field PROG_REF_DONE_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return c8_adsp_reg_STAT_INT_EVENT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_MSK,
                                            (value<<ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_DONE_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void c8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear( c8_adsp_buffer_t *b_ptr,
                                                                  c8_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear( c8_adsp_buffer_t *b_ptr,
                                                                  c8_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear", A, value );

    /* (0x00000824 bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    c8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF,
                                                          value);
}

static INLINE UINT32 c8_adsp_field_PROG_REF_TIMEOUT_I_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_TIMEOUT_I_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_TIMEOUT_I_get", A, 7);
    /* (0x00000824 bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = c8_adsp_reg_STAT_INT_EVENT_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_PROG_REF_TIMEOUT_I_poll( c8_adsp_buffer_t *b_ptr,
                                                                          c8_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_PROG_REF_TIMEOUT_I_poll( c8_adsp_buffer_t *b_ptr,
                                                                          c8_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_I_poll", A, value );

    /* (0x00000824 bits 8) field PROG_REF_TIMEOUT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return c8_adsp_reg_STAT_INT_EVENT_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_MSK,
                                            (value<<ADSP_REG_STAT_INT_EVENT_BIT_PROG_REF_TIMEOUT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void c8_adsp_field_STOP_ADAPT_I_set_to_clear( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_STOP_ADAPT_I_set_to_clear( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_STOP_ADAPT_I_set_to_clear", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_STOP_ADAPT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_STOP_ADAPT_I_set_to_clear", A, value );

    /* (0x00000824 bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    c8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK,
                                                          ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF,
                                                          value);
}

static INLINE UINT32 c8_adsp_field_STOP_ADAPT_I_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_STOP_ADAPT_I_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_STOP_ADAPT_I_get", A, 7);
    /* (0x00000824 bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    reg_value = c8_adsp_reg_STAT_INT_EVENT_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_MSK) >> ADSP_REG_STAT_INT_EVENT_BIT_STOP_ADAPT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_STOP_ADAPT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_STOP_ADAPT_I_poll( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_STOP_ADAPT_I_poll( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "c8_adsp_field_STOP_ADAPT_I_poll", A, value );

    /* (0x00000824 bits 0) field STOP_ADAPT_I of register PMC_ADSP_REG_STAT_INT_EVENT */
    return c8_adsp_reg_STAT_INT_EVENT_poll( b_ptr,
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
static INLINE void c8_adsp_field_PROG_REF_DONE_E_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PROG_REF_DONE_E_set( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_DONE_E_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PROG_REF_DONE_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PROG_REF_DONE_E_set", A, value );

    /* (0x00000820 bits 9) field PROG_REF_DONE_E of register PMC_ADSP_REG_STAT_INT_EN */
    c8_adsp_reg_STAT_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_MSK,
                                       ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_OFF,
                                       value);
}

static INLINE UINT32 c8_adsp_field_PROG_REF_DONE_E_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_DONE_E_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_DONE_E_get", A, 7);
    /* (0x00000820 bits 9) field PROG_REF_DONE_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = c8_adsp_reg_STAT_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_PROG_REF_DONE_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_DONE_E_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_PROG_REF_TIMEOUT_E_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_PROG_REF_TIMEOUT_E_set( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_TIMEOUT_E_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_E_set", A, value );

    /* (0x00000820 bits 8) field PROG_REF_TIMEOUT_E of register PMC_ADSP_REG_STAT_INT_EN */
    c8_adsp_reg_STAT_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_MSK,
                                       ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_OFF,
                                       value);
}

static INLINE UINT32 c8_adsp_field_PROG_REF_TIMEOUT_E_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_TIMEOUT_E_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_TIMEOUT_E_get", A, 7);
    /* (0x00000820 bits 8) field PROG_REF_TIMEOUT_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = c8_adsp_reg_STAT_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_PROG_REF_TIMEOUT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_E_get", A, value );

    return value;
}
static INLINE void c8_adsp_field_STOP_ADAPT_E_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void c8_adsp_field_STOP_ADAPT_E_set( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_STOP_ADAPT_E_set", A, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "c8_adsp_field_STOP_ADAPT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "c8_adsp_field_STOP_ADAPT_E_set", A, value );

    /* (0x00000820 bits 0) field STOP_ADAPT_E of register PMC_ADSP_REG_STAT_INT_EN */
    c8_adsp_reg_STAT_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_MSK,
                                       ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_OFF,
                                       value);
}

static INLINE UINT32 c8_adsp_field_STOP_ADAPT_E_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_STOP_ADAPT_E_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_STOP_ADAPT_E_get", A, 7);
    /* (0x00000820 bits 0) field STOP_ADAPT_E of register PMC_ADSP_REG_STAT_INT_EN */
    reg_value = c8_adsp_reg_STAT_INT_EN_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_MSK) >> ADSP_REG_STAT_INT_EN_BIT_STOP_ADAPT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_STOP_ADAPT_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 c8_adsp_field_PROG_REF_DONE_V_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_DONE_V_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_DONE_V_get", A, 7);
    /* (0x00000828 bits 9) field PROG_REF_DONE_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = c8_adsp_reg_STAT_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_MSK) >> ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_DONE_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_PROG_REF_DONE_V_poll( c8_adsp_buffer_t *b_ptr,
                                                                       c8_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_PROG_REF_DONE_V_poll( c8_adsp_buffer_t *b_ptr,
                                                                       c8_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "c8_adsp_field_PROG_REF_DONE_V_poll", A, value );

    /* (0x00000828 bits 9) field PROG_REF_DONE_V of register PMC_ADSP_REG_STAT_INT */
    return c8_adsp_reg_STAT_INT_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_MSK,
                                      (value<<ADSP_REG_STAT_INT_BIT_PROG_REF_DONE_V_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 c8_adsp_field_PROG_REF_TIMEOUT_V_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PROG_REF_TIMEOUT_V_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PROG_REF_TIMEOUT_V_get", A, 7);
    /* (0x00000828 bits 8) field PROG_REF_TIMEOUT_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = c8_adsp_reg_STAT_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_MSK) >> ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_PROG_REF_TIMEOUT_V_poll( c8_adsp_buffer_t *b_ptr,
                                                                          c8_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_PROG_REF_TIMEOUT_V_poll( c8_adsp_buffer_t *b_ptr,
                                                                          c8_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "c8_adsp_field_PROG_REF_TIMEOUT_V_poll", A, value );

    /* (0x00000828 bits 8) field PROG_REF_TIMEOUT_V of register PMC_ADSP_REG_STAT_INT */
    return c8_adsp_reg_STAT_INT_poll( b_ptr,
                                      h_ptr,
                                      A,
                                      ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_MSK,
                                      (value<<ADSP_REG_STAT_INT_BIT_PROG_REF_TIMEOUT_V_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 c8_adsp_field_STOP_ADAPT_V_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_STOP_ADAPT_V_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_STOP_ADAPT_V_get", A, 7);
    /* (0x00000828 bits 0) field STOP_ADAPT_V of register PMC_ADSP_REG_STAT_INT */
    reg_value = c8_adsp_reg_STAT_INT_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_MSK) >> ADSP_REG_STAT_INT_BIT_STOP_ADAPT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_STOP_ADAPT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_STOP_ADAPT_V_poll( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE c8_adsp_field_STOP_ADAPT_V_poll( c8_adsp_buffer_t *b_ptr,
                                                                    c8_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "c8_adsp_field_STOP_ADAPT_V_poll", A, value );

    /* (0x00000828 bits 0) field STOP_ADAPT_V of register PMC_ADSP_REG_STAT_INT */
    return c8_adsp_reg_STAT_INT_poll( b_ptr,
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
static INLINE UINT32 c8_adsp_field_ADAPT_STATUS_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADAPT_STATUS_get( c8_adsp_buffer_t *b_ptr,
                                                     c8_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADAPT_STATUS_get", A, 7);
    /* (0x0000080c bits 11:1) field ADAPT_STATUS of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = c8_adsp_reg_ADAPT_STAT_read(  b_ptr, h_ptr, A);
    value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK) >> ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADAPT_STATUS_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_ADAPT_STATUS_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_ADAPT_STATUS_get( c8_adsp_buffer_t *b_ptr,
                                                           c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADAPT_STATUS_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADAPT_STATUS_get", stop_bit, start_bit );
    if (stop_bit > 10) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADAPT_STATUS_get", stop_bit, 10 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 10) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 10;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000080c bits 11:1) field ADAPT_STATUS of register PMC_ADSP_REG_ADAPT_STAT */
    reg_value = c8_adsp_reg_ADAPT_STAT_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK)
                  >> ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_MSK, ADSP_REG_ADAPT_STAT_BIT_ADAPT_STATUS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADAPT_STATUS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_TR_FFE_B_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_TR_FFE_B_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TR_FFE_B_get", A, 7);
    /* (0x00000814 bits 30:26) field TR_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_TR_FFE_B_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_TR_FFE_B_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_TR_FFE_B_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TR_FFE_B_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TR_FFE_B_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TR_FFE_B_get", stop_bit, 4 );
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
    /* (0x00000814 bits 30:26) field TR_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_B_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TR_FFE_B_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_TR_FFE_A_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_TR_FFE_A_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TR_FFE_A_get", A, 7);
    /* (0x00000814 bits 25:21) field TR_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_TR_FFE_A_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_TR_FFE_A_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_TR_FFE_A_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TR_FFE_A_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TR_FFE_A_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TR_FFE_A_get", stop_bit, 4 );
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
    /* (0x00000814 bits 25:21) field TR_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_TR_FFE_A_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TR_FFE_A_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_DP_FFE_C_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_C_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_C_get", A, 7);
    /* (0x00000814 bits 20:14) field DP_FFE_C of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_C_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_DP_FFE_C_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_C_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_C_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_C_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_C_get", stop_bit, 6 );
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
    /* (0x00000814 bits 20:14) field DP_FFE_C of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_C_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_C_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_DP_FFE_B_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_B_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_B_get", A, 7);
    /* (0x00000814 bits 13:7) field DP_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_B_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_DP_FFE_B_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_B_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_B_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_B_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_B_get", stop_bit, 6 );
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
    /* (0x00000814 bits 13:7) field DP_FFE_B of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_B_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_B_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_DP_FFE_A_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DP_FFE_A_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DP_FFE_A_get", A, 7);
    /* (0x00000814 bits 6:0) field DP_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DP_FFE_A_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_DP_FFE_A_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DP_FFE_A_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DP_FFE_A_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DP_FFE_A_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DP_FFE_A_get", stop_bit, 6 );
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
    /* (0x00000814 bits 6:0) field DP_FFE_A of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_1 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_1_BIT_DP_FFE_A_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DP_FFE_A_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_DFE_COEF3_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF3_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF3_get", A, 7);
    /* (0x00000818 bits 27:21) field DFE_COEF3 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF3_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_DFE_COEF3_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF3_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF3_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF3_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF3_get", stop_bit, 6 );
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
    /* (0x00000818 bits 27:21) field DFE_COEF3 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF3_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_DFE_COEF2_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF2_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF2_get", A, 7);
    /* (0x00000818 bits 20:14) field DFE_COEF2 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF2_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_DFE_COEF2_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF2_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF2_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF2_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF2_get", stop_bit, 6 );
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
    /* (0x00000818 bits 20:14) field DFE_COEF2 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_DFE_COEF1_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF1_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF1_get", A, 7);
    /* (0x00000818 bits 13:7) field DFE_COEF1 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF1_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_DFE_COEF1_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF1_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF1_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF1_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF1_get", stop_bit, 6 );
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
    /* (0x00000818 bits 13:7) field DFE_COEF1 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_DFE_COEF0_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF0_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF0_get", A, 7);
    /* (0x00000818 bits 6:0) field DFE_COEF0 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF0_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_DFE_COEF0_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF0_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF0_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF0_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF0_get", stop_bit, 6 );
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
    /* (0x00000818 bits 6:0) field DFE_COEF0 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_2 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_2_BIT_DFE_COEF0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_DFE_COEF4_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_DFE_COEF4_get( c8_adsp_buffer_t *b_ptr,
                                                  c8_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_DFE_COEF4_get", A, 7);
    /* (0x0000081c bits 29:23) field DFE_COEF4 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_DFE_COEF4_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_DFE_COEF4_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_DFE_COEF4_get( c8_adsp_buffer_t *b_ptr,
                                                        c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_DFE_COEF4_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_DFE_COEF4_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_DFE_COEF4_get", stop_bit, 6 );
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
    /* (0x0000081c bits 29:23) field DFE_COEF4 of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_DFE_COEF4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_DFE_COEF4_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_ADC_MAP_SCALE_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_ADC_MAP_SCALE_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_ADC_MAP_SCALE_get", A, 7);
    /* (0x0000081c bits 22:19) field ADC_MAP_SCALE of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_ADC_MAP_SCALE_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_ADC_MAP_SCALE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_ADC_MAP_SCALE_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_ADC_MAP_SCALE_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_ADC_MAP_SCALE_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_ADC_MAP_SCALE_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_ADC_MAP_SCALE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_ADC_MAP_SCALE_get", stop_bit, 3 );
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
    /* (0x0000081c bits 22:19) field ADC_MAP_SCALE of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_ADC_MAP_SCALE_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_ADC_MAP_SCALE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_ADC_MAP_SCALE_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_ADC_MAP_SCALE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_ADC_MAP_SCALE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_TARGET_AMP_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_TARGET_AMP_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_TARGET_AMP_get", A, 7);
    /* (0x0000081c bits 18:13) field TARGET_AMP of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_TARGET_AMP_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_TARGET_AMP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_TARGET_AMP_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_TARGET_AMP_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_TARGET_AMP_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_TARGET_AMP_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_TARGET_AMP_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_TARGET_AMP_get", stop_bit, 5 );
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
    /* (0x0000081c bits 18:13) field TARGET_AMP of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_TARGET_AMP_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_TARGET_AMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_TARGET_AMP_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_TARGET_AMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_TARGET_AMP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_PGA_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PGA_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                   c8_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PGA_OFFSET_get", A, 7);
    /* (0x0000081c bits 12:5) field PGA_OFFSET of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_OFFSET_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_OFFSET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PGA_OFFSET_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_PGA_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_PGA_OFFSET_get( c8_adsp_buffer_t *b_ptr,
                                                         c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_PGA_OFFSET_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_PGA_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_PGA_OFFSET_get", stop_bit, 7 );
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
    /* (0x0000081c bits 12:5) field PGA_OFFSET of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_OFFSET_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_OFFSET_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_PGA_OFFSET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_PGA_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_PGA_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                 c8_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_PGA_GAIN_get", A, 7);
    /* (0x0000081c bits 4:0) field PGA_GAIN of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_GAIN_MSK) >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_PGA_GAIN_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_PGA_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_PGA_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_PGA_GAIN_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_PGA_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_PGA_GAIN_get", stop_bit, 4 );
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
    /* (0x0000081c bits 4:0) field PGA_GAIN of register PMC_ADSP_REG_ADAPT_RESULTS_CAPTURE_3 */
    reg_value = c8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read( b_ptr,
                                                          h_ptr,
                                                          A);
    field_value = (reg_value & ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_GAIN_MSK)
                  >> ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_GAIN_MSK, ADSP_REG_ADAPT_RESULTS_CAPTURE_3_BIT_PGA_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_PGA_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_CLIP_FLAG_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_CLIP_FLAG_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                      c8_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CLIP_FLAG_ADC_get", A, 7);
    /* (0x00000850 bits 7:2) field CLIP_FLAG_ADC of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK) >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_CLIP_FLAG_ADC_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_CLIP_FLAG_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_CLIP_FLAG_ADC_get( c8_adsp_buffer_t *b_ptr,
                                                            c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_CLIP_FLAG_ADC_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_CLIP_FLAG_ADC_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_CLIP_FLAG_ADC_get", stop_bit, 5 );
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
    /* (0x00000850 bits 7:2) field CLIP_FLAG_ADC of register PMC_ADSP_REG_CLIP_RATE_MON_ADC_RESULTS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_MSK, ADSP_REG_CLIP_RATE_MON_ADC_RESULTS_BIT_CLIP_FLAG_ADC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_CLIP_FLAG_ADC_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_CLIP_FLAG_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_CLIP_FLAG_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                       c8_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_CLIP_FLAG_GAIN_get", A, 7);
    /* (0x00000854 bits 31:0) field CLIP_FLAG_GAIN of register PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    value = (reg_value & ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK) >> ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "c8_adsp_field_CLIP_FLAG_GAIN_get", A, value );

    return value;
}
static INLINE UINT32 c8_adsp_field_range_CLIP_FLAG_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 c8_adsp_field_range_CLIP_FLAG_GAIN_get( c8_adsp_buffer_t *b_ptr,
                                                             c8_handle_t *h_ptr,
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

    if (A > 7)
        IO_RANGE_CHECK("%s A is %d but max is %d", "c8_adsp_field_range_CLIP_FLAG_GAIN_get", A, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "c8_adsp_field_range_CLIP_FLAG_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "c8_adsp_field_range_CLIP_FLAG_GAIN_get", stop_bit, 31 );
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
    /* (0x00000854 bits 31:0) field CLIP_FLAG_GAIN of register PMC_ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS */
    reg_value = c8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read( b_ptr,
                                                                 h_ptr,
                                                                 A);
    field_value = (reg_value & ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK)
                  >> ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_MSK, ADSP_REG_CLIP_RATE_MON_PGA_GAIN_RESULTS_BIT_CLIP_FLAG_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "c8_adsp_field_range_CLIP_FLAG_GAIN_get", A, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _C8_ADSP_IO_INLINE_H */
