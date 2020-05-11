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
 *     and register accessor functions for the mpmo_dsp block
 *****************************************************************************/
#ifndef _MPMO_DSP_IO_INLINE_H
#define _MPMO_DSP_IO_INLINE_H

#include "mpmo_loc.h"
#include "mpmo_dsp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MPMO_DSP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mpmo_dsp
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
    mpmo_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} mpmo_dsp_buffer_t;
static INLINE void mpmo_dsp_buffer_init( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_buffer_init( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mpmo_dsp_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mpmo_dsp_buffer_flush( mpmo_dsp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_buffer_flush( mpmo_dsp_buffer_t *b_ptr )
{
    IOLOG( "mpmo_dsp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mpmo_dsp_reg_read( mpmo_dsp_buffer_t *b_ptr,
                                        mpmo_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_read( mpmo_dsp_buffer_t *b_ptr,
                                        mpmo_handle_t *h_ptr,
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
static INLINE void mpmo_dsp_reg_write( mpmo_dsp_buffer_t *b_ptr,
                                       mpmo_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_write( mpmo_dsp_buffer_t *b_ptr,
                                       mpmo_handle_t *h_ptr,
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

static INLINE void mpmo_dsp_field_set( mpmo_dsp_buffer_t *b_ptr,
                                       mpmo_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_set( mpmo_dsp_buffer_t *b_ptr,
                                       mpmo_handle_t *h_ptr,
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

static INLINE void mpmo_dsp_action_on_write_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_action_on_write_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
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

static INLINE void mpmo_dsp_burst_read( mpmo_dsp_buffer_t *b_ptr,
                                        mpmo_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_burst_read( mpmo_dsp_buffer_t *b_ptr,
                                        mpmo_handle_t *h_ptr,
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

static INLINE void mpmo_dsp_burst_write( mpmo_dsp_buffer_t *b_ptr,
                                         mpmo_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_burst_write( mpmo_dsp_buffer_t *b_ptr,
                                         mpmo_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mpmo_dsp_poll( mpmo_dsp_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_dsp_poll( mpmo_dsp_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
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
 *  register access functions for mpmo_dsp
 * ==================================================================================
 */

static INLINE void mpmo_dsp_reg_COMMON_CFG_REG_write( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_COMMON_CFG_REG_write( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_dsp_reg_COMMON_CFG_REG_write", value );
    mpmo_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_COMMON_CFG_REG,
                        value);
}

static INLINE void mpmo_dsp_reg_COMMON_CFG_REG_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_COMMON_CFG_REG_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_dsp_reg_COMMON_CFG_REG_field_set", mask, ofs, value );
    mpmo_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_COMMON_CFG_REG,
                        mask,
                        PMC_MPMO_DSP_REG_COMMON_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpmo_dsp_reg_COMMON_CFG_REG_read( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_COMMON_CFG_REG_read( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMO_DSP_REG_COMMON_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "mpmo_dsp_reg_COMMON_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                            mpmo_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                            mpmo_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_write", value );
    mpmo_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG(N),
                        value);
}

static INLINE void mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                                mpmo_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                                mpmo_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_field_set", N, mask, ofs, value );
    mpmo_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG(N),
                        mask,
                        PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                             mpmo_handle_t *h_ptr,
                                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                             mpmo_handle_t *h_ptr,
                                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                              mpmo_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                              mpmo_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_write", value );
    mpmo_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG(N),
                        value);
}

static INLINE void mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                                  mpmo_handle_t *h_ptr,
                                                                                  UINT32  N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                                  mpmo_handle_t *h_ptr,
                                                                                  UINT32  N,
                                                                                  UINT32 mask,
                                                                                  UINT32 ofs,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_field_set", N, mask, ofs, value );
    mpmo_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG(N),
                        mask,
                        PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                               mpmo_handle_t *h_ptr,
                                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                               mpmo_handle_t *h_ptr,
                                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_dsp_reg_GENERAL_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_GENERAL_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_dsp_reg_GENERAL_CFG_REG_array_write", value );
    mpmo_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_GENERAL_CFG_REG(N),
                        value);
}

static INLINE void mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                 mpmo_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                 mpmo_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set", N, mask, ofs, value );
    mpmo_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_GENERAL_CFG_REG(N),
                        mask,
                        PMC_MPMO_DSP_REG_GENERAL_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpmo_dsp_reg_GENERAL_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_GENERAL_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMO_DSP_REG_GENERAL_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_dsp_reg_GENERAL_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_write", value );
    mpmo_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG(N),
                        value);
}

static INLINE void mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set", N, mask, ofs, value );
    mpmo_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG(N),
                        mask,
                        PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_write", value );
    mpmo_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG(N),
                        value);
}

static INLINE void mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                           mpmo_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                           mpmo_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_field_set", N, mask, ofs, value );
    mpmo_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG(N),
                        mask,
                        PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                        mpmo_handle_t *h_ptr,
                                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                        mpmo_handle_t *h_ptr,
                                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_write", value );
    mpmo_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG(N),
                        value);
}

static INLINE void mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                             mpmo_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                             mpmo_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_field_set", N, mask, ofs, value );
    mpmo_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG(N),
                        mask,
                        PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_write( mpmo_dsp_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_write", value );
    mpmo_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG(N),
                        value);
}

static INLINE void mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_field_set( mpmo_dsp_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_field_set", N, mask, ofs, value );
    mpmo_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG(N),
                        mask,
                        PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mpmo_dsp_reg_DEBUG684_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_reg_DEBUG684_REG_array_read( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_MPMO_DSP_REG_DEBUG684_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_dsp_reg_DEBUG684_REG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mpmo_dsp_field_CN_REQ_MODE_set( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_CN_REQ_MODE_set( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_CN_REQ_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_dsp_field_CN_REQ_MODE_set", value );

    /* (0x00006000 bits 0) field CN_REQ_MODE of register PMC_MPMO_DSP_REG_COMMON_CFG_REG */
    mpmo_dsp_reg_COMMON_CFG_REG_field_set( b_ptr,
                                           h_ptr,
                                           MPMO_DSP_REG_COMMON_CFG_REG_BIT_CN_REQ_MODE_MSK,
                                           MPMO_DSP_REG_COMMON_CFG_REG_BIT_CN_REQ_MODE_OFF,
                                           value);
}

static INLINE UINT32 mpmo_dsp_field_CN_REQ_MODE_get( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_CN_REQ_MODE_get( mpmo_dsp_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00006000 bits 0) field CN_REQ_MODE of register PMC_MPMO_DSP_REG_COMMON_CFG_REG */
    reg_value = mpmo_dsp_reg_COMMON_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MPMO_DSP_REG_COMMON_CFG_REG_BIT_CN_REQ_MODE_MSK) >> MPMO_DSP_REG_COMMON_CFG_REG_BIT_CN_REQ_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_dsp_field_CN_REQ_MODE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size8
 * ==================================================================================
 */
static INLINE void mpmo_dsp_field_FRM_PERIOD_N_set( mpmo_dsp_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_FRM_PERIOD_N_set( mpmo_dsp_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FRM_PERIOD_N_set", N, 7);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_FRM_PERIOD_N_set", N, value );

    /* ((0x00006004 + (N) * 0x8) bits 31:0) bits 0:31 use field FRM_PERIOD_N of register PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG index N=0..7 */
    mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_MSK,
                                                                 MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_OFF,
                                                                 value);
}

static INLINE UINT32 mpmo_dsp_field_FRM_PERIOD_N_get( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_FRM_PERIOD_N_get( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FRM_PERIOD_N_get", N, 7);
    /* ((0x00006004 + (N) * 0x8) bits 31:0) bits 0:31 use field FRM_PERIOD_N of register PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG index N=0..7 */
    reg_value = mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    value = (reg_value & MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_MSK) >> MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_FRM_PERIOD_N_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_FRM_PERIOD_N_set( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_FRM_PERIOD_N_set( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_FRM_PERIOD_N_set", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_FRM_PERIOD_N_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_FRM_PERIOD_N_set", stop_bit, 31 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_FRM_PERIOD_N_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006004 + (N) * 0x8) bits 31:0) bits 0:31 use field FRM_PERIOD_N of register PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG index N=0..7 */
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
        /* ((0x00006004 + (N) * 0x8) bits 31:0) bits 0:31 use field FRM_PERIOD_N of register PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG index N=0..7 */
        mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_field_set( b_ptr,
                                                                     h_ptr,
                                                                     N,
                                                                     subfield_mask << (MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_OFF + subfield_offset),
                                                                     MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_OFF + subfield_offset,
                                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_FRM_PERIOD_N_get( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_FRM_PERIOD_N_get( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_FRM_PERIOD_N_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_FRM_PERIOD_N_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_FRM_PERIOD_N_get", stop_bit, 31 );
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
    /* ((0x00006004 + (N) * 0x8) bits 31:0) bits 0:31 use field FRM_PERIOD_N of register PMC_MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG index N=0..7 */
    reg_value = mpmo_dsp_reg_FRAME_PERIOD_NUMERATOR_CFG_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    field_value = (reg_value & MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_MSK)
                  >> MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_MSK, MPMO_DSP_REG_FRAME_PERIOD_NUMERATOR_CFG_REG_BIT_FRM_PERIOD_N_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_FRM_PERIOD_N_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_FP_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_FP_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FP_ENABLE_set", N, 7);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_FP_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_FP_ENABLE_set", N, value );

    /* ((0x00006008 + (N) * 0x8) bits 31) field FP_ENABLE of register PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG index N=0..7 */
    mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   N,
                                                                   MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FP_ENABLE_MSK,
                                                                   MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FP_ENABLE_OFF,
                                                                   value);
}

static INLINE UINT32 mpmo_dsp_field_FP_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_FP_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FP_ENABLE_get", N, 7);
    /* ((0x00006008 + (N) * 0x8) bits 31) field FP_ENABLE of register PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG index N=0..7 */
    reg_value = mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    value = (reg_value & MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FP_ENABLE_MSK) >> MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FP_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_FP_ENABLE_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_FRM_PERIOD_D_set( mpmo_dsp_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_FRM_PERIOD_D_set( mpmo_dsp_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FRM_PERIOD_D_set", N, 7);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_FRM_PERIOD_D_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_FRM_PERIOD_D_set", N, value );

    /* ((0x00006008 + (N) * 0x8) bits 23:0) bits 0:23 use field FRM_PERIOD_D of register PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG index N=0..7 */
    mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   N,
                                                                   MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_MSK,
                                                                   MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_OFF,
                                                                   value);
}

static INLINE UINT32 mpmo_dsp_field_FRM_PERIOD_D_get( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_FRM_PERIOD_D_get( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FRM_PERIOD_D_get", N, 7);
    /* ((0x00006008 + (N) * 0x8) bits 23:0) bits 0:23 use field FRM_PERIOD_D of register PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG index N=0..7 */
    reg_value = mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    value = (reg_value & MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_MSK) >> MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_FRM_PERIOD_D_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_FRM_PERIOD_D_set( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_FRM_PERIOD_D_set( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_FRM_PERIOD_D_set", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_FRM_PERIOD_D_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_FRM_PERIOD_D_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_FRM_PERIOD_D_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006008 + (N) * 0x8) bits 23:0) bits 0:23 use field FRM_PERIOD_D of register PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG index N=0..7 */
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
        /* ((0x00006008 + (N) * 0x8) bits 23:0) bits 0:23 use field FRM_PERIOD_D of register PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG index N=0..7 */
        mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_field_set( b_ptr,
                                                                       h_ptr,
                                                                       N,
                                                                       subfield_mask << (MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_OFF + subfield_offset),
                                                                       MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_OFF + subfield_offset,
                                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_FRM_PERIOD_D_get( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_FRM_PERIOD_D_get( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
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

    if (N > 7)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_FRM_PERIOD_D_get", N, 7);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_FRM_PERIOD_D_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_FRM_PERIOD_D_get", stop_bit, 23 );
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
    /* ((0x00006008 + (N) * 0x8) bits 23:0) bits 0:23 use field FRM_PERIOD_D of register PMC_MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG index N=0..7 */
    reg_value = mpmo_dsp_reg_FRAME_PERIOD_DENOMINATOR_CFG_REG_array_read( b_ptr,
                                                                          h_ptr,
                                                                          N);
    field_value = (reg_value & MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_MSK)
                  >> MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_MSK, MPMO_DSP_REG_FRAME_PERIOD_DENOMINATOR_CFG_REG_BIT_FRM_PERIOD_D_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_FRM_PERIOD_D_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void mpmo_dsp_field_FRM_CNT_SEL_set( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_FRM_CNT_SEL_set( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FRM_CNT_SEL_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_FRM_CNT_SEL_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_FRM_CNT_SEL_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 29:27) field FRM_CNT_SEL of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_FRM_CNT_SEL_get( mpmo_dsp_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_FRM_CNT_SEL_get( mpmo_dsp_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FRM_CNT_SEL_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 29:27) field FRM_CNT_SEL of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_FRM_CNT_SEL_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_FRM_CNT_SEL_set( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_FRM_CNT_SEL_set( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_FRM_CNT_SEL_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_FRM_CNT_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_FRM_CNT_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_FRM_CNT_SEL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006044 + (N) * 0x14) bits 29:27) field FRM_CNT_SEL of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
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
        /* ((0x00006044 + (N) * 0x14) bits 29:27) field FRM_CNT_SEL of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
        mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_OFF + subfield_offset),
                                                      MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_FRM_CNT_SEL_get( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_FRM_CNT_SEL_get( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_FRM_CNT_SEL_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_FRM_CNT_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_FRM_CNT_SEL_get", stop_bit, 2 );
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
    /* ((0x00006044 + (N) * 0x14) bits 29:27) field FRM_CNT_SEL of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_MSK)
                  >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_MSK, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_FRM_CNT_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_FRM_CNT_SEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_AGE_INT_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_AGE_INT_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_AGE_INT_GAIN_set", N, 95);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_AGE_INT_GAIN_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_AGE_INT_GAIN_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 26:22) field AGE_INT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_AGE_INT_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_AGE_INT_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_AGE_INT_GAIN_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 26:22) field AGE_INT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_AGE_INT_GAIN_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_AGE_INT_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_AGE_INT_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_AGE_INT_GAIN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_AGE_INT_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_AGE_INT_GAIN_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_AGE_INT_GAIN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006044 + (N) * 0x14) bits 26:22) field AGE_INT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
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
        /* ((0x00006044 + (N) * 0x14) bits 26:22) field AGE_INT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
        mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_OFF + subfield_offset),
                                                      MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_AGE_INT_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_AGE_INT_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_AGE_INT_GAIN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_AGE_INT_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_AGE_INT_GAIN_get", stop_bit, 4 );
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
    /* ((0x00006044 + (N) * 0x14) bits 26:22) field AGE_INT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_MSK)
                  >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_MSK, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_INT_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_AGE_INT_GAIN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_AGE_PROP_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_AGE_PROP_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_AGE_PROP_GAIN_set", N, 95);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_AGE_PROP_GAIN_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_AGE_PROP_GAIN_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 21:17) field AGE_PROP_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_AGE_PROP_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_AGE_PROP_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_AGE_PROP_GAIN_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 21:17) field AGE_PROP_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_AGE_PROP_GAIN_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_AGE_PROP_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_AGE_PROP_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_AGE_PROP_GAIN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_AGE_PROP_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_AGE_PROP_GAIN_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_AGE_PROP_GAIN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006044 + (N) * 0x14) bits 21:17) field AGE_PROP_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
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
        /* ((0x00006044 + (N) * 0x14) bits 21:17) field AGE_PROP_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
        mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_OFF + subfield_offset),
                                                      MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_AGE_PROP_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_AGE_PROP_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_AGE_PROP_GAIN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_AGE_PROP_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_AGE_PROP_GAIN_get", stop_bit, 4 );
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
    /* ((0x00006044 + (N) * 0x14) bits 21:17) field AGE_PROP_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_MSK)
                  >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_MSK, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_PROP_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_AGE_PROP_GAIN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_ZONE_CENT_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_ZONE_CENT_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_ZONE_CENT_GAIN_set", N, 95);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_ZONE_CENT_GAIN_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_ZONE_CENT_GAIN_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 16:12) field ZONE_CENT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_ZONE_CENT_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_ZONE_CENT_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_ZONE_CENT_GAIN_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 16:12) field ZONE_CENT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_ZONE_CENT_GAIN_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_ZONE_CENT_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_ZONE_CENT_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_ZONE_CENT_GAIN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_ZONE_CENT_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_ZONE_CENT_GAIN_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_ZONE_CENT_GAIN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006044 + (N) * 0x14) bits 16:12) field ZONE_CENT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
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
        /* ((0x00006044 + (N) * 0x14) bits 16:12) field ZONE_CENT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
        mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_OFF + subfield_offset),
                                                      MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_ZONE_CENT_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_ZONE_CENT_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_ZONE_CENT_GAIN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_ZONE_CENT_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_ZONE_CENT_GAIN_get", stop_bit, 4 );
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
    /* ((0x00006044 + (N) * 0x14) bits 16:12) field ZONE_CENT_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_MSK)
                  >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_MSK, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_ZONE_CENT_GAIN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_LPF_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_LPF_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_LPF_GAIN_set", N, 95);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_LPF_GAIN_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_LPF_GAIN_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 11:8) field LPF_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_LPF_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_LPF_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_LPF_GAIN_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 11:8) field LPF_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_LPF_GAIN_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_LPF_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_LPF_GAIN_set( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_LPF_GAIN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_LPF_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_LPF_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_LPF_GAIN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006044 + (N) * 0x14) bits 11:8) field LPF_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
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
        /* ((0x00006044 + (N) * 0x14) bits 11:8) field LPF_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
        mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_OFF + subfield_offset),
                                                      MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_LPF_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_LPF_GAIN_get( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_LPF_GAIN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_LPF_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_LPF_GAIN_get", stop_bit, 3 );
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
    /* ((0x00006044 + (N) * 0x14) bits 11:8) field LPF_GAIN of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_MSK)
                  >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_MSK, MPMO_DSP_REG_GENERAL_CFG_REG_BIT_LPF_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_LPF_GAIN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_AGE_GAIN_UPDATE_set( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_AGE_GAIN_UPDATE_set( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_AGE_GAIN_UPDATE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_AGE_GAIN_UPDATE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_AGE_GAIN_UPDATE_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 7) field AGE_GAIN_UPDATE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_GAIN_UPDATE_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_GAIN_UPDATE_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_AGE_GAIN_UPDATE_get( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_AGE_GAIN_UPDATE_get( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_AGE_GAIN_UPDATE_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 7) field AGE_GAIN_UPDATE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_GAIN_UPDATE_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_AGE_GAIN_UPDATE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_AGE_GAIN_UPDATE_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_HOLDOVER_set( mpmo_dsp_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_HOLDOVER_set( mpmo_dsp_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_HOLDOVER_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_HOLDOVER_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_HOLDOVER_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 6) field HOLDOVER of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_HOLDOVER_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_HOLDOVER_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_HOLDOVER_get( mpmo_dsp_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_HOLDOVER_get( mpmo_dsp_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_HOLDOVER_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 6) field HOLDOVER of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_HOLDOVER_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_HOLDOVER_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_HOLDOVER_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_ZONE_CENT_CONTINUOUS_set( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_ZONE_CENT_CONTINUOUS_set( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_ZONE_CENT_CONTINUOUS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_ZONE_CENT_CONTINUOUS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_ZONE_CENT_CONTINUOUS_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 5) field ZONE_CENT_CONTINUOUS of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_CONTINUOUS_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_CONTINUOUS_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_ZONE_CENT_CONTINUOUS_get( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_ZONE_CENT_CONTINUOUS_get( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_ZONE_CENT_CONTINUOUS_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 5) field ZONE_CENT_CONTINUOUS of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_CONTINUOUS_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_CONTINUOUS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_ZONE_CENT_CONTINUOUS_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_ZONE_CENT_TRIGGER_set( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_ZONE_CENT_TRIGGER_set( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_ZONE_CENT_TRIGGER_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_ZONE_CENT_TRIGGER_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_ZONE_CENT_TRIGGER_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 4) field ZONE_CENT_TRIGGER of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_TRIGGER_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_TRIGGER_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_ZONE_CENT_TRIGGER_get( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_ZONE_CENT_TRIGGER_get( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_ZONE_CENT_TRIGGER_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 4) field ZONE_CENT_TRIGGER of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_TRIGGER_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_TRIGGER_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_ZONE_CENT_TRIGGER_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_ZONE_CENT_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_ZONE_CENT_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_ZONE_CENT_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_ZONE_CENT_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_ZONE_CENT_ENABLE_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 3) field ZONE_CENT_ENABLE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_ENABLE_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_ENABLE_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_ZONE_CENT_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_ZONE_CENT_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_ZONE_CENT_ENABLE_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 3) field ZONE_CENT_ENABLE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_ENABLE_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_ZONE_CENT_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_ZONE_CENT_ENABLE_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_CN_BIT_MODE_set( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_CN_BIT_MODE_set( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_BIT_MODE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_CN_BIT_MODE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_CN_BIT_MODE_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 2) field CN_BIT_MODE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_CN_BIT_MODE_get( mpmo_dsp_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_CN_BIT_MODE_get( mpmo_dsp_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_BIT_MODE_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 2) field CN_BIT_MODE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_CN_BIT_MODE_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_CN_GENERATOR_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_CN_GENERATOR_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_GENERATOR_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_CN_GENERATOR_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_CN_GENERATOR_ENABLE_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 1) field CN_GENERATOR_ENABLE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_GENERATOR_ENABLE_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_GENERATOR_ENABLE_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_CN_GENERATOR_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_CN_GENERATOR_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_GENERATOR_ENABLE_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 1) field CN_GENERATOR_ENABLE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_GENERATOR_ENABLE_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_CN_GENERATOR_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_CN_GENERATOR_ENABLE_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_SLICE_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_SLICE_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_SLICE_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_SLICE_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_SLICE_ENABLE_set", N, value );

    /* ((0x00006044 + (N) * 0x14) bits 0) field SLICE_ENABLE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpmo_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_MSK,
                                                  MPMO_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_OFF,
                                                  value);
}

static INLINE UINT32 mpmo_dsp_field_SLICE_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_SLICE_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_SLICE_ENABLE_get", N, 95);
    /* ((0x00006044 + (N) * 0x14) bits 0) field SLICE_ENABLE of register PMC_MPMO_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMO_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_MSK) >> MPMO_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_SLICE_ENABLE_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_CN_BASE_INT_set( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_CN_BASE_INT_set( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_BASE_INT_set", N, 95);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_CN_BASE_INT_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_CN_BASE_INT_set", N, value );

    /* ((0x00006048 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE_INT of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
    mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_MSK,
                                                          MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_OFF,
                                                          value);
}

static INLINE UINT32 mpmo_dsp_field_CN_BASE_INT_get( mpmo_dsp_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_CN_BASE_INT_get( mpmo_dsp_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_BASE_INT_get", N, 95);
    /* ((0x00006048 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE_INT of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_MSK) >> MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_CN_BASE_INT_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_CN_BASE_INT_set( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_CN_BASE_INT_set( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_CN_BASE_INT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_CN_BASE_INT_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_CN_BASE_INT_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_CN_BASE_INT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006048 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE_INT of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
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
        /* ((0x00006048 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE_INT of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
        mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              subfield_mask << (MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_OFF + subfield_offset),
                                                              MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_CN_BASE_INT_get( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_CN_BASE_INT_get( mpmo_dsp_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_CN_BASE_INT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_CN_BASE_INT_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_CN_BASE_INT_get", stop_bit, 23 );
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
    /* ((0x00006048 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE_INT of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    field_value = (reg_value & MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_MSK)
                  >> MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_MSK, MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_CN_BASE_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_CN_BASE_INT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_FAST_LOCK_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_FAST_LOCK_ENABLE_set( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FAST_LOCK_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_FAST_LOCK_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_FAST_LOCK_ENABLE_set", N, value );

    /* ((0x00006048 + (N) * 0x14) bits 31) field FAST_LOCK_ENABLE of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
    mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_ENABLE_MSK,
                                                          MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_ENABLE_OFF,
                                                          value);
}

static INLINE UINT32 mpmo_dsp_field_FAST_LOCK_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_FAST_LOCK_ENABLE_get( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FAST_LOCK_ENABLE_get", N, 95);
    /* ((0x00006048 + (N) * 0x14) bits 31) field FAST_LOCK_ENABLE of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_ENABLE_MSK) >> MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_FAST_LOCK_ENABLE_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_FAST_LOCK_THRESH_set( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_FAST_LOCK_THRESH_set( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FAST_LOCK_THRESH_set", N, 95);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_FAST_LOCK_THRESH_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_FAST_LOCK_THRESH_set", N, value );

    /* ((0x00006048 + (N) * 0x14) bits 27:24) bits 0:3 use field FAST_LOCK_THRESH of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
    mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_MSK,
                                                          MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_OFF,
                                                          value);
}

static INLINE UINT32 mpmo_dsp_field_FAST_LOCK_THRESH_get( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_FAST_LOCK_THRESH_get( mpmo_dsp_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_FAST_LOCK_THRESH_get", N, 95);
    /* ((0x00006048 + (N) * 0x14) bits 27:24) bits 0:3 use field FAST_LOCK_THRESH of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    value = (reg_value & MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_MSK) >> MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_FAST_LOCK_THRESH_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_FAST_LOCK_THRESH_set( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_FAST_LOCK_THRESH_set( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_FAST_LOCK_THRESH_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_FAST_LOCK_THRESH_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_FAST_LOCK_THRESH_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_FAST_LOCK_THRESH_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006048 + (N) * 0x14) bits 27:24) bits 0:3 use field FAST_LOCK_THRESH of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
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
        /* ((0x00006048 + (N) * 0x14) bits 27:24) bits 0:3 use field FAST_LOCK_THRESH of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
        mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              subfield_mask << (MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_OFF + subfield_offset),
                                                              MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_FAST_LOCK_THRESH_get( mpmo_dsp_buffer_t *b_ptr,
                                                                mpmo_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_FAST_LOCK_THRESH_get( mpmo_dsp_buffer_t *b_ptr,
                                                                mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_FAST_LOCK_THRESH_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_FAST_LOCK_THRESH_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_FAST_LOCK_THRESH_get", stop_bit, 3 );
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
    /* ((0x00006048 + (N) * 0x14) bits 27:24) bits 0:3 use field FAST_LOCK_THRESH of register PMC_MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_INTEGER_CFG_REG_array_read( b_ptr,
                                                                 h_ptr,
                                                                 N);
    field_value = (reg_value & MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_MSK)
                  >> MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_MSK, MPMO_DSP_REG_CN_BASE_INTEGER_CFG_REG_BIT_FAST_LOCK_THRESH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_FAST_LOCK_THRESH_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_CN_BASE_N_set( mpmo_dsp_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_CN_BASE_N_set( mpmo_dsp_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_BASE_N_set", N, 95);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_CN_BASE_N_set", N, value );

    /* ((0x0000604c + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_N of register PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG index N=0..95 */
    mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_MSK,
                                                            MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_OFF,
                                                            value);
}

static INLINE UINT32 mpmo_dsp_field_CN_BASE_N_get( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_CN_BASE_N_get( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_BASE_N_get", N, 95);
    /* ((0x0000604c + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_N of register PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_MSK) >> MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_CN_BASE_N_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_CN_BASE_N_set( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_CN_BASE_N_set( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_CN_BASE_N_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_CN_BASE_N_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_CN_BASE_N_set", stop_bit, 31 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_CN_BASE_N_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000604c + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_N of register PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG index N=0..95 */
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
        /* ((0x0000604c + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_N of register PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG index N=0..95 */
        mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_field_set( b_ptr,
                                                                h_ptr,
                                                                N,
                                                                subfield_mask << (MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_OFF + subfield_offset),
                                                                MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_CN_BASE_N_get( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_CN_BASE_N_get( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_CN_BASE_N_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_CN_BASE_N_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_CN_BASE_N_get", stop_bit, 31 );
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
    /* ((0x0000604c + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_N of register PMC_MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_NUMERATOR_CFG_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    field_value = (reg_value & MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_MSK)
                  >> MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_MSK, MPMO_DSP_REG_CN_BASE_NUMERATOR_CFG_REG_BIT_CN_BASE_N_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_CN_BASE_N_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_CN_BASE_D_set( mpmo_dsp_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_CN_BASE_D_set( mpmo_dsp_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_BASE_D_set", N, 95);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_CN_BASE_D_set", N, value );

    /* ((0x00006050 + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_D of register PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG index N=0..95 */
    mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_field_set( b_ptr,
                                                              h_ptr,
                                                              N,
                                                              MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_MSK,
                                                              MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_OFF,
                                                              value);
}

static INLINE UINT32 mpmo_dsp_field_CN_BASE_D_get( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_CN_BASE_D_get( mpmo_dsp_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_CN_BASE_D_get", N, 95);
    /* ((0x00006050 + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_D of register PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    value = (reg_value & MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_MSK) >> MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_CN_BASE_D_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_CN_BASE_D_set( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_CN_BASE_D_set( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_CN_BASE_D_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_CN_BASE_D_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_CN_BASE_D_set", stop_bit, 31 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_CN_BASE_D_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006050 + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_D of register PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG index N=0..95 */
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
        /* ((0x00006050 + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_D of register PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG index N=0..95 */
        mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_field_set( b_ptr,
                                                                  h_ptr,
                                                                  N,
                                                                  subfield_mask << (MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_OFF + subfield_offset),
                                                                  MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_OFF + subfield_offset,
                                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_CN_BASE_D_get( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_CN_BASE_D_get( mpmo_dsp_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_CN_BASE_D_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_CN_BASE_D_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_CN_BASE_D_get", stop_bit, 31 );
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
    /* ((0x00006050 + (N) * 0x14) bits 31:0) bits 0:31 use field CN_BASE_D of register PMC_MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_CN_BASE_DENOMINATOR_CFG_REG_array_read( b_ptr,
                                                                     h_ptr,
                                                                     N);
    field_value = (reg_value & MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_MSK)
                  >> MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_MSK, MPMO_DSP_REG_CN_BASE_DENOMINATOR_CFG_REG_BIT_CN_BASE_D_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_CN_BASE_D_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_BWR_RAMP_DELAY_set( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_BWR_RAMP_DELAY_set( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_BWR_RAMP_DELAY_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_BWR_RAMP_DELAY_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_BWR_RAMP_DELAY_set", N, value );

    /* ((0x00006054 + (N) * 0x14) bits 29:28) field BWR_RAMP_DELAY of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
    mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_MSK,
                                                           MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_OFF,
                                                           value);
}

static INLINE UINT32 mpmo_dsp_field_BWR_RAMP_DELAY_get( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_BWR_RAMP_DELAY_get( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_BWR_RAMP_DELAY_get", N, 95);
    /* ((0x00006054 + (N) * 0x14) bits 29:28) field BWR_RAMP_DELAY of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_MSK) >> MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_BWR_RAMP_DELAY_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_BWR_RAMP_DELAY_set( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_BWR_RAMP_DELAY_set( mpmo_dsp_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_BWR_RAMP_DELAY_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_BWR_RAMP_DELAY_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_BWR_RAMP_DELAY_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_BWR_RAMP_DELAY_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006054 + (N) * 0x14) bits 29:28) field BWR_RAMP_DELAY of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
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
        /* ((0x00006054 + (N) * 0x14) bits 29:28) field BWR_RAMP_DELAY of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
        mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               subfield_mask << (MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_OFF + subfield_offset),
                                                               MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_BWR_RAMP_DELAY_get( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_BWR_RAMP_DELAY_get( mpmo_dsp_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_BWR_RAMP_DELAY_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_BWR_RAMP_DELAY_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_BWR_RAMP_DELAY_get", stop_bit, 1 );
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
    /* ((0x00006054 + (N) * 0x14) bits 29:28) field BWR_RAMP_DELAY of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    field_value = (reg_value & MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_MSK)
                  >> MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_MSK, MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_RAMP_DELAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_BWR_RAMP_DELAY_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_dsp_field_BWR_STEP_set( mpmo_dsp_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_BWR_STEP_set( mpmo_dsp_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_BWR_STEP_set", N, 95);
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_dsp_field_BWR_STEP_set", value, 1048575);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_dsp_field_BWR_STEP_set", N, value );

    /* ((0x00006054 + (N) * 0x14) bits 19:0) bits 0:19 use field BWR_STEP of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
    mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_field_set( b_ptr,
                                                           h_ptr,
                                                           N,
                                                           MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_MSK,
                                                           MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_OFF,
                                                           value);
}

static INLINE UINT32 mpmo_dsp_field_BWR_STEP_get( mpmo_dsp_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_BWR_STEP_get( mpmo_dsp_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_BWR_STEP_get", N, 95);
    /* ((0x00006054 + (N) * 0x14) bits 19:0) bits 0:19 use field BWR_STEP of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    value = (reg_value & MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_MSK) >> MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_BWR_STEP_get", N, value );

    return value;
}
static INLINE void mpmo_dsp_field_range_BWR_STEP_set( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_dsp_field_range_BWR_STEP_set( mpmo_dsp_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_BWR_STEP_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_BWR_STEP_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_BWR_STEP_set", stop_bit, 19 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_BWR_STEP_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00006054 + (N) * 0x14) bits 19:0) bits 0:19 use field BWR_STEP of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
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
        if (stop_bit < 19) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 19;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00006054 + (N) * 0x14) bits 19:0) bits 0:19 use field BWR_STEP of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
        mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               subfield_mask << (MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_OFF + subfield_offset),
                                                               MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_dsp_field_range_BWR_STEP_get( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_BWR_STEP_get( mpmo_dsp_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
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

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_BWR_STEP_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_BWR_STEP_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_BWR_STEP_get", stop_bit, 19 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 19) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 19;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00006054 + (N) * 0x14) bits 19:0) bits 0:19 use field BWR_STEP of register PMC_MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG index N=0..95 */
    reg_value = mpmo_dsp_reg_ODUFLEX_RESIZING_CFG_REG_array_read( b_ptr,
                                                                  h_ptr,
                                                                  N);
    field_value = (reg_value & MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_MSK)
                  >> MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_MSK, MPMO_DSP_REG_ODUFLEX_RESIZING_CFG_REG_BIT_BWR_STEP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_BWR_STEP_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size80
 * ==================================================================================
 */
static INLINE UINT32 mpmo_dsp_field_INTEGRATOR684_get( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_INTEGRATOR684_get( mpmo_dsp_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 79)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_INTEGRATOR684_get", N, 79);
    /* ((0x00006684 + (N) * 0x8) bits 31:0) bits 0:31 use field INTEGRATOR684 of register PMC_MPMO_DSP_REG_DEBUG684_REG index N=0..79 */
    reg_value = mpmo_dsp_reg_DEBUG684_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & MPMO_DSP_REG_DEBUG684_REG_BIT_INTEGRATOR684_MSK) >> MPMO_DSP_REG_DEBUG684_REG_BIT_INTEGRATOR684_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_dsp_field_INTEGRATOR684_get", N, value );

    return value;
}
static INLINE UINT32 mpmo_dsp_field_range_INTEGRATOR684_get( mpmo_dsp_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_dsp_field_range_INTEGRATOR684_get( mpmo_dsp_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
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

    if (N > 79)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_dsp_field_range_INTEGRATOR684_get", N, 79);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_dsp_field_range_INTEGRATOR684_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_dsp_field_range_INTEGRATOR684_get", stop_bit, 31 );
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
    /* ((0x00006684 + (N) * 0x8) bits 31:0) bits 0:31 use field INTEGRATOR684 of register PMC_MPMO_DSP_REG_DEBUG684_REG index N=0..79 */
    reg_value = mpmo_dsp_reg_DEBUG684_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & MPMO_DSP_REG_DEBUG684_REG_BIT_INTEGRATOR684_MSK)
                  >> MPMO_DSP_REG_DEBUG684_REG_BIT_INTEGRATOR684_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMO_DSP_REG_DEBUG684_REG_BIT_INTEGRATOR684_MSK, MPMO_DSP_REG_DEBUG684_REG_BIT_INTEGRATOR684_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_dsp_field_range_INTEGRATOR684_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MPMO_DSP_IO_INLINE_H */
