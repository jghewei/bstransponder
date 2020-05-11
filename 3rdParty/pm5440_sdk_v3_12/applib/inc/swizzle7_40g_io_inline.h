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
 *     and register accessor functions for the swizzle7_40g block
 *****************************************************************************/
#ifndef _SWIZZLE7_40G_IO_INLINE_H
#define _SWIZZLE7_40G_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "swizzle7_40g_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SWIZZLE7_40G_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for swizzle7_40g
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
} swizzle7_40g_buffer_t;
static INLINE void swizzle7_40g_buffer_init( swizzle7_40g_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 A ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_buffer_init( swizzle7_40g_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x2000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "swizzle7_40g_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void swizzle7_40g_buffer_flush( swizzle7_40g_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_buffer_flush( swizzle7_40g_buffer_t *b_ptr )
{
    IOLOG( "swizzle7_40g_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 swizzle7_40g_reg_read( swizzle7_40g_buffer_t *b_ptr,
                                            lineotn_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_read( swizzle7_40g_buffer_t *b_ptr,
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
static INLINE void swizzle7_40g_reg_write( swizzle7_40g_buffer_t *b_ptr,
                                           lineotn_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_write( swizzle7_40g_buffer_t *b_ptr,
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

static INLINE void swizzle7_40g_field_set( swizzle7_40g_buffer_t *b_ptr,
                                           lineotn_handle_t *h_ptr,
                                           UINT32 A,
                                           UINT32 mem_type,
                                           UINT32 reg,
                                           UINT32 mask,
                                           UINT32 unused_mask,
                                           UINT32 ofs,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_set( swizzle7_40g_buffer_t *b_ptr,
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

static INLINE void swizzle7_40g_action_on_write_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mem_type,
                                                           UINT32 reg,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_action_on_write_field_set( swizzle7_40g_buffer_t *b_ptr,
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

static INLINE void swizzle7_40g_burst_read( swizzle7_40g_buffer_t *b_ptr,
                                            lineotn_handle_t *h_ptr,
                                            UINT32 A,
                                            UINT32 mem_type,
                                            UINT32 reg,
                                            UINT32 len,
                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_burst_read( swizzle7_40g_buffer_t *b_ptr,
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

static INLINE void swizzle7_40g_burst_write( swizzle7_40g_buffer_t *b_ptr,
                                             lineotn_handle_t *h_ptr,
                                             UINT32 A,
                                             UINT32 mem_type,
                                             UINT32 reg,
                                             UINT32 len,
                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_burst_write( swizzle7_40g_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE swizzle7_40g_poll( swizzle7_40g_buffer_t *b_ptr,
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
static INLINE PMC_POLL_RETURN_TYPE swizzle7_40g_poll( swizzle7_40g_buffer_t *b_ptr,
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
 *  register access functions for swizzle7_40g
 * ==================================================================================
 */

static INLINE void swizzle7_40g_reg_DECODER_CFG_write( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_DECODER_CFG_write( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_DECODER_CFG_write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_DECODER_CFG,
                            value);
}

static INLINE void swizzle7_40g_reg_DECODER_CFG_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_DECODER_CFG_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_DECODER_CFG_field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_DECODER_CFG,
                            mask,
                            PMC_SWIZZLE7_40G_REG_DECODER_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_DECODER_CFG_read( swizzle7_40g_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_DECODER_CFG_read( swizzle7_40g_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_DECODER_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_DECODER_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_write( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_write( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2,
                            value);
}

static INLINE void swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2,
                            mask,
                            PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( swizzle7_40g_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( swizzle7_40g_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_write( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_write( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2,
                            value);
}

static INLINE void swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2,
                            mask,
                            PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_read( swizzle7_40g_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_read( swizzle7_40g_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__write( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__write( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_,
                            value);
}

static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_,
                            mask,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( swizzle7_40g_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( swizzle7_40g_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__write( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__write( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_,
                            value);
}

static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_,
                            mask,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read( swizzle7_40g_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read( swizzle7_40g_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__write( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__write( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_,
                            value);
}

static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_,
                            mask,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( swizzle7_40g_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( swizzle7_40g_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__write( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__write( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_,
                            value);
}

static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_,
                            mask,
                            PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__read( swizzle7_40g_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__read( swizzle7_40g_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_FEC_CTRL_write( swizzle7_40g_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_FEC_CTRL_write( swizzle7_40g_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_FEC_CTRL_write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_FEC_CTRL,
                            value);
}

static INLINE void swizzle7_40g_reg_FEC_CTRL_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_FEC_CTRL_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_FEC_CTRL_field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_FEC_CTRL,
                            mask,
                            PMC_SWIZZLE7_40G_REG_FEC_CTRL_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_FEC_CTRL_read( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_FEC_CTRL_read( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_FEC_CTRL);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_FEC_CTRL_read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_write( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_write( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG,
                            value);
}

static INLINE void swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG,
                            mask,
                            PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_read( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_read( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_write( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_write( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_write", value );
    swizzle7_40g_reg_write( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG,
                            value);
}

static INLINE void swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_field_set( swizzle7_40g_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_field_set", A, mask, ofs, value );
    swizzle7_40g_field_set( b_ptr,
                            h_ptr,
                            A,
                            MEM_TYPE_CONFIG,
                            PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG,
                            mask,
                            PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_UNUSED_MASK,
                            ofs,
                            value);

}

static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_read( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_read( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_CONFIG,
                                       PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                     lineotn_handle_t *h_ptr,
                                                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_1S_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_1S_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_1S_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_0S_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_0S_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_0S_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_FAILURE_STAT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_FAILURE_STAT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_FAILURE_STAT);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LLSA_STATISTICS_FAILURE_STAT_read", reg_value, A);
    return reg_value;
}

static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                      lineotn_handle_t *h_ptr,
                                                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                      lineotn_handle_t *h_ptr,
                                                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT);

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_burst_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                                         lineotn_handle_t *h_ptr,
                                                                                                         UINT32 A,
                                                                                                         UINT32 ofs,
                                                                                                         UINT32 len,
                                                                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_burst_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                                         lineotn_handle_t *h_ptr,
                                                                                                         UINT32 A,
                                                                                                         UINT32 ofs,
                                                                                                         UINT32 len,
                                                                                                         UINT32 *value )
{
    if (len + ofs > 20)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_burst_read", ofs, len, 20 );
    swizzle7_40g_burst_read( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words A=%d ofs=%d len=%d", "swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_burst_read", 20, A, ofs, len);
}

static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                                     lineotn_handle_t *h_ptr,
                                                                                                     UINT32 A,
                                                                                                     UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                                     lineotn_handle_t *h_ptr,
                                                                                                     UINT32 A,
                                                                                                     UINT32 N )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19(N));

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_read", reg_value, A);
    return reg_value;
}

static INLINE void swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_burst_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                                         lineotn_handle_t *h_ptr,
                                                                                                         UINT32 A,
                                                                                                         UINT32 ofs,
                                                                                                         UINT32 len,
                                                                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_burst_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                                         lineotn_handle_t *h_ptr,
                                                                                                         UINT32 A,
                                                                                                         UINT32 ofs,
                                                                                                         UINT32 len,
                                                                                                         UINT32 *value )
{
    if (len + ofs > 20)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_burst_read", ofs, len, 20 );
    swizzle7_40g_burst_read( b_ptr,
                             h_ptr,
                             A,
                             MEM_TYPE_STATUS,
                             PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19(ofs),
                             len,
                             value);

    IOLOG( "%s of %d words A=%d ofs=%d len=%d", "swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_burst_read", 20, A, ofs, len);
}

static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                                     lineotn_handle_t *h_ptr,
                                                                                                     UINT32 A,
                                                                                                     UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_read( swizzle7_40g_buffer_t *b_ptr,
                                                                                                     lineotn_handle_t *h_ptr,
                                                                                                     UINT32 A,
                                                                                                     UINT32 N )
{
    UINT32 reg_value;

    reg_value = swizzle7_40g_reg_read( b_ptr,
                                       h_ptr,
                                       A,
                                       MEM_TYPE_STATUS,
                                       PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19(N));

    IOLOG( "%s -> 0x%08x; A=%d", "swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_read", reg_value, A);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void swizzle7_40g_field_LIMIT_LATENCY_set( swizzle7_40g_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_LIMIT_LATENCY_set( swizzle7_40g_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LIMIT_LATENCY_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_LIMIT_LATENCY_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_LIMIT_LATENCY_set", A, value );

    /* (0x00020000 bits 7:1) bits 0:6 use field LIMIT_LATENCY of register PMC_SWIZZLE7_40G_REG_DECODER_CFG */
    swizzle7_40g_reg_DECODER_CFG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_MSK,
                                            SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_OFF,
                                            value);
}

static INLINE UINT32 swizzle7_40g_field_LIMIT_LATENCY_get( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LIMIT_LATENCY_get( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LIMIT_LATENCY_get", A, 2);
    /* (0x00020000 bits 7:1) bits 0:6 use field LIMIT_LATENCY of register PMC_SWIZZLE7_40G_REG_DECODER_CFG */
    reg_value = swizzle7_40g_reg_DECODER_CFG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_MSK) >> SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LIMIT_LATENCY_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_LIMIT_LATENCY_set( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_LIMIT_LATENCY_set( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LIMIT_LATENCY_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LIMIT_LATENCY_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LIMIT_LATENCY_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LIMIT_LATENCY_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020000 bits 7:1) bits 0:6 use field LIMIT_LATENCY of register PMC_SWIZZLE7_40G_REG_DECODER_CFG */
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
        /* (0x00020000 bits 7:1) bits 0:6 use field LIMIT_LATENCY of register PMC_SWIZZLE7_40G_REG_DECODER_CFG */
        swizzle7_40g_reg_DECODER_CFG_field_set( b_ptr,
                                                h_ptr,
                                                A,
                                                subfield_mask << (SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_OFF + subfield_offset),
                                                SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_LIMIT_LATENCY_get( swizzle7_40g_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_LIMIT_LATENCY_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LIMIT_LATENCY_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LIMIT_LATENCY_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LIMIT_LATENCY_get", stop_bit, 6 );
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
    /* (0x00020000 bits 7:1) bits 0:6 use field LIMIT_LATENCY of register PMC_SWIZZLE7_40G_REG_DECODER_CFG */
    reg_value = swizzle7_40g_reg_DECODER_CFG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    field_value = (reg_value & SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_MSK)
                  >> SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_MSK, SWIZZLE7_40G_REG_DECODER_CFG_BIT_LIMIT_LATENCY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LIMIT_LATENCY_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_DEC_DISABLE_set( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_DEC_DISABLE_set( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_DEC_DISABLE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_DEC_DISABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_DEC_DISABLE_set", A, value );

    /* (0x00020000 bits 0) field DEC_DISABLE of register PMC_SWIZZLE7_40G_REG_DECODER_CFG */
    swizzle7_40g_reg_DECODER_CFG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            SWIZZLE7_40G_REG_DECODER_CFG_BIT_DEC_DISABLE_MSK,
                                            SWIZZLE7_40G_REG_DECODER_CFG_BIT_DEC_DISABLE_OFF,
                                            value);
}

static INLINE UINT32 swizzle7_40g_field_DEC_DISABLE_get( swizzle7_40g_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_DEC_DISABLE_get( swizzle7_40g_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_DEC_DISABLE_get", A, 2);
    /* (0x00020000 bits 0) field DEC_DISABLE of register PMC_SWIZZLE7_40G_REG_DECODER_CFG */
    reg_value = swizzle7_40g_reg_DECODER_CFG_read( b_ptr,
                                                   h_ptr,
                                                   A);
    value = (reg_value & SWIZZLE7_40G_REG_DECODER_CFG_BIT_DEC_DISABLE_MSK) >> SWIZZLE7_40G_REG_DECODER_CFG_BIT_DEC_DISABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_DEC_DISABLE_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_FRAME_COMMIT_SEL_set( swizzle7_40g_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_FRAME_COMMIT_SEL_set( swizzle7_40g_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_FRAME_COMMIT_SEL_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_FRAME_COMMIT_SEL_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_FRAME_COMMIT_SEL_set", A, value );

    /* (0x00020020 bits 16) field FRAME_COMMIT_SEL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_FRAME_COMMIT_SEL_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_FRAME_COMMIT_SEL_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_FRAME_COMMIT_SEL_get( swizzle7_40g_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_FRAME_COMMIT_SEL_get( swizzle7_40g_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_FRAME_COMMIT_SEL_get", A, 2);
    /* (0x00020020 bits 16) field FRAME_COMMIT_SEL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_FRAME_COMMIT_SEL_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_FRAME_COMMIT_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_FRAME_COMMIT_SEL_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_LONG_TRIGGER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_LONG_TRIGGER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LONG_TRIGGER_MODE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_LONG_TRIGGER_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_LONG_TRIGGER_MODE_set", A, value );

    /* (0x00020020 bits 15) field LONG_TRIGGER_MODE of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_LONG_TRIGGER_MODE_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_LONG_TRIGGER_MODE_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_LONG_TRIGGER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LONG_TRIGGER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LONG_TRIGGER_MODE_get", A, 2);
    /* (0x00020020 bits 15) field LONG_TRIGGER_MODE of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_LONG_TRIGGER_MODE_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_LONG_TRIGGER_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LONG_TRIGGER_MODE_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_TRAP_ENABLE_set( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_TRAP_ENABLE_set( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_TRAP_ENABLE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_TRAP_ENABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_TRAP_ENABLE_set", A, value );

    /* (0x00020020 bits 14) field TRAP_ENABLE of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_ENABLE_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_ENABLE_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_TRAP_ENABLE_get( swizzle7_40g_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_TRAP_ENABLE_get( swizzle7_40g_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_TRAP_ENABLE_get", A, 2);
    /* (0x00020020 bits 14) field TRAP_ENABLE of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_ENABLE_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_TRAP_ENABLE_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_TRAP_TYPE_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_TRAP_TYPE_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_TRAP_TYPE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_TRAP_TYPE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_TRAP_TYPE_set", A, value );

    /* (0x00020020 bits 13) field TRAP_TYPE of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_TYPE_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_TYPE_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_TRAP_TYPE_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_TRAP_TYPE_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_TRAP_TYPE_get", A, 2);
    /* (0x00020020 bits 13) field TRAP_TYPE of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_TYPE_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_TRAP_TYPE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_TRAP_TYPE_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_PDF_CAPTURE_ENABLE_set( swizzle7_40g_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_PDF_CAPTURE_ENABLE_set( swizzle7_40g_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_PDF_CAPTURE_ENABLE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_PDF_CAPTURE_ENABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_PDF_CAPTURE_ENABLE_set", A, value );

    /* (0x00020020 bits 12) field PDF_CAPTURE_ENABLE of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_PDF_CAPTURE_ENABLE_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_PDF_CAPTURE_ENABLE_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_PDF_CAPTURE_ENABLE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_PDF_CAPTURE_ENABLE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_PDF_CAPTURE_ENABLE_get", A, 2);
    /* (0x00020020 bits 12) field PDF_CAPTURE_ENABLE of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_PDF_CAPTURE_ENABLE_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_PDF_CAPTURE_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_PDF_CAPTURE_ENABLE_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_STAT_SEL_set( swizzle7_40g_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_STAT_SEL_set( swizzle7_40g_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A,
                                                    UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_STAT_SEL_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_STAT_SEL_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_STAT_SEL_set", A, value );

    /* (0x00020020 bits 11:10) bits 0:1 use field STAT_SEL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_STAT_SEL_get( swizzle7_40g_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_STAT_SEL_get( swizzle7_40g_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_STAT_SEL_get", A, 2);
    /* (0x00020020 bits 11:10) bits 0:1 use field STAT_SEL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_STAT_SEL_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_STAT_SEL_set( swizzle7_40g_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_STAT_SEL_set( swizzle7_40g_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_STAT_SEL_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_STAT_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_STAT_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_STAT_SEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020020 bits 11:10) bits 0:1 use field STAT_SEL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
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
        /* (0x00020020 bits 11:10) bits 0:1 use field STAT_SEL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
        swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_OFF + subfield_offset),
                                                         SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_STAT_SEL_get( swizzle7_40g_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_STAT_SEL_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_STAT_SEL_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_STAT_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_STAT_SEL_get", stop_bit, 1 );
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
    /* (0x00020020 bits 11:10) bits 0:1 use field STAT_SEL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_MSK)
                  >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_MSK, SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STAT_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_STAT_SEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_DISABLE_PRIORITY_set( swizzle7_40g_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_DISABLE_PRIORITY_set( swizzle7_40g_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_DISABLE_PRIORITY_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_DISABLE_PRIORITY_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_DISABLE_PRIORITY_set", A, value );

    /* (0x00020020 bits 9) field DISABLE_PRIORITY of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DISABLE_PRIORITY_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DISABLE_PRIORITY_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_DISABLE_PRIORITY_get( swizzle7_40g_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_DISABLE_PRIORITY_get( swizzle7_40g_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_DISABLE_PRIORITY_get", A, 2);
    /* (0x00020020 bits 9) field DISABLE_PRIORITY of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DISABLE_PRIORITY_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DISABLE_PRIORITY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_DISABLE_PRIORITY_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_STATISTICS_FRAMES_set( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_STATISTICS_FRAMES_set( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_STATISTICS_FRAMES_set", A, 2);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_STATISTICS_FRAMES_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_STATISTICS_FRAMES_set", A, value );

    /* (0x00020020 bits 8:1) bits 0:7 use field STATISTICS_FRAMES of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_STATISTICS_FRAMES_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_STATISTICS_FRAMES_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_STATISTICS_FRAMES_get", A, 2);
    /* (0x00020020 bits 8:1) bits 0:7 use field STATISTICS_FRAMES of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_STATISTICS_FRAMES_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_STATISTICS_FRAMES_set( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_STATISTICS_FRAMES_set( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_STATISTICS_FRAMES_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_STATISTICS_FRAMES_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_STATISTICS_FRAMES_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_STATISTICS_FRAMES_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020020 bits 8:1) bits 0:7 use field STATISTICS_FRAMES of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
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
        /* (0x00020020 bits 8:1) bits 0:7 use field STATISTICS_FRAMES of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
        swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_OFF + subfield_offset),
                                                         SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_STATISTICS_FRAMES_get( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_STATISTICS_FRAMES_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_STATISTICS_FRAMES_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_STATISTICS_FRAMES_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_STATISTICS_FRAMES_get", stop_bit, 7 );
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
    /* (0x00020020 bits 8:1) bits 0:7 use field STATISTICS_FRAMES of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_MSK)
                  >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_MSK, SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_STATISTICS_FRAMES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_STATISTICS_FRAMES_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_DEC_DISABLE_RESYNC_set( swizzle7_40g_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_DEC_DISABLE_RESYNC_set( swizzle7_40g_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_DEC_DISABLE_RESYNC_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_DEC_DISABLE_RESYNC_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_DEC_DISABLE_RESYNC_set", A, value );

    /* (0x00020020 bits 0) field DEC_DISABLE_RESYNC of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DEC_DISABLE_RESYNC_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DEC_DISABLE_RESYNC_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_DEC_DISABLE_RESYNC_get( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_DEC_DISABLE_RESYNC_get( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_DEC_DISABLE_RESYNC_get", A, 2);
    /* (0x00020020 bits 0) field DEC_DISABLE_RESYNC of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DEC_DISABLE_RESYNC_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_1_2_BIT_DEC_DISABLE_RESYNC_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_DEC_DISABLE_RESYNC_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_REPROCESS_CONTROL_set( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_REPROCESS_CONTROL_set( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_REPROCESS_CONTROL_set", A, 2);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_REPROCESS_CONTROL_set", value, 65535);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_REPROCESS_CONTROL_set", A, value );

    /* (0x00020024 bits 15:0) bits 0:15 use field REPROCESS_CONTROL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2 */
    swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_MSK,
                                                     SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_OFF,
                                                     value);
}

static INLINE UINT32 swizzle7_40g_field_REPROCESS_CONTROL_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_REPROCESS_CONTROL_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_REPROCESS_CONTROL_get", A, 2);
    /* (0x00020024 bits 15:0) bits 0:15 use field REPROCESS_CONTROL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_MSK) >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_REPROCESS_CONTROL_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_REPROCESS_CONTROL_set( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_REPROCESS_CONTROL_set( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit,
                                                                   UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_REPROCESS_CONTROL_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_REPROCESS_CONTROL_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_REPROCESS_CONTROL_set", stop_bit, 15 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_REPROCESS_CONTROL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020024 bits 15:0) bits 0:15 use field REPROCESS_CONTROL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2 */
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
        /* (0x00020024 bits 15:0) bits 0:15 use field REPROCESS_CONTROL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2 */
        swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_OFF + subfield_offset),
                                                         SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_REPROCESS_CONTROL_get( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_REPROCESS_CONTROL_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_REPROCESS_CONTROL_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_REPROCESS_CONTROL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_REPROCESS_CONTROL_get", stop_bit, 15 );
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
    /* (0x00020024 bits 15:0) bits 0:15 use field REPROCESS_CONTROL of register PMC_SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2 */
    reg_value = swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_read( b_ptr,
                                                            h_ptr,
                                                            A);
    field_value = (reg_value & SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_MSK)
                  >> SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_MSK, SWIZZLE7_40G_REG_DIAG_DECODER_CFG_2_2_BIT_REPROCESS_CONTROL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_REPROCESS_CONTROL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T6_3_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T6_3_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_3_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T6_3_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_3_set", A, value );

    /* (0x00020080 bits 30:24) bits 0:6 use field CONF_T6_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T6_3_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T6_3_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_3_get", A, 2);
    /* (0x00020080 bits 30:24) bits 0:6 use field CONF_T6_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_3_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T6_3_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T6_3_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_3_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_3_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_3_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_3_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020080 bits 30:24) bits 0:6 use field CONF_T6_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
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
        /* (0x00020080 bits 30:24) bits 0:6 use field CONF_T6_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_3_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_3_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_3_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_3_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_3_get", stop_bit, 6 );
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
    /* (0x00020080 bits 30:24) bits 0:6 use field CONF_T6_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_3_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T6_2_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T6_2_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_2_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T6_2_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_2_set", A, value );

    /* (0x00020080 bits 22:16) bits 0:6 use field CONF_T6_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T6_2_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T6_2_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_2_get", A, 2);
    /* (0x00020080 bits 22:16) bits 0:6 use field CONF_T6_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_2_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T6_2_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T6_2_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_2_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_2_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_2_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_2_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020080 bits 22:16) bits 0:6 use field CONF_T6_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
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
        /* (0x00020080 bits 22:16) bits 0:6 use field CONF_T6_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_2_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_2_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_2_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_2_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_2_get", stop_bit, 6 );
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
    /* (0x00020080 bits 22:16) bits 0:6 use field CONF_T6_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T6_1_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T6_1_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_1_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T6_1_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_1_set", A, value );

    /* (0x00020080 bits 14:8) bits 0:6 use field CONF_T6_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T6_1_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T6_1_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_1_get", A, 2);
    /* (0x00020080 bits 14:8) bits 0:6 use field CONF_T6_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_1_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T6_1_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T6_1_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_1_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_1_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_1_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_1_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020080 bits 14:8) bits 0:6 use field CONF_T6_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
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
        /* (0x00020080 bits 14:8) bits 0:6 use field CONF_T6_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_1_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_1_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_1_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_1_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_1_get", stop_bit, 6 );
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
    /* (0x00020080 bits 14:8) bits 0:6 use field CONF_T6_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T6_0_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T6_0_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_0_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T6_0_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_0_set", A, value );

    /* (0x00020080 bits 6:0) bits 0:6 use field CONF_T6_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T6_0_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T6_0_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_0_get", A, 2);
    /* (0x00020080 bits 6:0) bits 0:6 use field CONF_T6_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_0_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T6_0_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T6_0_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_0_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_0_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_0_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020080 bits 6:0) bits 0:6 use field CONF_T6_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
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
        /* (0x00020080 bits 6:0) bits 0:6 use field CONF_T6_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_0_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_0_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_0_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_0_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_0_get", stop_bit, 6 );
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
    /* (0x00020080 bits 6:0) bits 0:6 use field CONF_T6_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_1_2_BIT_CONF_T6_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T6_6_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T6_6_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_6_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T6_6_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_6_set", A, value );

    /* (0x00020084 bits 22:16) bits 0:6 use field CONF_T6_6 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T6_6_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T6_6_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_6_get", A, 2);
    /* (0x00020084 bits 22:16) bits 0:6 use field CONF_T6_6 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_6_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T6_6_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T6_6_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_6_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_6_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_6_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_6_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020084 bits 22:16) bits 0:6 use field CONF_T6_6 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
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
        /* (0x00020084 bits 22:16) bits 0:6 use field CONF_T6_6 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_6_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_6_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_6_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_6_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_6_get", stop_bit, 6 );
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
    /* (0x00020084 bits 22:16) bits 0:6 use field CONF_T6_6 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_6_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T6_5_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T6_5_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_5_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T6_5_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_5_set", A, value );

    /* (0x00020084 bits 14:8) bits 0:6 use field CONF_T6_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T6_5_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T6_5_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_5_get", A, 2);
    /* (0x00020084 bits 14:8) bits 0:6 use field CONF_T6_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_5_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T6_5_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T6_5_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_5_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_5_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_5_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_5_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020084 bits 14:8) bits 0:6 use field CONF_T6_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
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
        /* (0x00020084 bits 14:8) bits 0:6 use field CONF_T6_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_5_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_5_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_5_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_5_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_5_get", stop_bit, 6 );
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
    /* (0x00020084 bits 14:8) bits 0:6 use field CONF_T6_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_5_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T6_4_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T6_4_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_4_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T6_4_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_4_set", A, value );

    /* (0x00020084 bits 6:0) bits 0:6 use field CONF_T6_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T6_4_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T6_4_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T6_4_get", A, 2);
    /* (0x00020084 bits 6:0) bits 0:6 use field CONF_T6_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T6_4_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T6_4_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T6_4_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_4_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_4_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_4_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_4_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020084 bits 6:0) bits 0:6 use field CONF_T6_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
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
        /* (0x00020084 bits 6:0) bits 0:6 use field CONF_T6_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_4_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T6_4_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T6_4_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T6_4_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T6_4_get", stop_bit, 6 );
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
    /* (0x00020084 bits 6:0) bits 0:6 use field CONF_T6_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T6_CODEWORDS_2_2_BIT_CONF_T6_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T6_4_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T5_3_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T5_3_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_3_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T5_3_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_3_set", A, value );

    /* (0x00020088 bits 30:24) bits 0:6 use field CONF_T5_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T5_3_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T5_3_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_3_get", A, 2);
    /* (0x00020088 bits 30:24) bits 0:6 use field CONF_T5_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_3_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T5_3_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T5_3_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_3_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_3_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_3_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_3_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020088 bits 30:24) bits 0:6 use field CONF_T5_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
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
        /* (0x00020088 bits 30:24) bits 0:6 use field CONF_T5_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_3_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_3_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_3_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_3_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_3_get", stop_bit, 6 );
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
    /* (0x00020088 bits 30:24) bits 0:6 use field CONF_T5_3 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_3_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T5_2_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T5_2_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_2_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T5_2_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_2_set", A, value );

    /* (0x00020088 bits 22:16) bits 0:6 use field CONF_T5_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T5_2_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T5_2_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_2_get", A, 2);
    /* (0x00020088 bits 22:16) bits 0:6 use field CONF_T5_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_2_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T5_2_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T5_2_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_2_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_2_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_2_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_2_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020088 bits 22:16) bits 0:6 use field CONF_T5_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
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
        /* (0x00020088 bits 22:16) bits 0:6 use field CONF_T5_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_2_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_2_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_2_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_2_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_2_get", stop_bit, 6 );
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
    /* (0x00020088 bits 22:16) bits 0:6 use field CONF_T5_2 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_2_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T5_1_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T5_1_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_1_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T5_1_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_1_set", A, value );

    /* (0x00020088 bits 14:8) bits 0:6 use field CONF_T5_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T5_1_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T5_1_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_1_get", A, 2);
    /* (0x00020088 bits 14:8) bits 0:6 use field CONF_T5_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_1_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T5_1_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T5_1_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_1_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_1_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_1_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_1_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020088 bits 14:8) bits 0:6 use field CONF_T5_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
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
        /* (0x00020088 bits 14:8) bits 0:6 use field CONF_T5_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_1_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_1_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_1_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_1_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_1_get", stop_bit, 6 );
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
    /* (0x00020088 bits 14:8) bits 0:6 use field CONF_T5_1 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_1_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T5_0_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T5_0_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_0_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T5_0_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_0_set", A, value );

    /* (0x00020088 bits 6:0) bits 0:6 use field CONF_T5_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T5_0_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T5_0_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_0_get", A, 2);
    /* (0x00020088 bits 6:0) bits 0:6 use field CONF_T5_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_0_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T5_0_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T5_0_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_0_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_0_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_0_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_0_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020088 bits 6:0) bits 0:6 use field CONF_T5_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
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
        /* (0x00020088 bits 6:0) bits 0:6 use field CONF_T5_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_0_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_0_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_0_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_0_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_0_get", stop_bit, 6 );
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
    /* (0x00020088 bits 6:0) bits 0:6 use field CONF_T5_0 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_1_2_BIT_CONF_T5_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_0_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T5_5_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T5_5_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_5_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T5_5_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_5_set", A, value );

    /* (0x0002008c bits 14:8) bits 0:6 use field CONF_T5_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T5_5_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T5_5_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_5_get", A, 2);
    /* (0x0002008c bits 14:8) bits 0:6 use field CONF_T5_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_5_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T5_5_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T5_5_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_5_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_5_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_5_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_5_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002008c bits 14:8) bits 0:6 use field CONF_T5_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
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
        /* (0x0002008c bits 14:8) bits 0:6 use field CONF_T5_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_5_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_5_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_5_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_5_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_5_get", stop_bit, 6 );
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
    /* (0x0002008c bits 14:8) bits 0:6 use field CONF_T5_5 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_5_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_CONF_T5_4_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_CONF_T5_4_set( swizzle7_40g_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_4_set", A, 2);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_CONF_T5_4_set", value, 127);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_4_set", A, value );

    /* (0x0002008c bits 6:0) bits 0:6 use field CONF_T5_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
    swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__field_set( b_ptr,
                                                                 h_ptr,
                                                                 A,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_MSK,
                                                                 SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_OFF,
                                                                 value);
}

static INLINE UINT32 swizzle7_40g_field_CONF_T5_4_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_CONF_T5_4_get( swizzle7_40g_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_CONF_T5_4_get", A, 2);
    /* (0x0002008c bits 6:0) bits 0:6 use field CONF_T5_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_MSK) >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_CONF_T5_4_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_CONF_T5_4_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_CONF_T5_4_set( swizzle7_40g_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_4_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_4_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_4_set", stop_bit, 6 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_4_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0002008c bits 6:0) bits 0:6 use field CONF_T5_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
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
        /* (0x0002008c bits 6:0) bits 0:6 use field CONF_T5_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
        swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__field_set( b_ptr,
                                                                     h_ptr,
                                                                     A,
                                                                     subfield_mask << (SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_OFF + subfield_offset),
                                                                     SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_4_get( swizzle7_40g_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_CONF_T5_4_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_CONF_T5_4_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_CONF_T5_4_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_CONF_T5_4_get", stop_bit, 6 );
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
    /* (0x0002008c bits 6:0) bits 0:6 use field CONF_T5_4 of register PMC_SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_ */
    reg_value = swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__read( b_ptr,
                                                                        h_ptr,
                                                                        A);
    field_value = (reg_value & SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_MSK)
                  >> SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_MSK, SWIZZLE7_40G_REG_CONFIDENCE_FOR_T5_CODEWORDS_2_2_BIT_CONF_T5_4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_CONF_T5_4_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_FEC_RAM_LOWPWR_set( swizzle7_40g_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_FEC_RAM_LOWPWR_set( swizzle7_40g_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_FEC_RAM_LOWPWR_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_FEC_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_FEC_RAM_LOWPWR_set", A, value );

    /* (0x000200f0 bits 4) field FEC_RAM_LOWPWR of register PMC_SWIZZLE7_40G_REG_FEC_CTRL */
    swizzle7_40g_reg_FEC_CTRL_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_MSK,
                                         SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 swizzle7_40g_field_FEC_RAM_LOWPWR_get( swizzle7_40g_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_FEC_RAM_LOWPWR_get( swizzle7_40g_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_FEC_RAM_LOWPWR_get", A, 2);
    /* (0x000200f0 bits 4) field FEC_RAM_LOWPWR of register PMC_SWIZZLE7_40G_REG_FEC_CTRL */
    reg_value = swizzle7_40g_reg_FEC_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_MSK) >> SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_RAM_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_FEC_RAM_LOWPWR_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_FEC_LOWPWR_set( swizzle7_40g_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_FEC_LOWPWR_set( swizzle7_40g_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_FEC_LOWPWR_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_FEC_LOWPWR_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_FEC_LOWPWR_set", A, value );

    /* (0x000200f0 bits 1) field FEC_LOWPWR of register PMC_SWIZZLE7_40G_REG_FEC_CTRL */
    swizzle7_40g_reg_FEC_CTRL_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_LOWPWR_MSK,
                                         SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_LOWPWR_OFF,
                                         value);
}

static INLINE UINT32 swizzle7_40g_field_FEC_LOWPWR_get( swizzle7_40g_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_FEC_LOWPWR_get( swizzle7_40g_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_FEC_LOWPWR_get", A, 2);
    /* (0x000200f0 bits 1) field FEC_LOWPWR of register PMC_SWIZZLE7_40G_REG_FEC_CTRL */
    reg_value = swizzle7_40g_reg_FEC_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_LOWPWR_MSK) >> SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_LOWPWR_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_FEC_LOWPWR_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_FEC_SW_RST_set( swizzle7_40g_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_FEC_SW_RST_set( swizzle7_40g_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_FEC_SW_RST_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_FEC_SW_RST_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_FEC_SW_RST_set", A, value );

    /* (0x000200f0 bits 0) field FEC_SW_RST of register PMC_SWIZZLE7_40G_REG_FEC_CTRL */
    swizzle7_40g_reg_FEC_CTRL_field_set( b_ptr,
                                         h_ptr,
                                         A,
                                         SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_SW_RST_MSK,
                                         SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_SW_RST_OFF,
                                         value);
}

static INLINE UINT32 swizzle7_40g_field_FEC_SW_RST_get( swizzle7_40g_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_FEC_SW_RST_get( swizzle7_40g_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_FEC_SW_RST_get", A, 2);
    /* (0x000200f0 bits 0) field FEC_SW_RST of register PMC_SWIZZLE7_40G_REG_FEC_CTRL */
    reg_value = swizzle7_40g_reg_FEC_CTRL_read(  b_ptr, h_ptr, A);
    value = (reg_value & SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_SW_RST_MSK) >> SWIZZLE7_40G_REG_FEC_CTRL_BIT_FEC_SW_RST_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_FEC_SW_RST_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_LPA_STATS_COUNTER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_LPA_STATS_COUNTER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LPA_STATS_COUNTER_MODE_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_LPA_STATS_COUNTER_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_LPA_STATS_COUNTER_MODE_set", A, value );

    /* (0x00020180 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG */
    swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK,
                                                        SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF,
                                                        value);
}

static INLINE UINT32 swizzle7_40g_field_LPA_STATS_COUNTER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_COUNTER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LPA_STATS_COUNTER_MODE_get", A, 2);
    /* (0x00020180 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK) >> SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LPA_STATS_COUNTER_MODE_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00020180 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG */
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
        /* (0x00020180 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG */
        swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF + subfield_offset),
                                                            SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_get", stop_bit, 1 );
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
    /* (0x00020180 bits 1:0) bits 0:1 use field LPA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK)
                  >> SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_MSK, SWIZZLE7_40G_REG_LPA_STATISTICS_MODE_CFG_BIT_LPA_STATS_COUNTER_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set", A, value );

    /* (0x000201a0 bits 2) field LLSA_STATS_LATCH_SOURCE of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG */
    swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_LATCH_SOURCE_MSK,
                                                         SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_LATCH_SOURCE_OFF,
                                                         value);
}

static INLINE UINT32 swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_get", A, 2);
    /* (0x000201a0 bits 2) field LLSA_STATS_LATCH_SOURCE of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG */
    reg_value = swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_LATCH_SOURCE_MSK) >> SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_LATCH_SOURCE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_set", A, value );

    /* (0x000201a0 bits 1:0) bits 0:1 use field LLSA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG */
    swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_MSK,
                                                         SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_OFF,
                                                         value);
}

static INLINE UINT32 swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_get", A, 2);
    /* (0x000201a0 bits 1:0) bits 0:1 use field LLSA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG */
    reg_value = swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_read( b_ptr,
                                                                h_ptr,
                                                                A);
    value = (reg_value & SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_MSK) >> SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_get", A, value );

    return value;
}
static INLINE void swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_set( swizzle7_40g_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000201a0 bits 1:0) bits 0:1 use field LLSA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG */
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
        /* (0x000201a0 bits 1:0) bits 0:1 use field LLSA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG */
        swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             subfield_mask << (SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_OFF + subfield_offset),
                                                             SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_OFF + subfield_offset,
                                                             value >> subfield_shift);
    }
}

static INLINE UINT32 swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_get", stop_bit, 1 );
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
    /* (0x000201a0 bits 1:0) bits 0:1 use field LLSA_STATS_COUNTER_MODE of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG */
    reg_value = swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_read( b_ptr,
                                                                h_ptr,
                                                                A);
    field_value = (reg_value & SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_MSK)
                  >> SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_MSK, SWIZZLE7_40G_REG_LLSA_STATISTICS_MODE_CFG_BIT_LLSA_STATS_COUNTER_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_FP_get( swizzle7_40g_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_FP_get( swizzle7_40g_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LPA_STATS_FP_get", A, 2);
    /* (0x00020190 bits 31:0) bits 0:31 use field LPA_STATS_FP of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_BIT_LPA_STATS_FP_MSK) >> SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_BIT_LPA_STATS_FP_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LPA_STATS_FP_get", A, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_FP_get( swizzle7_40g_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_FP_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LPA_STATS_FP_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LPA_STATS_FP_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LPA_STATS_FP_get", stop_bit, 31 );
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
    /* (0x00020190 bits 31:0) bits 0:31 use field LPA_STATS_FP of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_read( b_ptr,
                                                                          h_ptr,
                                                                          A);
    field_value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_BIT_LPA_STATS_FP_MSK)
                  >> SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_BIT_LPA_STATS_FP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_BIT_LPA_STATS_FP_MSK, SWIZZLE7_40G_REG_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_BIT_LPA_STATS_FP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LPA_STATS_FP_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_UNCORR_CWDS_get( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_UNCORR_CWDS_get( swizzle7_40g_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LPA_STATS_UNCORR_CWDS_get", A, 2);
    /* (0x00020194 bits 31:0) bits 0:31 use field LPA_STATS_UNCORR_CWDS of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read( b_ptr,
                                                                                h_ptr,
                                                                                A);
    value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LPA_STATS_UNCORR_CWDS_MSK) >> SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LPA_STATS_UNCORR_CWDS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LPA_STATS_UNCORR_CWDS_get", A, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_UNCORR_CWDS_get( swizzle7_40g_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_UNCORR_CWDS_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LPA_STATS_UNCORR_CWDS_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LPA_STATS_UNCORR_CWDS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LPA_STATS_UNCORR_CWDS_get", stop_bit, 31 );
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
    /* (0x00020194 bits 31:0) bits 0:31 use field LPA_STATS_UNCORR_CWDS of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read( b_ptr,
                                                                                h_ptr,
                                                                                A);
    field_value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LPA_STATS_UNCORR_CWDS_MSK)
                  >> SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LPA_STATS_UNCORR_CWDS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LPA_STATS_UNCORR_CWDS_MSK, SWIZZLE7_40G_REG_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LPA_STATS_UNCORR_CWDS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LPA_STATS_UNCORR_CWDS_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_CORR_1S_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_CORR_1S_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LPA_STATS_CORR_1S_get", A, 2);
    /* (0x00020198 bits 31:0) bits 0:31 use field LPA_STATS_CORR_1S of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_1S_CNT_read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_BIT_LPA_STATS_CORR_1S_MSK) >> SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_BIT_LPA_STATS_CORR_1S_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LPA_STATS_CORR_1S_get", A, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_CORR_1S_get( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_CORR_1S_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LPA_STATS_CORR_1S_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LPA_STATS_CORR_1S_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LPA_STATS_CORR_1S_get", stop_bit, 31 );
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
    /* (0x00020198 bits 31:0) bits 0:31 use field LPA_STATS_CORR_1S of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_1S_CNT_read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    field_value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_BIT_LPA_STATS_CORR_1S_MSK)
                  >> SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_BIT_LPA_STATS_CORR_1S_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_BIT_LPA_STATS_CORR_1S_MSK, SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_1S_CNT_BIT_LPA_STATS_CORR_1S_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LPA_STATS_CORR_1S_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_CORR_0S_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LPA_STATS_CORR_0S_get( swizzle7_40g_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LPA_STATS_CORR_0S_get", A, 2);
    /* (0x0002019c bits 31:0) bits 0:31 use field LPA_STATS_CORR_0S of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_0S_CNT_read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_BIT_LPA_STATS_CORR_0S_MSK) >> SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_BIT_LPA_STATS_CORR_0S_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LPA_STATS_CORR_0S_get", A, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_CORR_0S_get( swizzle7_40g_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_LPA_STATS_CORR_0S_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LPA_STATS_CORR_0S_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LPA_STATS_CORR_0S_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LPA_STATS_CORR_0S_get", stop_bit, 31 );
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
    /* (0x0002019c bits 31:0) bits 0:31 use field LPA_STATS_CORR_0S of register PMC_SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT */
    reg_value = swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_0S_CNT_read( b_ptr,
                                                                       h_ptr,
                                                                       A);
    field_value = (reg_value & SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_BIT_LPA_STATS_CORR_0S_MSK)
                  >> SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_BIT_LPA_STATS_CORR_0S_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_BIT_LPA_STATS_CORR_0S_MSK, SWIZZLE7_40G_REG_LPA_STATISTICS_CORRECTED_0S_CNT_BIT_LPA_STATS_CORR_0S_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LPA_STATS_CORR_0S_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_LLSA_STATS_OTU_FAIL_get( swizzle7_40g_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LLSA_STATS_OTU_FAIL_get( swizzle7_40g_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LLSA_STATS_OTU_FAIL_get", A, 2);
    /* (0x00020200 bits 0) field LLSA_STATS_OTU_FAIL of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_FAILURE_STAT */
    reg_value = swizzle7_40g_reg_LLSA_STATISTICS_FAILURE_STAT_read( b_ptr,
                                                                    h_ptr,
                                                                    A);
    value = (reg_value & SWIZZLE7_40G_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_MSK) >> SWIZZLE7_40G_REG_LLSA_STATISTICS_FAILURE_STAT_BIT_LLSA_STATS_OTU_FAIL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LLSA_STATS_OTU_FAIL_get", A, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_LLSA_STATS_UNCORR_CWDS_get( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_LLSA_STATS_UNCORR_CWDS_get( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_LLSA_STATS_UNCORR_CWDS_get", A, 2);
    /* (0x0002021c bits 31:0) bits 0:31 use field LLSA_STATS_UNCORR_CWDS of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT */
    reg_value = swizzle7_40g_reg_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    value = (reg_value & SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LLSA_STATS_UNCORR_CWDS_MSK) >> SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LLSA_STATS_UNCORR_CWDS_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "swizzle7_40g_field_LLSA_STATS_UNCORR_CWDS_get", A, value );

    return value;
}
static INLINE UINT32 swizzle7_40g_field_range_LLSA_STATS_UNCORR_CWDS_get( swizzle7_40g_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_field_range_LLSA_STATS_UNCORR_CWDS_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_field_range_LLSA_STATS_UNCORR_CWDS_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "swizzle7_40g_field_range_LLSA_STATS_UNCORR_CWDS_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "swizzle7_40g_field_range_LLSA_STATS_UNCORR_CWDS_get", stop_bit, 31 );
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
    /* (0x0002021c bits 31:0) bits 0:31 use field LLSA_STATS_UNCORR_CWDS of register PMC_SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT */
    reg_value = swizzle7_40g_reg_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read( b_ptr,
                                                                                 h_ptr,
                                                                                 A);
    field_value = (reg_value & SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LLSA_STATS_UNCORR_CWDS_MSK)
                  >> SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LLSA_STATS_UNCORR_CWDS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LLSA_STATS_UNCORR_CWDS_MSK, SWIZZLE7_40G_REG_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_BIT_LLSA_STATS_UNCORR_CWDS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "swizzle7_40g_field_range_LLSA_STATS_UNCORR_CWDS_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_burstsize20
 * ==================================================================================
 */
static INLINE void swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_1S_get( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value[20] ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_1S_get( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value[20] )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_1S_get", A, 2);
    IOLOG( "%s  A=%d", "swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_1S_get", A);
    swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_burst_read( b_ptr,
                                                                                          h_ptr,
                                                                                          A,
                                                                                          0,
                                                                                          20,
                                                                                          (UINT32 *)value);
}

static INLINE UINT32 swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_1S_get( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_1S_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_1S_get", A, 2);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_1S_get", start_bit, stop_bit );
    }
    reg_value = swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_read( b_ptr,
                                                                                                h_ptr,
                                                                                                A,
                                                                                                word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d value=0x%08x", "swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_1S_get", A, start_bit, stop_bit, value );
    return value;
}

static INLINE void swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_0S_get( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value[20] ) ALWAYS_INLINE;
static INLINE void swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_0S_get( swizzle7_40g_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value[20] )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_0S_get", A, 2);
    IOLOG( "%s  A=%d", "swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_0S_get", A);
    swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_burst_read( b_ptr,
                                                                                          h_ptr,
                                                                                          A,
                                                                                          0,
                                                                                          20,
                                                                                          (UINT32 *)value);
}

static INLINE UINT32 swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_0S_get( swizzle7_40g_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_0S_get( swizzle7_40g_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s A is %d but max is %d", "swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_0S_get", A, 2);
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_0S_get", start_bit, stop_bit );
    }
    reg_value = swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_read( b_ptr,
                                                                                                h_ptr,
                                                                                                A,
                                                                                                word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d value=0x%08x", "swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_0S_get", A, start_bit, stop_bit, value );
    return value;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SWIZZLE7_40G_IO_INLINE_H */
