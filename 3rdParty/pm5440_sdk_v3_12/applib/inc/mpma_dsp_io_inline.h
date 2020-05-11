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
 *     and register accessor functions for the mpma_dsp block
 *****************************************************************************/
#ifndef _MPMA_DSP_IO_INLINE_H
#define _MPMA_DSP_IO_INLINE_H

#include "mpma_loc.h"
#include "mpma_dsp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MPMA_DSP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mpma_dsp
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
    mpma_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} mpma_dsp_buffer_t;
static INLINE void mpma_dsp_buffer_init( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mpma_dsp_buffer_init( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mpma_dsp_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mpma_dsp_buffer_flush( mpma_dsp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mpma_dsp_buffer_flush( mpma_dsp_buffer_t *b_ptr )
{
    IOLOG( "mpma_dsp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mpma_dsp_reg_read( mpma_dsp_buffer_t *b_ptr,
                                        mpma_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_reg_read( mpma_dsp_buffer_t *b_ptr,
                                        mpma_handle_t *h_ptr,
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
static INLINE void mpma_dsp_reg_write( mpma_dsp_buffer_t *b_ptr,
                                       mpma_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_write( mpma_dsp_buffer_t *b_ptr,
                                       mpma_handle_t *h_ptr,
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

static INLINE void mpma_dsp_field_set( mpma_dsp_buffer_t *b_ptr,
                                       mpma_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_set( mpma_dsp_buffer_t *b_ptr,
                                       mpma_handle_t *h_ptr,
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

static INLINE void mpma_dsp_action_on_write_field_set( mpma_dsp_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_action_on_write_field_set( mpma_dsp_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
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

static INLINE void mpma_dsp_burst_read( mpma_dsp_buffer_t *b_ptr,
                                        mpma_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_burst_read( mpma_dsp_buffer_t *b_ptr,
                                        mpma_handle_t *h_ptr,
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

static INLINE void mpma_dsp_burst_write( mpma_dsp_buffer_t *b_ptr,
                                         mpma_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_burst_write( mpma_dsp_buffer_t *b_ptr,
                                         mpma_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mpma_dsp_poll( mpma_dsp_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_dsp_poll( mpma_dsp_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
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
 *  register access functions for mpma_dsp
 * ==================================================================================
 */

static INLINE void mpma_dsp_reg_COMMON_CFG_REG_write( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_COMMON_CFG_REG_write( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_dsp_reg_COMMON_CFG_REG_write", value );
    mpma_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_COMMON_CFG_REG,
                        value);
}

static INLINE void mpma_dsp_reg_COMMON_CFG_REG_field_set( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_COMMON_CFG_REG_field_set( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_dsp_reg_COMMON_CFG_REG_field_set", mask, ofs, value );
    mpma_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_COMMON_CFG_REG,
                        mask,
                        PMC_MPMA_DSP_REG_COMMON_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpma_dsp_reg_COMMON_CFG_REG_read( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_reg_COMMON_CFG_REG_read( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMA_DSP_REG_COMMON_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "mpma_dsp_reg_COMMON_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void mpma_dsp_reg_GENERAL_CFG_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_GENERAL_CFG_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_dsp_reg_GENERAL_CFG_REG_array_write", value );
    mpma_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_GENERAL_CFG_REG(N),
                        value);
}

static INLINE void mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpma_dsp_reg_GENERAL_CFG_REG_array_field_set", N, mask, ofs, value );
    mpma_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_GENERAL_CFG_REG(N),
                        mask,
                        PMC_MPMA_DSP_REG_GENERAL_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpma_dsp_reg_GENERAL_CFG_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_reg_GENERAL_CFG_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpma_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMA_DSP_REG_GENERAL_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpma_dsp_reg_GENERAL_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_write", value );
    mpma_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG(N),
                        value);
}

static INLINE void mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                                     mpma_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                                     mpma_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set", N, mask, ofs, value );
    mpma_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG(N),
                        mask,
                        PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                                  mpma_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                                  mpma_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpma_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                                   mpma_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                                   mpma_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_write", value );
    mpma_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG(N),
                        value);
}

static INLINE void mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                                       mpma_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                                       mpma_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_field_set", N, mask, ofs, value );
    mpma_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG(N),
                        mask,
                        PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                                    mpma_handle_t *h_ptr,
                                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                                    mpma_handle_t *h_ptr,
                                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpma_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpma_dsp_reg_DSM_CFG_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_DSM_CFG_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_dsp_reg_DSM_CFG_REG_array_write", value );
    mpma_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_DSM_CFG_REG(N),
                        value);
}

static INLINE void mpma_dsp_reg_DSM_CFG_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_DSM_CFG_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpma_dsp_reg_DSM_CFG_REG_array_field_set", N, mask, ofs, value );
    mpma_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_DSM_CFG_REG(N),
                        mask,
                        PMC_MPMA_DSP_REG_DSM_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpma_dsp_reg_DSM_CFG_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_reg_DSM_CFG_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpma_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMA_DSP_REG_DSM_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpma_dsp_reg_DSM_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpma_dsp_reg_CN_BASE_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_CN_BASE_REG_array_write( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_dsp_reg_CN_BASE_REG_array_write", value );
    mpma_dsp_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_CN_BASE_REG(N),
                        value);
}

static INLINE void mpma_dsp_reg_CN_BASE_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_reg_CN_BASE_REG_array_field_set( mpma_dsp_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpma_dsp_reg_CN_BASE_REG_array_field_set", N, mask, ofs, value );
    mpma_dsp_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_MPMA_DSP_REG_CN_BASE_REG(N),
                        mask,
                        PMC_MPMA_DSP_REG_CN_BASE_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 mpma_dsp_reg_CN_BASE_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_reg_CN_BASE_REG_array_read( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpma_dsp_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_MPMA_DSP_REG_CN_BASE_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpma_dsp_reg_CN_BASE_REG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mpma_dsp_field_SCHEDULING_PACER_EN_set( mpma_dsp_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_SCHEDULING_PACER_EN_set( mpma_dsp_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_SCHEDULING_PACER_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpma_dsp_field_SCHEDULING_PACER_EN_set", value );

    /* (0x00003800 bits 16) field SCHEDULING_PACER_EN of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
    mpma_dsp_reg_COMMON_CFG_REG_field_set( b_ptr,
                                           h_ptr,
                                           MPMA_DSP_REG_COMMON_CFG_REG_BIT_SCHEDULING_PACER_EN_MSK,
                                           MPMA_DSP_REG_COMMON_CFG_REG_BIT_SCHEDULING_PACER_EN_OFF,
                                           value);
}

static INLINE UINT32 mpma_dsp_field_SCHEDULING_PACER_EN_get( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_SCHEDULING_PACER_EN_get( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00003800 bits 16) field SCHEDULING_PACER_EN of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
    reg_value = mpma_dsp_reg_COMMON_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MPMA_DSP_REG_COMMON_CFG_REG_BIT_SCHEDULING_PACER_EN_MSK) >> MPMA_DSP_REG_COMMON_CFG_REG_BIT_SCHEDULING_PACER_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_dsp_field_SCHEDULING_PACER_EN_get", value );

    return value;
}
static INLINE void mpma_dsp_field_INTERRUPT_CLR_set( mpma_dsp_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_INTERRUPT_CLR_set( mpma_dsp_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_INTERRUPT_CLR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpma_dsp_field_INTERRUPT_CLR_set", value );

    /* (0x00003800 bits 8) field INTERRUPT_CLR of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
    mpma_dsp_reg_COMMON_CFG_REG_field_set( b_ptr,
                                           h_ptr,
                                           MPMA_DSP_REG_COMMON_CFG_REG_BIT_INTERRUPT_CLR_MSK,
                                           MPMA_DSP_REG_COMMON_CFG_REG_BIT_INTERRUPT_CLR_OFF,
                                           value);
}

static INLINE UINT32 mpma_dsp_field_INTERRUPT_CLR_get( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_INTERRUPT_CLR_get( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00003800 bits 8) field INTERRUPT_CLR of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
    reg_value = mpma_dsp_reg_COMMON_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MPMA_DSP_REG_COMMON_CFG_REG_BIT_INTERRUPT_CLR_MSK) >> MPMA_DSP_REG_COMMON_CFG_REG_BIT_INTERRUPT_CLR_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_dsp_field_INTERRUPT_CLR_get", value );

    return value;
}
static INLINE void mpma_dsp_field_CLIENT_WIDTH_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_CLIENT_WIDTH_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_CLIENT_WIDTH_set", value, 63);
    IOLOG( "%s <= 0x%08x", "mpma_dsp_field_CLIENT_WIDTH_set", value );

    /* (0x00003800 bits 5:0) bits 0:5 use field CLIENT_WIDTH of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
    mpma_dsp_reg_COMMON_CFG_REG_field_set( b_ptr,
                                           h_ptr,
                                           MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_MSK,
                                           MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_OFF,
                                           value);
}

static INLINE UINT32 mpma_dsp_field_CLIENT_WIDTH_get( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CLIENT_WIDTH_get( mpma_dsp_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00003800 bits 5:0) bits 0:5 use field CLIENT_WIDTH of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
    reg_value = mpma_dsp_reg_COMMON_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_MSK) >> MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_dsp_field_CLIENT_WIDTH_get", value );

    return value;
}
static INLINE void mpma_dsp_field_range_CLIENT_WIDTH_set( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_CLIENT_WIDTH_set( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_CLIENT_WIDTH_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_CLIENT_WIDTH_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_CLIENT_WIDTH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00003800 bits 5:0) bits 0:5 use field CLIENT_WIDTH of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
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
        /* (0x00003800 bits 5:0) bits 0:5 use field CLIENT_WIDTH of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
        mpma_dsp_reg_COMMON_CFG_REG_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_OFF + subfield_offset),
                                               MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_CLIENT_WIDTH_get( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_CLIENT_WIDTH_get( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_CLIENT_WIDTH_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_CLIENT_WIDTH_get", stop_bit, 5 );
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
    /* (0x00003800 bits 5:0) bits 0:5 use field CLIENT_WIDTH of register PMC_MPMA_DSP_REG_COMMON_CFG_REG */
    reg_value = mpma_dsp_reg_COMMON_CFG_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_MSK)
                  >> MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_MSK, MPMA_DSP_REG_COMMON_CFG_REG_BIT_CLIENT_WIDTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_CLIENT_WIDTH_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void mpma_dsp_field_PKTS_PER_CN_set( mpma_dsp_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_PKTS_PER_CN_set( mpma_dsp_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKTS_PER_CN_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_PKTS_PER_CN_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_PKTS_PER_CN_set", N, value );

    /* ((0x00003804 + (N) * 0x14) bits 31:16) bits 0:15 use field PKTS_PER_CN of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_MSK,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_OFF,
                                                  value);
}

static INLINE UINT32 mpma_dsp_field_PKTS_PER_CN_get( mpma_dsp_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_PKTS_PER_CN_get( mpma_dsp_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKTS_PER_CN_get", N, 95);
    /* ((0x00003804 + (N) * 0x14) bits 31:16) bits 0:15 use field PKTS_PER_CN of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_MSK) >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_PKTS_PER_CN_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_PKTS_PER_CN_set( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_PKTS_PER_CN_set( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKTS_PER_CN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKTS_PER_CN_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKTS_PER_CN_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKTS_PER_CN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003804 + (N) * 0x14) bits 31:16) bits 0:15 use field PKTS_PER_CN of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
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
        /* ((0x00003804 + (N) * 0x14) bits 31:16) bits 0:15 use field PKTS_PER_CN of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
        mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_OFF + subfield_offset),
                                                      MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_PKTS_PER_CN_get( mpma_dsp_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_PKTS_PER_CN_get( mpma_dsp_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKTS_PER_CN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKTS_PER_CN_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKTS_PER_CN_get", stop_bit, 15 );
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
    /* ((0x00003804 + (N) * 0x14) bits 31:16) bits 0:15 use field PKTS_PER_CN of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_MSK)
                  >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_MSK, MPMA_DSP_REG_GENERAL_CFG_REG_BIT_PKTS_PER_CN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKTS_PER_CN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_dsp_field_CN_MODE_set( mpma_dsp_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_CN_MODE_set( mpma_dsp_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_MODE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_CN_MODE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_CN_MODE_set", N, value );

    /* ((0x00003804 + (N) * 0x14) bits 4) field CN_MODE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_MODE_MSK,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_MODE_OFF,
                                                  value);
}

static INLINE UINT32 mpma_dsp_field_CN_MODE_get( mpma_dsp_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CN_MODE_get( mpma_dsp_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_MODE_get", N, 95);
    /* ((0x00003804 + (N) * 0x14) bits 4) field CN_MODE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_MODE_MSK) >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_CN_MODE_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_CN_BIT_MODE_set( mpma_dsp_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_CN_BIT_MODE_set( mpma_dsp_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_BIT_MODE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_CN_BIT_MODE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_CN_BIT_MODE_set", N, value );

    /* ((0x00003804 + (N) * 0x14) bits 3) field CN_BIT_MODE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_MSK,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_OFF,
                                                  value);
}

static INLINE UINT32 mpma_dsp_field_CN_BIT_MODE_get( mpma_dsp_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CN_BIT_MODE_get( mpma_dsp_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_BIT_MODE_get", N, 95);
    /* ((0x00003804 + (N) * 0x14) bits 3) field CN_BIT_MODE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_MSK) >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_BIT_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_CN_BIT_MODE_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_CN_ENABLE_set( mpma_dsp_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_CN_ENABLE_set( mpma_dsp_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_CN_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_CN_ENABLE_set", N, value );

    /* ((0x00003804 + (N) * 0x14) bits 2) field CN_ENABLE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_ENABLE_MSK,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_ENABLE_OFF,
                                                  value);
}

static INLINE UINT32 mpma_dsp_field_CN_ENABLE_get( mpma_dsp_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CN_ENABLE_get( mpma_dsp_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_ENABLE_get", N, 95);
    /* ((0x00003804 + (N) * 0x14) bits 2) field CN_ENABLE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_ENABLE_MSK) >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CN_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_CN_ENABLE_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_CND_ENABLE_set( mpma_dsp_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_CND_ENABLE_set( mpma_dsp_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CND_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_CND_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_CND_ENABLE_set", N, value );

    /* ((0x00003804 + (N) * 0x14) bits 1) field CND_ENABLE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CND_ENABLE_MSK,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CND_ENABLE_OFF,
                                                  value);
}

static INLINE UINT32 mpma_dsp_field_CND_ENABLE_get( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CND_ENABLE_get( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CND_ENABLE_get", N, 95);
    /* ((0x00003804 + (N) * 0x14) bits 1) field CND_ENABLE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CND_ENABLE_MSK) >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_CND_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_CND_ENABLE_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_SLICE_ENABLE_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_SLICE_ENABLE_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_SLICE_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_SLICE_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_SLICE_ENABLE_set", N, value );

    /* ((0x00003804 + (N) * 0x14) bits 0) field SLICE_ENABLE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_MSK,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_OFF,
                                                  value);
}

static INLINE UINT32 mpma_dsp_field_SLICE_ENABLE_get( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_SLICE_ENABLE_get( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_SLICE_ENABLE_get", N, 95);
    /* ((0x00003804 + (N) * 0x14) bits 0) field SLICE_ENABLE of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_MSK) >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SLICE_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_SLICE_ENABLE_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_INTEGRATOR_CLAMP_set( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_INTEGRATOR_CLAMP_set( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_INTEGRATOR_CLAMP_set", N, 95);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_INTEGRATOR_CLAMP_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_INTEGRATOR_CLAMP_set", N, value );

    /* ((0x00003804 + (N) * 0x14) bits 15:12) bits 0:3 use field INTEGRATOR_CLAMP of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_MSK,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_OFF,
                                                  value);
}

static INLINE UINT32 mpma_dsp_field_INTEGRATOR_CLAMP_get( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_INTEGRATOR_CLAMP_get( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_INTEGRATOR_CLAMP_get", N, 95);
    /* ((0x00003804 + (N) * 0x14) bits 15:12) bits 0:3 use field INTEGRATOR_CLAMP of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_MSK) >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_INTEGRATOR_CLAMP_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_INTEGRATOR_CLAMP_set( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_INTEGRATOR_CLAMP_set( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_INTEGRATOR_CLAMP_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_INTEGRATOR_CLAMP_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_INTEGRATOR_CLAMP_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_INTEGRATOR_CLAMP_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003804 + (N) * 0x14) bits 15:12) bits 0:3 use field INTEGRATOR_CLAMP of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
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
        /* ((0x00003804 + (N) * 0x14) bits 15:12) bits 0:3 use field INTEGRATOR_CLAMP of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
        mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_OFF + subfield_offset),
                                                      MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_INTEGRATOR_CLAMP_get( mpma_dsp_buffer_t *b_ptr,
                                                                mpma_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_INTEGRATOR_CLAMP_get( mpma_dsp_buffer_t *b_ptr,
                                                                mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_INTEGRATOR_CLAMP_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_INTEGRATOR_CLAMP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_INTEGRATOR_CLAMP_get", stop_bit, 3 );
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
    /* ((0x00003804 + (N) * 0x14) bits 15:12) bits 0:3 use field INTEGRATOR_CLAMP of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_MSK)
                  >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_MSK, MPMA_DSP_REG_GENERAL_CFG_REG_BIT_INTEGRATOR_CLAMP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_INTEGRATOR_CLAMP_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_set( mpma_dsp_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_set( mpma_dsp_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_set", N, value );

    /* ((0x00003804 + (N) * 0x14) bits 5) field SCHEDULING_PACER_EN_LOCAL of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    mpma_dsp_reg_GENERAL_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SCHEDULING_PACER_EN_LOCAL_MSK,
                                                  MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SCHEDULING_PACER_EN_LOCAL_OFF,
                                                  value);
}

static INLINE UINT32 mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_get( mpma_dsp_buffer_t *b_ptr,
                                                                   mpma_handle_t *h_ptr,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_get( mpma_dsp_buffer_t *b_ptr,
                                                                   mpma_handle_t *h_ptr,
                                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_get", N, 95);
    /* ((0x00003804 + (N) * 0x14) bits 5) field SCHEDULING_PACER_EN_LOCAL of register PMC_MPMA_DSP_REG_GENERAL_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_GENERAL_CFG_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SCHEDULING_PACER_EN_LOCAL_MSK) >> MPMA_DSP_REG_GENERAL_CFG_REG_BIT_SCHEDULING_PACER_EN_LOCAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_SCHEDULING_PACER_EN_LOCAL_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_PKT_PERIOD_D_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_PKT_PERIOD_D_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_PERIOD_D_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_PKT_PERIOD_D_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_PKT_PERIOD_D_set", N, value );

    /* ((0x00003808 + (N) * 0x14) bits 31:24) bits 0:7 use field PKT_PERIOD_D of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
    mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_MSK,
                                                      MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_OFF,
                                                      value);
}

static INLINE UINT32 mpma_dsp_field_PKT_PERIOD_D_get( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_PKT_PERIOD_D_get( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_PERIOD_D_get", N, 95);
    /* ((0x00003808 + (N) * 0x14) bits 31:24) bits 0:7 use field PKT_PERIOD_D of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_MSK) >> MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_PKT_PERIOD_D_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_PKT_PERIOD_D_set( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_PKT_PERIOD_D_set( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKT_PERIOD_D_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKT_PERIOD_D_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKT_PERIOD_D_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKT_PERIOD_D_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003808 + (N) * 0x14) bits 31:24) bits 0:7 use field PKT_PERIOD_D of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
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
        /* ((0x00003808 + (N) * 0x14) bits 31:24) bits 0:7 use field PKT_PERIOD_D of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
        mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_OFF + subfield_offset),
                                                          MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_PKT_PERIOD_D_get( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_PKT_PERIOD_D_get( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKT_PERIOD_D_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKT_PERIOD_D_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKT_PERIOD_D_get", stop_bit, 7 );
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
    /* ((0x00003808 + (N) * 0x14) bits 31:24) bits 0:7 use field PKT_PERIOD_D of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_MSK)
                  >> MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_MSK, MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_PERIOD_D_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKT_PERIOD_D_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_dsp_field_PKT_SIZE_VAR_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_PKT_SIZE_VAR_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_SIZE_VAR_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_PKT_SIZE_VAR_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_PKT_SIZE_VAR_set", N, value );

    /* ((0x00003808 + (N) * 0x14) bits 16) field PKT_SIZE_VAR of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
    mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_MSK,
                                                      MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_OFF,
                                                      value);
}

static INLINE UINT32 mpma_dsp_field_PKT_SIZE_VAR_get( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_PKT_SIZE_VAR_get( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_SIZE_VAR_get", N, 95);
    /* ((0x00003808 + (N) * 0x14) bits 16) field PKT_SIZE_VAR of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_MSK) >> MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_PKT_SIZE_VAR_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_PKT_SIZE_set( mpma_dsp_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_PKT_SIZE_set( mpma_dsp_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_SIZE_set", N, 95);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_PKT_SIZE_set", value, 511);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_PKT_SIZE_set", N, value );

    /* ((0x00003808 + (N) * 0x14) bits 8:0) bits 0:8 use field PKT_SIZE of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
    mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK,
                                                      MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF,
                                                      value);
}

static INLINE UINT32 mpma_dsp_field_PKT_SIZE_get( mpma_dsp_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_PKT_SIZE_get( mpma_dsp_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_SIZE_get", N, 95);
    /* ((0x00003808 + (N) * 0x14) bits 8:0) bits 0:8 use field PKT_SIZE of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK) >> MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_PKT_SIZE_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_PKT_SIZE_set( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_PKT_SIZE_set( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKT_SIZE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKT_SIZE_set", stop_bit, 8 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKT_SIZE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003808 + (N) * 0x14) bits 8:0) bits 0:8 use field PKT_SIZE of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
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
        /* ((0x00003808 + (N) * 0x14) bits 8:0) bits 0:8 use field PKT_SIZE of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
        mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF + subfield_offset),
                                                          MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_PKT_SIZE_get( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_PKT_SIZE_get( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKT_SIZE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKT_SIZE_get", stop_bit, 8 );
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
    /* ((0x00003808 + (N) * 0x14) bits 8:0) bits 0:8 use field PKT_SIZE of register PMC_MPMA_DSP_REG_PACKET_SIZE_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_SIZE_CFG_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK)
                  >> MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK, MPMA_DSP_REG_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKT_SIZE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_dsp_field_PKT_PERIOD_N_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_PKT_PERIOD_N_set( mpma_dsp_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_PERIOD_N_set", N, 95);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_PKT_PERIOD_N_set", value, 511);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_PKT_PERIOD_N_set", N, value );

    /* ((0x0000380c + (N) * 0x14) bits 24:16) bits 0:8 use field PKT_PERIOD_N of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
    mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_MSK,
                                                        MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_OFF,
                                                        value);
}

static INLINE UINT32 mpma_dsp_field_PKT_PERIOD_N_get( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_PKT_PERIOD_N_get( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_PERIOD_N_get", N, 95);
    /* ((0x0000380c + (N) * 0x14) bits 24:16) bits 0:8 use field PKT_PERIOD_N of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_MSK) >> MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_PKT_PERIOD_N_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_PKT_PERIOD_N_set( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_PKT_PERIOD_N_set( mpma_dsp_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKT_PERIOD_N_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKT_PERIOD_N_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKT_PERIOD_N_set", stop_bit, 8 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKT_PERIOD_N_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000380c + (N) * 0x14) bits 24:16) bits 0:8 use field PKT_PERIOD_N of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
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
        /* ((0x0000380c + (N) * 0x14) bits 24:16) bits 0:8 use field PKT_PERIOD_N of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
        mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_OFF + subfield_offset),
                                                            MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_PKT_PERIOD_N_get( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_PKT_PERIOD_N_get( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKT_PERIOD_N_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKT_PERIOD_N_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKT_PERIOD_N_get", stop_bit, 8 );
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
    /* ((0x0000380c + (N) * 0x14) bits 24:16) bits 0:8 use field PKT_PERIOD_N of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_MSK)
                  >> MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_MSK, MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_N_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKT_PERIOD_N_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_dsp_field_PKT_PERIOD_INT_set( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_PKT_PERIOD_INT_set( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_PERIOD_INT_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_PKT_PERIOD_INT_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_PKT_PERIOD_INT_set", N, value );

    /* ((0x0000380c + (N) * 0x14) bits 15:0) bits 0:15 use field PKT_PERIOD_INT of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
    mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_field_set( b_ptr,
                                                        h_ptr,
                                                        N,
                                                        MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_MSK,
                                                        MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_OFF,
                                                        value);
}

static INLINE UINT32 mpma_dsp_field_PKT_PERIOD_INT_get( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_PKT_PERIOD_INT_get( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_PKT_PERIOD_INT_get", N, 95);
    /* ((0x0000380c + (N) * 0x14) bits 15:0) bits 0:15 use field PKT_PERIOD_INT of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    value = (reg_value & MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_MSK) >> MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_PKT_PERIOD_INT_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_PKT_PERIOD_INT_set( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_PKT_PERIOD_INT_set( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKT_PERIOD_INT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKT_PERIOD_INT_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKT_PERIOD_INT_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKT_PERIOD_INT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000380c + (N) * 0x14) bits 15:0) bits 0:15 use field PKT_PERIOD_INT of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
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
        /* ((0x0000380c + (N) * 0x14) bits 15:0) bits 0:15 use field PKT_PERIOD_INT of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
        mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            subfield_mask << (MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_OFF + subfield_offset),
                                                            MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_PKT_PERIOD_INT_get( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_PKT_PERIOD_INT_get( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_PKT_PERIOD_INT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_PKT_PERIOD_INT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_PKT_PERIOD_INT_get", stop_bit, 15 );
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
    /* ((0x0000380c + (N) * 0x14) bits 15:0) bits 0:15 use field PKT_PERIOD_INT of register PMC_MPMA_DSP_REG_PACKET_PERIOD_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_PACKET_PERIOD_CFG_REG_array_read( b_ptr,
                                                               h_ptr,
                                                               N);
    field_value = (reg_value & MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_MSK)
                  >> MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_MSK, MPMA_DSP_REG_PACKET_PERIOD_CFG_REG_BIT_PKT_PERIOD_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_PKT_PERIOD_INT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_dsp_field_DSM_TYPE_set( mpma_dsp_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_DSM_TYPE_set( mpma_dsp_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_DSM_TYPE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_DSM_TYPE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_DSM_TYPE_set", N, value );

    /* ((0x00003810 + (N) * 0x14) bits 4) field DSM_TYPE of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    mpma_dsp_reg_DSM_CFG_REG_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              MPMA_DSP_REG_DSM_CFG_REG_BIT_DSM_TYPE_MSK,
                                              MPMA_DSP_REG_DSM_CFG_REG_BIT_DSM_TYPE_OFF,
                                              value);
}

static INLINE UINT32 mpma_dsp_field_DSM_TYPE_get( mpma_dsp_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_DSM_TYPE_get( mpma_dsp_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_DSM_TYPE_get", N, 95);
    /* ((0x00003810 + (N) * 0x14) bits 4) field DSM_TYPE of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_DSM_CFG_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & MPMA_DSP_REG_DSM_CFG_REG_BIT_DSM_TYPE_MSK) >> MPMA_DSP_REG_DSM_CFG_REG_BIT_DSM_TYPE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_DSM_TYPE_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_MPMA_GAIN_set( mpma_dsp_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_MPMA_GAIN_set( mpma_dsp_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_MPMA_GAIN_set", N, 95);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_MPMA_GAIN_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_MPMA_GAIN_set", N, value );

    /* ((0x00003810 + (N) * 0x14) bits 3:0) bits 0:3 use field MPMA_GAIN of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    mpma_dsp_reg_DSM_CFG_REG_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_MSK,
                                              MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_OFF,
                                              value);
}

static INLINE UINT32 mpma_dsp_field_MPMA_GAIN_get( mpma_dsp_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_MPMA_GAIN_get( mpma_dsp_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_MPMA_GAIN_get", N, 95);
    /* ((0x00003810 + (N) * 0x14) bits 3:0) bits 0:3 use field MPMA_GAIN of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_DSM_CFG_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_MSK) >> MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_MPMA_GAIN_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_MPMA_GAIN_set( mpma_dsp_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_MPMA_GAIN_set( mpma_dsp_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_MPMA_GAIN_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_MPMA_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_MPMA_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_MPMA_GAIN_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003810 + (N) * 0x14) bits 3:0) bits 0:3 use field MPMA_GAIN of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
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
        /* ((0x00003810 + (N) * 0x14) bits 3:0) bits 0:3 use field MPMA_GAIN of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
        mpma_dsp_reg_DSM_CFG_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  subfield_mask << (MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_OFF + subfield_offset),
                                                  MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_MPMA_GAIN_get( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_MPMA_GAIN_get( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_MPMA_GAIN_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_MPMA_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_MPMA_GAIN_get", stop_bit, 3 );
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
    /* ((0x00003810 + (N) * 0x14) bits 3:0) bits 0:3 use field MPMA_GAIN of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_DSM_CFG_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    field_value = (reg_value & MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_MSK)
                  >> MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_MSK, MPMA_DSP_REG_DSM_CFG_REG_BIT_MPMA_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_MPMA_GAIN_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_dsp_field_CN_BASE_OFFSET_set( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_CN_BASE_OFFSET_set( mpma_dsp_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_BASE_OFFSET_set", N, 95);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_CN_BASE_OFFSET_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_CN_BASE_OFFSET_set", N, value );

    /* ((0x00003814 + (N) * 0x14) bits 28:24) bits 0:4 use field CN_BASE_OFFSET of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
    mpma_dsp_reg_CN_BASE_REG_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_MSK,
                                              MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_OFF,
                                              value);
}

static INLINE UINT32 mpma_dsp_field_CN_BASE_OFFSET_get( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CN_BASE_OFFSET_get( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_BASE_OFFSET_get", N, 95);
    /* ((0x00003814 + (N) * 0x14) bits 28:24) bits 0:4 use field CN_BASE_OFFSET of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
    reg_value = mpma_dsp_reg_CN_BASE_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_MSK) >> MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_CN_BASE_OFFSET_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_CN_BASE_OFFSET_set( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_CN_BASE_OFFSET_set( mpma_dsp_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_CN_BASE_OFFSET_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_CN_BASE_OFFSET_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_CN_BASE_OFFSET_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_CN_BASE_OFFSET_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003814 + (N) * 0x14) bits 28:24) bits 0:4 use field CN_BASE_OFFSET of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
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
        /* ((0x00003814 + (N) * 0x14) bits 28:24) bits 0:4 use field CN_BASE_OFFSET of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
        mpma_dsp_reg_CN_BASE_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  subfield_mask << (MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_OFF + subfield_offset),
                                                  MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_CN_BASE_OFFSET_get( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_CN_BASE_OFFSET_get( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_CN_BASE_OFFSET_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_CN_BASE_OFFSET_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_CN_BASE_OFFSET_get", stop_bit, 4 );
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
    /* ((0x00003814 + (N) * 0x14) bits 28:24) bits 0:4 use field CN_BASE_OFFSET of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
    reg_value = mpma_dsp_reg_CN_BASE_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    field_value = (reg_value & MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_MSK)
                  >> MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_MSK, MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFFSET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_CN_BASE_OFFSET_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_dsp_field_CN_BASE_set( mpma_dsp_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_CN_BASE_set( mpma_dsp_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_BASE_set", N, 95);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_dsp_field_CN_BASE_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_dsp_field_CN_BASE_set", N, value );

    /* ((0x00003814 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
    mpma_dsp_reg_CN_BASE_REG_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_MSK,
                                              MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFF,
                                              value);
}

static INLINE UINT32 mpma_dsp_field_CN_BASE_get( mpma_dsp_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CN_BASE_get( mpma_dsp_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CN_BASE_get", N, 95);
    /* ((0x00003814 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
    reg_value = mpma_dsp_reg_CN_BASE_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_MSK) >> MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_CN_BASE_get", N, value );

    return value;
}
static INLINE void mpma_dsp_field_range_CN_BASE_set( mpma_dsp_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_dsp_field_range_CN_BASE_set( mpma_dsp_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_CN_BASE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_CN_BASE_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_CN_BASE_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_CN_BASE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00003814 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
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
        /* ((0x00003814 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
        mpma_dsp_reg_CN_BASE_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  subfield_mask << (MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFF + subfield_offset),
                                                  MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_dsp_field_range_CN_BASE_get( mpma_dsp_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_range_CN_BASE_get( mpma_dsp_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_range_CN_BASE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_dsp_field_range_CN_BASE_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_dsp_field_range_CN_BASE_get", stop_bit, 23 );
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
    /* ((0x00003814 + (N) * 0x14) bits 23:0) bits 0:23 use field CN_BASE of register PMC_MPMA_DSP_REG_CN_BASE_REG index N=0..95 */
    reg_value = mpma_dsp_reg_CN_BASE_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    field_value = (reg_value & MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_MSK)
                  >> MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_MSK, MPMA_DSP_REG_CN_BASE_REG_BIT_CN_BASE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_dsp_field_range_CN_BASE_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size96
 * ==================================================================================
 */
static INLINE UINT32 mpma_dsp_field_INTEGRATOR_UNDERFLOW_get( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_INTEGRATOR_UNDERFLOW_get( mpma_dsp_buffer_t *b_ptr,
                                                              mpma_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_INTEGRATOR_UNDERFLOW_get", N, 95);
    /* ((0x00003810 + (N) * 0x14) bits 6) field INTEGRATOR_UNDERFLOW of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_DSM_CFG_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & MPMA_DSP_REG_DSM_CFG_REG_BIT_INTEGRATOR_UNDERFLOW_MSK) >> MPMA_DSP_REG_DSM_CFG_REG_BIT_INTEGRATOR_UNDERFLOW_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_INTEGRATOR_UNDERFLOW_get", N, value );

    return value;
}
static INLINE UINT32 mpma_dsp_field_INTEGRATOR_OVERFLOW_get( mpma_dsp_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_INTEGRATOR_OVERFLOW_get( mpma_dsp_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_INTEGRATOR_OVERFLOW_get", N, 95);
    /* ((0x00003810 + (N) * 0x14) bits 5) field INTEGRATOR_OVERFLOW of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_DSM_CFG_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & MPMA_DSP_REG_DSM_CFG_REG_BIT_INTEGRATOR_OVERFLOW_MSK) >> MPMA_DSP_REG_DSM_CFG_REG_BIT_INTEGRATOR_OVERFLOW_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_INTEGRATOR_OVERFLOW_get", N, value );

    return value;
}
static INLINE UINT32 mpma_dsp_field_CLAMP_UNDERFLOW_get( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CLAMP_UNDERFLOW_get( mpma_dsp_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CLAMP_UNDERFLOW_get", N, 95);
    /* ((0x00003810 + (N) * 0x14) bits 10) field CLAMP_UNDERFLOW of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_DSM_CFG_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & MPMA_DSP_REG_DSM_CFG_REG_BIT_CLAMP_UNDERFLOW_MSK) >> MPMA_DSP_REG_DSM_CFG_REG_BIT_CLAMP_UNDERFLOW_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_CLAMP_UNDERFLOW_get", N, value );

    return value;
}
static INLINE UINT32 mpma_dsp_field_CLAMP_OVERFLOW_get( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_dsp_field_CLAMP_OVERFLOW_get( mpma_dsp_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_dsp_field_CLAMP_OVERFLOW_get", N, 95);
    /* ((0x00003810 + (N) * 0x14) bits 9) field CLAMP_OVERFLOW of register PMC_MPMA_DSP_REG_DSM_CFG_REG index N=0..95 */
    reg_value = mpma_dsp_reg_DSM_CFG_REG_array_read( b_ptr,
                                                     h_ptr,
                                                     N);
    value = (reg_value & MPMA_DSP_REG_DSM_CFG_REG_BIT_CLAMP_OVERFLOW_MSK) >> MPMA_DSP_REG_DSM_CFG_REG_BIT_CLAMP_OVERFLOW_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_dsp_field_CLAMP_OVERFLOW_get", N, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MPMA_DSP_IO_INLINE_H */
