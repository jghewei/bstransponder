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
 *     and register accessor functions for the i4_fec block
 *****************************************************************************/
#ifndef _I4_FEC_IO_INLINE_H
#define _I4_FEC_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "i4_fec_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define I4_FEC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for i4_fec
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
} i4_fec_buffer_t;
static INLINE void i4_fec_buffer_init( i4_fec_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A ) ALWAYS_INLINE;
static INLINE void i4_fec_buffer_init( i4_fec_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x2000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "i4_fec_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void i4_fec_buffer_flush( i4_fec_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void i4_fec_buffer_flush( i4_fec_buffer_t *b_ptr )
{
    IOLOG( "i4_fec_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 i4_fec_reg_read( i4_fec_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_read( i4_fec_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x2000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg);
    return value;
}
static INLINE void i4_fec_reg_write( i4_fec_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_write( i4_fec_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x2000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                         value);
}

static INLINE void i4_fec_field_set( i4_fec_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_set( i4_fec_buffer_t *b_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x2000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void i4_fec_action_on_write_field_set( i4_fec_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_action_on_write_field_set( i4_fec_buffer_t *b_ptr,
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
                                         (b_ptr->base_address + (b_ptr->A)*0x2000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                         value<<ofs);
}

static INLINE void i4_fec_burst_read( i4_fec_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void i4_fec_burst_read( i4_fec_buffer_t *b_ptr,
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
                          (b_ptr->base_address + (b_ptr->A)*0x2000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                          len,
                          value);
}

static INLINE void i4_fec_burst_write( i4_fec_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void i4_fec_burst_write( i4_fec_buffer_t *b_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x2000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE i4_fec_poll( i4_fec_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE i4_fec_poll( i4_fec_buffer_t *b_ptr,
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
                           (b_ptr->base_address + (b_ptr->A)*0x2000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x2000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for i4_fec
 * ==================================================================================
 */

static INLINE void i4_fec_reg_FEC_ENCODER_CONFIG_write( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_FEC_ENCODER_CONFIG_write( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "i4_fec_reg_FEC_ENCODER_CONFIG_write", value );
    i4_fec_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_FEC_ENCODER_CONFIG,
                      value);
}

static INLINE void i4_fec_reg_FEC_ENCODER_CONFIG_field_set( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_FEC_ENCODER_CONFIG_field_set( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "i4_fec_reg_FEC_ENCODER_CONFIG_field_set", A, mask, ofs, value );
    i4_fec_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_FEC_ENCODER_CONFIG,
                      mask,
                      PMC_I4_FEC_REG_FEC_ENCODER_CONFIG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 i4_fec_reg_FEC_ENCODER_CONFIG_read( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_FEC_ENCODER_CONFIG_read( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_I4_FEC_REG_FEC_ENCODER_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_FEC_ENCODER_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void i4_fec_reg_FEC_DECODER_CONFIG_write( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_FEC_DECODER_CONFIG_write( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "i4_fec_reg_FEC_DECODER_CONFIG_write", value );
    i4_fec_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_FEC_DECODER_CONFIG,
                      value);
}

static INLINE void i4_fec_reg_FEC_DECODER_CONFIG_field_set( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_FEC_DECODER_CONFIG_field_set( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "i4_fec_reg_FEC_DECODER_CONFIG_field_set", A, mask, ofs, value );
    i4_fec_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_FEC_DECODER_CONFIG,
                      mask,
                      PMC_I4_FEC_REG_FEC_DECODER_CONFIG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 i4_fec_reg_FEC_DECODER_CONFIG_read( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_FEC_DECODER_CONFIG_read( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_I4_FEC_REG_FEC_DECODER_CONFIG);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_FEC_DECODER_CONFIG_read", reg_value, A);
    return reg_value;
}

static INLINE void i4_fec_reg_LPA_CNT_MODE_write( i4_fec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_LPA_CNT_MODE_write( i4_fec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "i4_fec_reg_LPA_CNT_MODE_write", value );
    i4_fec_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_LPA_CNT_MODE,
                      value);
}

static INLINE void i4_fec_reg_LPA_CNT_MODE_field_set( i4_fec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_LPA_CNT_MODE_field_set( i4_fec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "i4_fec_reg_LPA_CNT_MODE_field_set", A, mask, ofs, value );
    i4_fec_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_LPA_CNT_MODE,
                      mask,
                      PMC_I4_FEC_REG_LPA_CNT_MODE_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 i4_fec_reg_LPA_CNT_MODE_read( i4_fec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_LPA_CNT_MODE_read( i4_fec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_I4_FEC_REG_LPA_CNT_MODE);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_LPA_CNT_MODE_read", reg_value, A);
    return reg_value;
}

static INLINE void i4_fec_reg_LLSA_CNT_MODE_write( i4_fec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_LLSA_CNT_MODE_write( i4_fec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "i4_fec_reg_LLSA_CNT_MODE_write", value );
    i4_fec_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_LLSA_CNT_MODE,
                      value);
}

static INLINE void i4_fec_reg_LLSA_CNT_MODE_field_set( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_LLSA_CNT_MODE_field_set( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "i4_fec_reg_LLSA_CNT_MODE_field_set", A, mask, ofs, value );
    i4_fec_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_LLSA_CNT_MODE,
                      mask,
                      PMC_I4_FEC_REG_LLSA_CNT_MODE_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 i4_fec_reg_LLSA_CNT_MODE_read( i4_fec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_LLSA_CNT_MODE_read( i4_fec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_I4_FEC_REG_LLSA_CNT_MODE);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_LLSA_CNT_MODE_read", reg_value, A);
    return reg_value;
}

static INLINE void i4_fec_reg_LLSA_LATCH_SOURCE_write( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_LLSA_LATCH_SOURCE_write( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "i4_fec_reg_LLSA_LATCH_SOURCE_write", value );
    i4_fec_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_LLSA_LATCH_SOURCE,
                      value);
}

static INLINE void i4_fec_reg_LLSA_LATCH_SOURCE_field_set( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_LLSA_LATCH_SOURCE_field_set( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "i4_fec_reg_LLSA_LATCH_SOURCE_field_set", A, mask, ofs, value );
    i4_fec_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_LLSA_LATCH_SOURCE,
                      mask,
                      PMC_I4_FEC_REG_LLSA_LATCH_SOURCE_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 i4_fec_reg_LLSA_LATCH_SOURCE_read( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_LLSA_LATCH_SOURCE_read( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_I4_FEC_REG_LLSA_LATCH_SOURCE);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_LLSA_LATCH_SOURCE_read", reg_value, A);
    return reg_value;
}

static INLINE void i4_fec_reg_FEC_CTRL_write( i4_fec_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_FEC_CTRL_write( i4_fec_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 A,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "i4_fec_reg_FEC_CTRL_write", value );
    i4_fec_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_FEC_CTRL,
                      value);
}

static INLINE void i4_fec_reg_FEC_CTRL_field_set( i4_fec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_FEC_CTRL_field_set( i4_fec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "i4_fec_reg_FEC_CTRL_field_set", A, mask, ofs, value );
    i4_fec_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_I4_FEC_REG_FEC_CTRL,
                      mask,
                      PMC_I4_FEC_REG_FEC_CTRL_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 i4_fec_reg_FEC_CTRL_read( i4_fec_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_FEC_CTRL_read( i4_fec_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_I4_FEC_REG_FEC_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_FEC_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_CORRECTED_ZEROES_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_CORRECTED_ZEROES_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_CORRECTED_ZEROES_3_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_CORRECTED_ZEROES_3_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_CORRECTED_ZEROES_2_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_CORRECTED_ZEROES_2_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_CORRECTED_ZEROES_2_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_CORRECTED_ZEROES_2_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_CORRECTED_ZEROES_1_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_CORRECTED_ZEROES_1_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_CORRECTED_ZEROES_1_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_CORRECTED_ZEROES_1_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_CORRECTED_ZEROES_0_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_CORRECTED_ZEROES_0_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_CORRECTED_ZEROES_0_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_CORRECTED_ZEROES_0_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_CORRECTED_ONES_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_CORRECTED_ONES_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_CORRECTED_ONES_3_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_CORRECTED_ONES_3_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_CORRECTED_ONES_2_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_CORRECTED_ONES_2_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_CORRECTED_ONES_2_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_CORRECTED_ONES_2_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_CORRECTED_ONES_1_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_CORRECTED_ONES_1_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_CORRECTED_ONES_1_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_CORRECTED_ONES_1_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_CORRECTED_ONES_0_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_CORRECTED_ONES_0_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_CORRECTED_ONES_0_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_CORRECTED_ONES_0_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_UNCORRECTED_ERRS_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_UNCORRECTED_ERRS_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_UNCORRECTED_ERRS_3_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_UNCORRECTED_ERRS_3_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_UNCORRECTED_ERRS_2_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_UNCORRECTED_ERRS_2_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_UNCORRECTED_ERRS_2_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_UNCORRECTED_ERRS_2_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_UNCORRECTED_ERRS_1_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_UNCORRECTED_ERRS_1_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_UNCORRECTED_ERRS_1_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_UNCORRECTED_ERRS_1_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_UNCORRECTED_ERRS_0_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_UNCORRECTED_ERRS_0_CNT_read( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_UNCORRECTED_ERRS_0_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_UNCORRECTED_ERRS_0_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_RX_FRAME_PULSE_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_RX_FRAME_PULSE_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_RX_FRAME_PULSE_3_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_RX_FRAME_PULSE_3_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_RX_FRAME_PULSE_2_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_RX_FRAME_PULSE_2_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_RX_FRAME_PULSE_2_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_RX_FRAME_PULSE_2_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_RX_FRAME_PULSE_1_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_RX_FRAME_PULSE_1_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_RX_FRAME_PULSE_1_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_RX_FRAME_PULSE_1_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_RX_FRAME_PULSE_0_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_RX_FRAME_PULSE_0_CNT_read( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_RX_FRAME_PULSE_0_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_RX_FRAME_PULSE_0_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_LLSA_UNCORRECTED_ERRS_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_LLSA_UNCORRECTED_ERRS_3_CNT_read( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_LLSA_UNCORRECTED_ERRS_3_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 i4_fec_reg_LLSA_STATISTICS_FAILURE_STAT_read( i4_fec_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_LLSA_STATISTICS_FAILURE_STAT_read( i4_fec_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_LLSA_STATISTICS_FAILURE_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE void i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_burst_read( i4_fec_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_burst_read( i4_fec_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_burst_read", ofs, len, 16 );
    i4_fec_burst_read( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT(ofs),
                       len,
                       value);

    IOLOG( "%s of %d words A=%d ofs=%d len=%d", "i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_burst_read", 16, A, ofs, len);
}

static INLINE UINT32 i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_read( i4_fec_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_read( i4_fec_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_0S_CNT(N));

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_read", reg_value, A);
    return reg_value;
}

static INLINE void i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_burst_read( i4_fec_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_burst_read( i4_fec_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 ofs,
                                                                                 UINT32 len,
                                                                                 UINT32 *value )
{
    if (len + ofs > 16)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_burst_read", ofs, len, 16 );
    i4_fec_burst_read( b_ptr,
                       h_ptr,
                       A,
                       MEM_TYPE_STATUS,
                       PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT(ofs),
                       len,
                       value);

    IOLOG( "%s of %d words A=%d ofs=%d len=%d", "i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_burst_read", 16, A, ofs, len);
}

static INLINE UINT32 i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_read( i4_fec_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_read( i4_fec_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = i4_fec_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_I4_FEC_REG_LLSA_STATISTICS_CORRECTED_1S_CNT(N));

    IOLOG( "%s -> 0x%08x; A=%d", "i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void i4_fec_field_ENCODER_DATA_TYPE_set( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_ENCODER_DATA_TYPE_set( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_ENCODER_DATA_TYPE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_ENCODER_DATA_TYPE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_ENCODER_DATA_TYPE_set", A, value );

    /* (0x00028000 bits 0) field ENCODER_DATA_TYPE of register PMC_I4_FEC_REG_FEC_ENCODER_CONFIG */
    i4_fec_reg_FEC_ENCODER_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             I4_FEC_REG_FEC_ENCODER_CONFIG_BIT_ENCODER_DATA_TYPE_MSK,
                                             I4_FEC_REG_FEC_ENCODER_CONFIG_BIT_ENCODER_DATA_TYPE_OFF,
                                             value);
}

static INLINE UINT32 i4_fec_field_ENCODER_DATA_TYPE_get( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_ENCODER_DATA_TYPE_get( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_ENCODER_DATA_TYPE_get", A, 2);
    /* (0x00028000 bits 0) field ENCODER_DATA_TYPE of register PMC_I4_FEC_REG_FEC_ENCODER_CONFIG */
    reg_value = i4_fec_reg_FEC_ENCODER_CONFIG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & I4_FEC_REG_FEC_ENCODER_CONFIG_BIT_ENCODER_DATA_TYPE_MSK) >> I4_FEC_REG_FEC_ENCODER_CONFIG_BIT_ENCODER_DATA_TYPE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_ENCODER_DATA_TYPE_get", A, value );

    return value;
}
static INLINE void i4_fec_field_DECODER_LN_DISABLE_set( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_DECODER_LN_DISABLE_set( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_DECODER_LN_DISABLE_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_DECODER_LN_DISABLE_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_DECODER_LN_DISABLE_set", A, value );

    /* (0x00028004 bits 11:8) field DECODER_LN_DISABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    i4_fec_reg_FEC_DECODER_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_MSK,
                                             I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_OFF,
                                             value);
}

static INLINE UINT32 i4_fec_field_DECODER_LN_DISABLE_get( i4_fec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_DECODER_LN_DISABLE_get( i4_fec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_DECODER_LN_DISABLE_get", A, 2);
    /* (0x00028004 bits 11:8) field DECODER_LN_DISABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    reg_value = i4_fec_reg_FEC_DECODER_CONFIG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_MSK) >> I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_DECODER_LN_DISABLE_get", A, value );

    return value;
}
static INLINE void i4_fec_field_range_DECODER_LN_DISABLE_set( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_range_DECODER_LN_DISABLE_set( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_DECODER_LN_DISABLE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_DECODER_LN_DISABLE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_DECODER_LN_DISABLE_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_DECODER_LN_DISABLE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00028004 bits 11:8) field DECODER_LN_DISABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
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
        /* (0x00028004 bits 11:8) field DECODER_LN_DISABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
        i4_fec_reg_FEC_DECODER_CONFIG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_OFF + subfield_offset),
                                                 I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 i4_fec_field_range_DECODER_LN_DISABLE_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_DECODER_LN_DISABLE_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_DECODER_LN_DISABLE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_DECODER_LN_DISABLE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_DECODER_LN_DISABLE_get", stop_bit, 3 );
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
    /* (0x00028004 bits 11:8) field DECODER_LN_DISABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    reg_value = i4_fec_reg_FEC_DECODER_CONFIG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_MSK)
                  >> I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_MSK, I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_LN_DISABLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_DECODER_LN_DISABLE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void i4_fec_field_DECODER_STATS_ENABLE_set( i4_fec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_DECODER_STATS_ENABLE_set( i4_fec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_DECODER_STATS_ENABLE_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_DECODER_STATS_ENABLE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_DECODER_STATS_ENABLE_set", A, value );

    /* (0x00028004 bits 5:4) field DECODER_STATS_ENABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    i4_fec_reg_FEC_DECODER_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_MSK,
                                             I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_OFF,
                                             value);
}

static INLINE UINT32 i4_fec_field_DECODER_STATS_ENABLE_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_DECODER_STATS_ENABLE_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_DECODER_STATS_ENABLE_get", A, 2);
    /* (0x00028004 bits 5:4) field DECODER_STATS_ENABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    reg_value = i4_fec_reg_FEC_DECODER_CONFIG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_MSK) >> I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_DECODER_STATS_ENABLE_get", A, value );

    return value;
}
static INLINE void i4_fec_field_range_DECODER_STATS_ENABLE_set( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_range_DECODER_STATS_ENABLE_set( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_DECODER_STATS_ENABLE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_DECODER_STATS_ENABLE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_DECODER_STATS_ENABLE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_DECODER_STATS_ENABLE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00028004 bits 5:4) field DECODER_STATS_ENABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
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
        /* (0x00028004 bits 5:4) field DECODER_STATS_ENABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
        i4_fec_reg_FEC_DECODER_CONFIG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_OFF + subfield_offset),
                                                 I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 i4_fec_field_range_DECODER_STATS_ENABLE_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_DECODER_STATS_ENABLE_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_DECODER_STATS_ENABLE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_DECODER_STATS_ENABLE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_DECODER_STATS_ENABLE_get", stop_bit, 1 );
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
    /* (0x00028004 bits 5:4) field DECODER_STATS_ENABLE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    reg_value = i4_fec_reg_FEC_DECODER_CONFIG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_MSK)
                  >> I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_MSK, I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_STATS_ENABLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_DECODER_STATS_ENABLE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void i4_fec_field_DECODER_OP_MODE_set( i4_fec_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_DECODER_OP_MODE_set( i4_fec_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_DECODER_OP_MODE_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_DECODER_OP_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_DECODER_OP_MODE_set", A, value );

    /* (0x00028004 bits 1:0) field DECODER_OP_MODE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    i4_fec_reg_FEC_DECODER_CONFIG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_MSK,
                                             I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_OFF,
                                             value);
}

static INLINE UINT32 i4_fec_field_DECODER_OP_MODE_get( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_DECODER_OP_MODE_get( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_DECODER_OP_MODE_get", A, 2);
    /* (0x00028004 bits 1:0) field DECODER_OP_MODE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    reg_value = i4_fec_reg_FEC_DECODER_CONFIG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_MSK) >> I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_DECODER_OP_MODE_get", A, value );

    return value;
}
static INLINE void i4_fec_field_range_DECODER_OP_MODE_set( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_range_DECODER_OP_MODE_set( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_DECODER_OP_MODE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_DECODER_OP_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_DECODER_OP_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_DECODER_OP_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00028004 bits 1:0) field DECODER_OP_MODE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
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
        /* (0x00028004 bits 1:0) field DECODER_OP_MODE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
        i4_fec_reg_FEC_DECODER_CONFIG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_OFF + subfield_offset),
                                                 I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 i4_fec_field_range_DECODER_OP_MODE_get( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_DECODER_OP_MODE_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_DECODER_OP_MODE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_DECODER_OP_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_DECODER_OP_MODE_get", stop_bit, 1 );
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
    /* (0x00028004 bits 1:0) field DECODER_OP_MODE of register PMC_I4_FEC_REG_FEC_DECODER_CONFIG */
    reg_value = i4_fec_reg_FEC_DECODER_CONFIG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_MSK)
                  >> I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_MSK, I4_FEC_REG_FEC_DECODER_CONFIG_BIT_DECODER_OP_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_DECODER_OP_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void i4_fec_field_LPA_COUNTER_MODE_set( i4_fec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_LPA_COUNTER_MODE_set( i4_fec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_LPA_COUNTER_MODE_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_LPA_COUNTER_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_LPA_COUNTER_MODE_set", A, value );

    /* (0x000280d8 bits 1:0) field LPA_COUNTER_MODE of register PMC_I4_FEC_REG_LPA_CNT_MODE */
    i4_fec_reg_LPA_CNT_MODE_field_set( b_ptr,
                                       h_ptr,
                                       A,
                                       I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_MSK,
                                       I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_OFF,
                                       value);
}

static INLINE UINT32 i4_fec_field_LPA_COUNTER_MODE_get( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_LPA_COUNTER_MODE_get( i4_fec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_LPA_COUNTER_MODE_get", A, 2);
    /* (0x000280d8 bits 1:0) field LPA_COUNTER_MODE of register PMC_I4_FEC_REG_LPA_CNT_MODE */
    reg_value = i4_fec_reg_LPA_CNT_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_MSK) >> I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_LPA_COUNTER_MODE_get", A, value );

    return value;
}
static INLINE void i4_fec_field_range_LPA_COUNTER_MODE_set( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_range_LPA_COUNTER_MODE_set( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_LPA_COUNTER_MODE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_LPA_COUNTER_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_LPA_COUNTER_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_LPA_COUNTER_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000280d8 bits 1:0) field LPA_COUNTER_MODE of register PMC_I4_FEC_REG_LPA_CNT_MODE */
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
        /* (0x000280d8 bits 1:0) field LPA_COUNTER_MODE of register PMC_I4_FEC_REG_LPA_CNT_MODE */
        i4_fec_reg_LPA_CNT_MODE_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_OFF + subfield_offset),
                                           I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 i4_fec_field_range_LPA_COUNTER_MODE_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_LPA_COUNTER_MODE_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_LPA_COUNTER_MODE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_LPA_COUNTER_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_LPA_COUNTER_MODE_get", stop_bit, 1 );
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
    /* (0x000280d8 bits 1:0) field LPA_COUNTER_MODE of register PMC_I4_FEC_REG_LPA_CNT_MODE */
    reg_value = i4_fec_reg_LPA_CNT_MODE_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_MSK)
                  >> I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_MSK, I4_FEC_REG_LPA_CNT_MODE_BIT_LPA_COUNTER_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_LPA_COUNTER_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void i4_fec_field_LLSA_COUNTER_MODE_set( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_LLSA_COUNTER_MODE_set( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_LLSA_COUNTER_MODE_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_LLSA_COUNTER_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_LLSA_COUNTER_MODE_set", A, value );

    /* (0x000280e8 bits 1:0) field LLSA_COUNTER_MODE of register PMC_I4_FEC_REG_LLSA_CNT_MODE */
    i4_fec_reg_LLSA_CNT_MODE_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_MSK,
                                        I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_OFF,
                                        value);
}

static INLINE UINT32 i4_fec_field_LLSA_COUNTER_MODE_get( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_LLSA_COUNTER_MODE_get( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_LLSA_COUNTER_MODE_get", A, 2);
    /* (0x000280e8 bits 1:0) field LLSA_COUNTER_MODE of register PMC_I4_FEC_REG_LLSA_CNT_MODE */
    reg_value = i4_fec_reg_LLSA_CNT_MODE_read(  b_ptr, h_ptr, A);
    value = (reg_value & I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_MSK) >> I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_LLSA_COUNTER_MODE_get", A, value );

    return value;
}
static INLINE void i4_fec_field_range_LLSA_COUNTER_MODE_set( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_range_LLSA_COUNTER_MODE_set( i4_fec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_LLSA_COUNTER_MODE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_LLSA_COUNTER_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_LLSA_COUNTER_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_LLSA_COUNTER_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000280e8 bits 1:0) field LLSA_COUNTER_MODE of register PMC_I4_FEC_REG_LLSA_CNT_MODE */
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
        /* (0x000280e8 bits 1:0) field LLSA_COUNTER_MODE of register PMC_I4_FEC_REG_LLSA_CNT_MODE */
        i4_fec_reg_LLSA_CNT_MODE_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_OFF + subfield_offset),
                                            I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 i4_fec_field_range_LLSA_COUNTER_MODE_get( i4_fec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_LLSA_COUNTER_MODE_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_LLSA_COUNTER_MODE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_LLSA_COUNTER_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_LLSA_COUNTER_MODE_get", stop_bit, 1 );
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
    /* (0x000280e8 bits 1:0) field LLSA_COUNTER_MODE of register PMC_I4_FEC_REG_LLSA_CNT_MODE */
    reg_value = i4_fec_reg_LLSA_CNT_MODE_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_MSK)
                  >> I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_MSK, I4_FEC_REG_LLSA_CNT_MODE_BIT_LLSA_COUNTER_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_LLSA_COUNTER_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void i4_fec_field_LLSA_LATCH_SOURCE_set( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_LLSA_LATCH_SOURCE_set( i4_fec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_LLSA_LATCH_SOURCE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_LLSA_LATCH_SOURCE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_LLSA_LATCH_SOURCE_set", A, value );

    /* (0x000280ec bits 0) field LLSA_LATCH_SOURCE of register PMC_I4_FEC_REG_LLSA_LATCH_SOURCE */
    i4_fec_reg_LLSA_LATCH_SOURCE_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            I4_FEC_REG_LLSA_LATCH_SOURCE_BIT_LLSA_LATCH_SOURCE_MSK,
                                            I4_FEC_REG_LLSA_LATCH_SOURCE_BIT_LLSA_LATCH_SOURCE_OFF,
                                            value);
}

static INLINE UINT32 i4_fec_field_LLSA_LATCH_SOURCE_get( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_LLSA_LATCH_SOURCE_get( i4_fec_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_LLSA_LATCH_SOURCE_get", A, 2);
    /* (0x000280ec bits 0) field LLSA_LATCH_SOURCE of register PMC_I4_FEC_REG_LLSA_LATCH_SOURCE */
    reg_value = i4_fec_reg_LLSA_LATCH_SOURCE_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & I4_FEC_REG_LLSA_LATCH_SOURCE_BIT_LLSA_LATCH_SOURCE_MSK) >> I4_FEC_REG_LLSA_LATCH_SOURCE_BIT_LLSA_LATCH_SOURCE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_LLSA_LATCH_SOURCE_get", A, value );

    return value;
}
static INLINE void i4_fec_field_FEC_RAM_LOWPWR_set( i4_fec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_FEC_RAM_LOWPWR_set( i4_fec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_FEC_RAM_LOWPWR_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_FEC_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_FEC_RAM_LOWPWR_set", A, value );

    /* (0x00028114 bits 2) field FEC_RAM_LOWPWR of register PMC_I4_FEC_REG_FEC_CTRL */
    i4_fec_reg_FEC_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   I4_FEC_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_MSK,
                                   I4_FEC_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_OFF,
                                   value);
}

static INLINE UINT32 i4_fec_field_FEC_RAM_LOWPWR_get( i4_fec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_FEC_RAM_LOWPWR_get( i4_fec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_FEC_RAM_LOWPWR_get", A, 2);
    /* (0x00028114 bits 2) field FEC_RAM_LOWPWR of register PMC_I4_FEC_REG_FEC_CTRL */
    reg_value = i4_fec_reg_FEC_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & I4_FEC_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_MSK) >> I4_FEC_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_FEC_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void i4_fec_field_FEC_LOWPWR_set( i4_fec_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_FEC_LOWPWR_set( i4_fec_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_FEC_LOWPWR_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_FEC_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_FEC_LOWPWR_set", A, value );

    /* (0x00028114 bits 1) field FEC_LOWPWR of register PMC_I4_FEC_REG_FEC_CTRL */
    i4_fec_reg_FEC_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   I4_FEC_REG_FEC_CTRL_BIT_FEC_LOWPWR_MSK,
                                   I4_FEC_REG_FEC_CTRL_BIT_FEC_LOWPWR_OFF,
                                   value);
}

static INLINE UINT32 i4_fec_field_FEC_LOWPWR_get( i4_fec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_FEC_LOWPWR_get( i4_fec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_FEC_LOWPWR_get", A, 2);
    /* (0x00028114 bits 1) field FEC_LOWPWR of register PMC_I4_FEC_REG_FEC_CTRL */
    reg_value = i4_fec_reg_FEC_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & I4_FEC_REG_FEC_CTRL_BIT_FEC_LOWPWR_MSK) >> I4_FEC_REG_FEC_CTRL_BIT_FEC_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_FEC_LOWPWR_get", A, value );

    return value;
}
static INLINE void i4_fec_field_FEC_SW_RST_set( i4_fec_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void i4_fec_field_FEC_SW_RST_set( i4_fec_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_FEC_SW_RST_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "i4_fec_field_FEC_SW_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "i4_fec_field_FEC_SW_RST_set", A, value );

    /* (0x00028114 bits 0) field FEC_SW_RST of register PMC_I4_FEC_REG_FEC_CTRL */
    i4_fec_reg_FEC_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   A,
                                   I4_FEC_REG_FEC_CTRL_BIT_FEC_SW_RST_MSK,
                                   I4_FEC_REG_FEC_CTRL_BIT_FEC_SW_RST_OFF,
                                   value);
}

static INLINE UINT32 i4_fec_field_FEC_SW_RST_get( i4_fec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_FEC_SW_RST_get( i4_fec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_FEC_SW_RST_get", A, 2);
    /* (0x00028114 bits 0) field FEC_SW_RST of register PMC_I4_FEC_REG_FEC_CTRL */
    reg_value = i4_fec_reg_FEC_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & I4_FEC_REG_FEC_CTRL_BIT_FEC_SW_RST_MSK) >> I4_FEC_REG_FEC_CTRL_BIT_FEC_SW_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_FEC_SW_RST_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 i4_fec_field_CORRECTED_ZEROES_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_CORRECTED_ZEROES_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_CORRECTED_ZEROES_3_COUNT_get", A, 2);
    /* (0x00028008 bits 31:0) field CORRECTED_ZEROES_3_COUNT of register PMC_I4_FEC_REG_CORRECTED_ZEROES_3_CNT */
    reg_value = i4_fec_reg_CORRECTED_ZEROES_3_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & I4_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_MSK) >> I4_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_CORRECTED_ZEROES_3_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_CORRECTED_ZEROES_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_CORRECTED_ZEROES_3_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_CORRECTED_ZEROES_3_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_CORRECTED_ZEROES_3_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_CORRECTED_ZEROES_3_COUNT_get", stop_bit, 31 );
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
    /* (0x00028008 bits 31:0) field CORRECTED_ZEROES_3_COUNT of register PMC_I4_FEC_REG_CORRECTED_ZEROES_3_CNT */
    reg_value = i4_fec_reg_CORRECTED_ZEROES_3_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & I4_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_MSK)
                  >> I4_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_MSK, I4_FEC_REG_CORRECTED_ZEROES_3_CNT_BIT_CORRECTED_ZEROES_3_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_CORRECTED_ZEROES_3_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_CORRECTED_ZEROES_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_CORRECTED_ZEROES_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_CORRECTED_ZEROES_2_COUNT_get", A, 2);
    /* (0x0002800c bits 31:0) field CORRECTED_ZEROES_2_COUNT of register PMC_I4_FEC_REG_CORRECTED_ZEROES_2_CNT */
    reg_value = i4_fec_reg_CORRECTED_ZEROES_2_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & I4_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_MSK) >> I4_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_CORRECTED_ZEROES_2_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_CORRECTED_ZEROES_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_CORRECTED_ZEROES_2_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_CORRECTED_ZEROES_2_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_CORRECTED_ZEROES_2_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_CORRECTED_ZEROES_2_COUNT_get", stop_bit, 31 );
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
    /* (0x0002800c bits 31:0) field CORRECTED_ZEROES_2_COUNT of register PMC_I4_FEC_REG_CORRECTED_ZEROES_2_CNT */
    reg_value = i4_fec_reg_CORRECTED_ZEROES_2_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & I4_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_MSK)
                  >> I4_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_MSK, I4_FEC_REG_CORRECTED_ZEROES_2_CNT_BIT_CORRECTED_ZEROES_2_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_CORRECTED_ZEROES_2_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_CORRECTED_ZEROES_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_CORRECTED_ZEROES_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_CORRECTED_ZEROES_1_COUNT_get", A, 2);
    /* (0x00028010 bits 31:0) field CORRECTED_ZEROES_1_COUNT of register PMC_I4_FEC_REG_CORRECTED_ZEROES_1_CNT */
    reg_value = i4_fec_reg_CORRECTED_ZEROES_1_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & I4_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_MSK) >> I4_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_CORRECTED_ZEROES_1_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_CORRECTED_ZEROES_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_CORRECTED_ZEROES_1_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_CORRECTED_ZEROES_1_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_CORRECTED_ZEROES_1_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_CORRECTED_ZEROES_1_COUNT_get", stop_bit, 31 );
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
    /* (0x00028010 bits 31:0) field CORRECTED_ZEROES_1_COUNT of register PMC_I4_FEC_REG_CORRECTED_ZEROES_1_CNT */
    reg_value = i4_fec_reg_CORRECTED_ZEROES_1_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & I4_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_MSK)
                  >> I4_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_MSK, I4_FEC_REG_CORRECTED_ZEROES_1_CNT_BIT_CORRECTED_ZEROES_1_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_CORRECTED_ZEROES_1_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_CORRECTED_ZEROES_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_CORRECTED_ZEROES_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_CORRECTED_ZEROES_0_COUNT_get", A, 2);
    /* (0x00028014 bits 31:0) field CORRECTED_ZEROES_0_COUNT of register PMC_I4_FEC_REG_CORRECTED_ZEROES_0_CNT */
    reg_value = i4_fec_reg_CORRECTED_ZEROES_0_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & I4_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_MSK) >> I4_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_CORRECTED_ZEROES_0_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_CORRECTED_ZEROES_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_CORRECTED_ZEROES_0_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_CORRECTED_ZEROES_0_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_CORRECTED_ZEROES_0_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_CORRECTED_ZEROES_0_COUNT_get", stop_bit, 31 );
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
    /* (0x00028014 bits 31:0) field CORRECTED_ZEROES_0_COUNT of register PMC_I4_FEC_REG_CORRECTED_ZEROES_0_CNT */
    reg_value = i4_fec_reg_CORRECTED_ZEROES_0_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & I4_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_MSK)
                  >> I4_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_MSK, I4_FEC_REG_CORRECTED_ZEROES_0_CNT_BIT_CORRECTED_ZEROES_0_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_CORRECTED_ZEROES_0_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_CORRECTED_ONES_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_CORRECTED_ONES_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_CORRECTED_ONES_3_COUNT_get", A, 2);
    /* (0x00028018 bits 31:0) field CORRECTED_ONES_3_COUNT of register PMC_I4_FEC_REG_CORRECTED_ONES_3_CNT */
    reg_value = i4_fec_reg_CORRECTED_ONES_3_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & I4_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_MSK) >> I4_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_CORRECTED_ONES_3_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_CORRECTED_ONES_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_CORRECTED_ONES_3_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_CORRECTED_ONES_3_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_CORRECTED_ONES_3_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_CORRECTED_ONES_3_COUNT_get", stop_bit, 31 );
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
    /* (0x00028018 bits 31:0) field CORRECTED_ONES_3_COUNT of register PMC_I4_FEC_REG_CORRECTED_ONES_3_CNT */
    reg_value = i4_fec_reg_CORRECTED_ONES_3_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & I4_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_MSK)
                  >> I4_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_MSK, I4_FEC_REG_CORRECTED_ONES_3_CNT_BIT_CORRECTED_ONES_3_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_CORRECTED_ONES_3_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_CORRECTED_ONES_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_CORRECTED_ONES_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_CORRECTED_ONES_2_COUNT_get", A, 2);
    /* (0x0002801c bits 31:0) field CORRECTED_ONES_2_COUNT of register PMC_I4_FEC_REG_CORRECTED_ONES_2_CNT */
    reg_value = i4_fec_reg_CORRECTED_ONES_2_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & I4_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_MSK) >> I4_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_CORRECTED_ONES_2_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_CORRECTED_ONES_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_CORRECTED_ONES_2_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_CORRECTED_ONES_2_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_CORRECTED_ONES_2_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_CORRECTED_ONES_2_COUNT_get", stop_bit, 31 );
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
    /* (0x0002801c bits 31:0) field CORRECTED_ONES_2_COUNT of register PMC_I4_FEC_REG_CORRECTED_ONES_2_CNT */
    reg_value = i4_fec_reg_CORRECTED_ONES_2_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & I4_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_MSK)
                  >> I4_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_MSK, I4_FEC_REG_CORRECTED_ONES_2_CNT_BIT_CORRECTED_ONES_2_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_CORRECTED_ONES_2_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_CORRECTED_ONES_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_CORRECTED_ONES_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_CORRECTED_ONES_1_COUNT_get", A, 2);
    /* (0x00028020 bits 31:0) field CORRECTED_ONES_1_COUNT of register PMC_I4_FEC_REG_CORRECTED_ONES_1_CNT */
    reg_value = i4_fec_reg_CORRECTED_ONES_1_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & I4_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_MSK) >> I4_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_CORRECTED_ONES_1_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_CORRECTED_ONES_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_CORRECTED_ONES_1_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_CORRECTED_ONES_1_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_CORRECTED_ONES_1_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_CORRECTED_ONES_1_COUNT_get", stop_bit, 31 );
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
    /* (0x00028020 bits 31:0) field CORRECTED_ONES_1_COUNT of register PMC_I4_FEC_REG_CORRECTED_ONES_1_CNT */
    reg_value = i4_fec_reg_CORRECTED_ONES_1_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & I4_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_MSK)
                  >> I4_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_MSK, I4_FEC_REG_CORRECTED_ONES_1_CNT_BIT_CORRECTED_ONES_1_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_CORRECTED_ONES_1_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_CORRECTED_ONES_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_CORRECTED_ONES_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_CORRECTED_ONES_0_COUNT_get", A, 2);
    /* (0x00028024 bits 31:0) field CORRECTED_ONES_0_COUNT of register PMC_I4_FEC_REG_CORRECTED_ONES_0_CNT */
    reg_value = i4_fec_reg_CORRECTED_ONES_0_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & I4_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_MSK) >> I4_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_CORRECTED_ONES_0_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_CORRECTED_ONES_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_CORRECTED_ONES_0_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_CORRECTED_ONES_0_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_CORRECTED_ONES_0_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_CORRECTED_ONES_0_COUNT_get", stop_bit, 31 );
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
    /* (0x00028024 bits 31:0) field CORRECTED_ONES_0_COUNT of register PMC_I4_FEC_REG_CORRECTED_ONES_0_CNT */
    reg_value = i4_fec_reg_CORRECTED_ONES_0_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & I4_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_MSK)
                  >> I4_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_MSK, I4_FEC_REG_CORRECTED_ONES_0_CNT_BIT_CORRECTED_ONES_0_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_CORRECTED_ONES_0_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_UNCORR_ERROR_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_UNCORR_ERROR_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_UNCORR_ERROR_3_COUNT_get", A, 2);
    /* (0x00028028 bits 31:0) field UNCORR_ERROR_3_COUNT of register PMC_I4_FEC_REG_UNCORRECTED_ERRS_3_CNT */
    reg_value = i4_fec_reg_UNCORRECTED_ERRS_3_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_MSK) >> I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_UNCORR_ERROR_3_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_UNCORR_ERROR_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_UNCORR_ERROR_3_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_UNCORR_ERROR_3_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_UNCORR_ERROR_3_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_UNCORR_ERROR_3_COUNT_get", stop_bit, 31 );
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
    /* (0x00028028 bits 31:0) field UNCORR_ERROR_3_COUNT of register PMC_I4_FEC_REG_UNCORRECTED_ERRS_3_CNT */
    reg_value = i4_fec_reg_UNCORRECTED_ERRS_3_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_MSK)
                  >> I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_MSK, I4_FEC_REG_UNCORRECTED_ERRS_3_CNT_BIT_UNCORR_ERROR_3_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_UNCORR_ERROR_3_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_UNCORR_ERROR_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_UNCORR_ERROR_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_UNCORR_ERROR_2_COUNT_get", A, 2);
    /* (0x0002802c bits 31:0) field UNCORR_ERROR_2_COUNT of register PMC_I4_FEC_REG_UNCORRECTED_ERRS_2_CNT */
    reg_value = i4_fec_reg_UNCORRECTED_ERRS_2_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_MSK) >> I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_UNCORR_ERROR_2_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_UNCORR_ERROR_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_UNCORR_ERROR_2_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_UNCORR_ERROR_2_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_UNCORR_ERROR_2_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_UNCORR_ERROR_2_COUNT_get", stop_bit, 31 );
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
    /* (0x0002802c bits 31:0) field UNCORR_ERROR_2_COUNT of register PMC_I4_FEC_REG_UNCORRECTED_ERRS_2_CNT */
    reg_value = i4_fec_reg_UNCORRECTED_ERRS_2_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_MSK)
                  >> I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_MSK, I4_FEC_REG_UNCORRECTED_ERRS_2_CNT_BIT_UNCORR_ERROR_2_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_UNCORR_ERROR_2_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_UNCORR_ERROR_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_UNCORR_ERROR_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_UNCORR_ERROR_1_COUNT_get", A, 2);
    /* (0x00028030 bits 31:0) field UNCORR_ERROR_1_COUNT of register PMC_I4_FEC_REG_UNCORRECTED_ERRS_1_CNT */
    reg_value = i4_fec_reg_UNCORRECTED_ERRS_1_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_MSK) >> I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_UNCORR_ERROR_1_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_UNCORR_ERROR_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_UNCORR_ERROR_1_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_UNCORR_ERROR_1_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_UNCORR_ERROR_1_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_UNCORR_ERROR_1_COUNT_get", stop_bit, 31 );
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
    /* (0x00028030 bits 31:0) field UNCORR_ERROR_1_COUNT of register PMC_I4_FEC_REG_UNCORRECTED_ERRS_1_CNT */
    reg_value = i4_fec_reg_UNCORRECTED_ERRS_1_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_MSK)
                  >> I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_MSK, I4_FEC_REG_UNCORRECTED_ERRS_1_CNT_BIT_UNCORR_ERROR_1_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_UNCORR_ERROR_1_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_UNCORR_ERROR_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_UNCORR_ERROR_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_UNCORR_ERROR_0_COUNT_get", A, 2);
    /* (0x00028034 bits 31:0) field UNCORR_ERROR_0_COUNT of register PMC_I4_FEC_REG_UNCORRECTED_ERRS_0_CNT */
    reg_value = i4_fec_reg_UNCORRECTED_ERRS_0_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    value = (reg_value & I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_MSK) >> I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_UNCORR_ERROR_0_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_UNCORR_ERROR_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_UNCORR_ERROR_0_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_UNCORR_ERROR_0_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_UNCORR_ERROR_0_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_UNCORR_ERROR_0_COUNT_get", stop_bit, 31 );
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
    /* (0x00028034 bits 31:0) field UNCORR_ERROR_0_COUNT of register PMC_I4_FEC_REG_UNCORRECTED_ERRS_0_CNT */
    reg_value = i4_fec_reg_UNCORRECTED_ERRS_0_CNT_read( b_ptr,
                                                        h_ptr,
                                                        A);
    field_value = (reg_value & I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_MSK)
                  >> I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_MSK, I4_FEC_REG_UNCORRECTED_ERRS_0_CNT_BIT_UNCORR_ERROR_0_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_UNCORR_ERROR_0_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_RX_FRM_PULSE_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_RX_FRM_PULSE_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_RX_FRM_PULSE_3_COUNT_get", A, 2);
    /* (0x00028038 bits 31:0) field RX_FRM_PULSE_3_COUNT of register PMC_I4_FEC_REG_RX_FRAME_PULSE_3_CNT */
    reg_value = i4_fec_reg_RX_FRAME_PULSE_3_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & I4_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_MSK) >> I4_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_RX_FRM_PULSE_3_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_RX_FRM_PULSE_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_RX_FRM_PULSE_3_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_RX_FRM_PULSE_3_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_RX_FRM_PULSE_3_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_RX_FRM_PULSE_3_COUNT_get", stop_bit, 31 );
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
    /* (0x00028038 bits 31:0) field RX_FRM_PULSE_3_COUNT of register PMC_I4_FEC_REG_RX_FRAME_PULSE_3_CNT */
    reg_value = i4_fec_reg_RX_FRAME_PULSE_3_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & I4_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_MSK)
                  >> I4_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_MSK, I4_FEC_REG_RX_FRAME_PULSE_3_CNT_BIT_RX_FRM_PULSE_3_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_RX_FRM_PULSE_3_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_RX_FRM_PULSE_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_RX_FRM_PULSE_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_RX_FRM_PULSE_2_COUNT_get", A, 2);
    /* (0x0002803c bits 31:0) field RX_FRM_PULSE_2_COUNT of register PMC_I4_FEC_REG_RX_FRAME_PULSE_2_CNT */
    reg_value = i4_fec_reg_RX_FRAME_PULSE_2_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & I4_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_MSK) >> I4_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_RX_FRM_PULSE_2_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_RX_FRM_PULSE_2_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_RX_FRM_PULSE_2_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_RX_FRM_PULSE_2_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_RX_FRM_PULSE_2_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_RX_FRM_PULSE_2_COUNT_get", stop_bit, 31 );
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
    /* (0x0002803c bits 31:0) field RX_FRM_PULSE_2_COUNT of register PMC_I4_FEC_REG_RX_FRAME_PULSE_2_CNT */
    reg_value = i4_fec_reg_RX_FRAME_PULSE_2_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & I4_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_MSK)
                  >> I4_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_MSK, I4_FEC_REG_RX_FRAME_PULSE_2_CNT_BIT_RX_FRM_PULSE_2_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_RX_FRM_PULSE_2_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_RX_FRM_PULSE_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_RX_FRM_PULSE_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_RX_FRM_PULSE_1_COUNT_get", A, 2);
    /* (0x00028040 bits 31:0) field RX_FRM_PULSE_1_COUNT of register PMC_I4_FEC_REG_RX_FRAME_PULSE_1_CNT */
    reg_value = i4_fec_reg_RX_FRAME_PULSE_1_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & I4_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_MSK) >> I4_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_RX_FRM_PULSE_1_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_RX_FRM_PULSE_1_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_RX_FRM_PULSE_1_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_RX_FRM_PULSE_1_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_RX_FRM_PULSE_1_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_RX_FRM_PULSE_1_COUNT_get", stop_bit, 31 );
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
    /* (0x00028040 bits 31:0) field RX_FRM_PULSE_1_COUNT of register PMC_I4_FEC_REG_RX_FRAME_PULSE_1_CNT */
    reg_value = i4_fec_reg_RX_FRAME_PULSE_1_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & I4_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_MSK)
                  >> I4_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_MSK, I4_FEC_REG_RX_FRAME_PULSE_1_CNT_BIT_RX_FRM_PULSE_1_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_RX_FRM_PULSE_1_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_RX_FRM_PULSE_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_RX_FRM_PULSE_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_RX_FRM_PULSE_0_COUNT_get", A, 2);
    /* (0x00028044 bits 31:0) field RX_FRM_PULSE_0_COUNT of register PMC_I4_FEC_REG_RX_FRAME_PULSE_0_CNT */
    reg_value = i4_fec_reg_RX_FRAME_PULSE_0_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & I4_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_MSK) >> I4_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_RX_FRM_PULSE_0_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_RX_FRM_PULSE_0_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_RX_FRM_PULSE_0_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_RX_FRM_PULSE_0_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_RX_FRM_PULSE_0_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_RX_FRM_PULSE_0_COUNT_get", stop_bit, 31 );
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
    /* (0x00028044 bits 31:0) field RX_FRM_PULSE_0_COUNT of register PMC_I4_FEC_REG_RX_FRAME_PULSE_0_CNT */
    reg_value = i4_fec_reg_RX_FRAME_PULSE_0_CNT_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & I4_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_MSK)
                  >> I4_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_MSK, I4_FEC_REG_RX_FRAME_PULSE_0_CNT_BIT_RX_FRM_PULSE_0_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_RX_FRM_PULSE_0_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_LLSA_UNCORR_ERROR_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_LLSA_UNCORR_ERROR_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_LLSA_UNCORR_ERROR_3_COUNT_get", A, 2);
    /* (0x000280c8 bits 31:0) field LLSA_UNCORR_ERROR_3_COUNT of register PMC_I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT */
    reg_value = i4_fec_reg_LLSA_UNCORRECTED_ERRS_3_CNT_read( b_ptr,
                                                             h_ptr,
                                                             A);
    value = (reg_value & I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_BIT_LLSA_UNCORR_ERROR_3_COUNT_MSK) >> I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_BIT_LLSA_UNCORR_ERROR_3_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_LLSA_UNCORR_ERROR_3_COUNT_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_LLSA_UNCORR_ERROR_3_COUNT_get( i4_fec_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_LLSA_UNCORR_ERROR_3_COUNT_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_LLSA_UNCORR_ERROR_3_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_LLSA_UNCORR_ERROR_3_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_LLSA_UNCORR_ERROR_3_COUNT_get", stop_bit, 31 );
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
    /* (0x000280c8 bits 31:0) field LLSA_UNCORR_ERROR_3_COUNT of register PMC_I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT */
    reg_value = i4_fec_reg_LLSA_UNCORRECTED_ERRS_3_CNT_read( b_ptr,
                                                             h_ptr,
                                                             A);
    field_value = (reg_value & I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_BIT_LLSA_UNCORR_ERROR_3_COUNT_MSK)
                  >> I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_BIT_LLSA_UNCORR_ERROR_3_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_BIT_LLSA_UNCORR_ERROR_3_COUNT_MSK, I4_FEC_REG_LLSA_UNCORRECTED_ERRS_3_CNT_BIT_LLSA_UNCORR_ERROR_3_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_LLSA_UNCORR_ERROR_3_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 i4_fec_field_LLSA_STATS_OTU_FAIL_get( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_LLSA_STATS_OTU_FAIL_get( i4_fec_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_LLSA_STATS_OTU_FAIL_get", A, 2);
    /* (0x0002810c bits 3:0) field LLSA_STATS_OTU_FAIL of register PMC_I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT */
    reg_value = i4_fec_reg_LLSA_STATISTICS_FAILURE_STAT_read( b_ptr,
                                                              h_ptr,
                                                              A);
    value = (reg_value & I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_MSK) >> I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "i4_fec_field_LLSA_STATS_OTU_FAIL_get", A, value );

    return value;
}
static INLINE UINT32 i4_fec_field_range_LLSA_STATS_OTU_FAIL_get( i4_fec_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 i4_fec_field_range_LLSA_STATS_OTU_FAIL_get( i4_fec_buffer_t *b_ptr,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_field_range_LLSA_STATS_OTU_FAIL_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "i4_fec_field_range_LLSA_STATS_OTU_FAIL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "i4_fec_field_range_LLSA_STATS_OTU_FAIL_get", stop_bit, 3 );
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
    /* (0x0002810c bits 3:0) field LLSA_STATS_OTU_FAIL of register PMC_I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT */
    reg_value = i4_fec_reg_LLSA_STATISTICS_FAILURE_STAT_read( b_ptr,
                                                              h_ptr,
                                                              A);
    field_value = (reg_value & I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_MSK)
                  >> I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_MSK, I4_FEC_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "i4_fec_field_range_LLSA_STATS_OTU_FAIL_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_burstsize16
 * ==================================================================================
 */
static INLINE void i4_fec_lfield_LLSA_STATS_LANE_CORR_0S_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void i4_fec_lfield_LLSA_STATS_LANE_CORR_0S_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value[16] )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_lfield_LLSA_STATS_LANE_CORR_0S_get", A, 2);
    IOLOG( "%s  A=%d", "i4_fec_lfield_LLSA_STATS_LANE_CORR_0S_get", A);
    i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_burst_read( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  0,
                                                                  16,
                                                                  (UINT32 *)value);
}

static INLINE UINT32 i4_fec_lfield_range_LLSA_STATS_LANE_CORR_0S_get( i4_fec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 i4_fec_lfield_range_LLSA_STATS_LANE_CORR_0S_get( i4_fec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_lfield_range_LLSA_STATS_LANE_CORR_0S_get", A, 2);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "i4_fec_lfield_range_LLSA_STATS_LANE_CORR_0S_get", start_bit, stop_bit );
    }
    reg_value = i4_fec_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_array_read( b_ptr,
                                                                        h_ptr,
                                                                        A,
                                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d value=0x%08x", "i4_fec_lfield_range_LLSA_STATS_LANE_CORR_0S_get", A, start_bit, stop_bit, value );
    return value;
}

static INLINE void i4_fec_lfield_LLSA_STATS_LANE_CORR_1S_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value[16] ) ALWAYS_INLINE;
static INLINE void i4_fec_lfield_LLSA_STATS_LANE_CORR_1S_get( i4_fec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value[16] )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_lfield_LLSA_STATS_LANE_CORR_1S_get", A, 2);
    IOLOG( "%s  A=%d", "i4_fec_lfield_LLSA_STATS_LANE_CORR_1S_get", A);
    i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_burst_read( b_ptr,
                                                                  h_ptr,
                                                                  A,
                                                                  0,
                                                                  16,
                                                                  (UINT32 *)value);
}

static INLINE UINT32 i4_fec_lfield_range_LLSA_STATS_LANE_CORR_1S_get( i4_fec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 i4_fec_lfield_range_LLSA_STATS_LANE_CORR_1S_get( i4_fec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "i4_fec_lfield_range_LLSA_STATS_LANE_CORR_1S_get", A, 2);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "i4_fec_lfield_range_LLSA_STATS_LANE_CORR_1S_get", start_bit, stop_bit );
    }
    reg_value = i4_fec_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_array_read( b_ptr,
                                                                        h_ptr,
                                                                        A,
                                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d value=0x%08x", "i4_fec_lfield_range_LLSA_STATS_LANE_CORR_1S_get", A, start_bit, stop_bit, value );
    return value;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _I4_FEC_IO_INLINE_H */
