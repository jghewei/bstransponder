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
 *     and register accessor functions for the mpma block
 *****************************************************************************/
#ifndef _MPMA_IO_INLINE_H
#define _MPMA_IO_INLINE_H

#include "mpma_loc.h"
#include "mpma_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MPMA_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mpma
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
} mpma_buffer_t;
static INLINE void mpma_buffer_init( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mpma_buffer_init( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mpma_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mpma_buffer_flush( mpma_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mpma_buffer_flush( mpma_buffer_t *b_ptr )
{
    IOLOG( "mpma_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mpma_reg_read( mpma_buffer_t *b_ptr,
                                    mpma_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_read( mpma_buffer_t *b_ptr,
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
static INLINE void mpma_reg_write( mpma_buffer_t *b_ptr,
                                   mpma_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_write( mpma_buffer_t *b_ptr,
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

static INLINE void mpma_field_set( mpma_buffer_t *b_ptr,
                                   mpma_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_set( mpma_buffer_t *b_ptr,
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

static INLINE void mpma_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_action_on_write_field_set( mpma_buffer_t *b_ptr,
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

static INLINE void mpma_burst_read( mpma_buffer_t *b_ptr,
                                    mpma_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void mpma_burst_read( mpma_buffer_t *b_ptr,
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

static INLINE void mpma_burst_write( mpma_buffer_t *b_ptr,
                                     mpma_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void mpma_burst_write( mpma_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mpma_poll( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_poll( mpma_buffer_t *b_ptr,
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
 *  register access functions for mpma
 * ==================================================================================
 */

static INLINE void mpma_reg_GEN_CFG_write( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_GEN_CFG_write( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_GEN_CFG_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_GEN_CFG,
                    value);
}

static INLINE void mpma_reg_GEN_CFG_field_set( mpma_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_GEN_CFG_field_set( mpma_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_GEN_CFG_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_GEN_CFG,
                    mask,
                    PMC_CORE_REG_GEN_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_GEN_CFG_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_GEN_CFG_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_GEN_CFG);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_GEN_CFG_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_CSI_CFG_0_write( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_CSI_CFG_0_write( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_CSI_CFG_0_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CSI_CFG_0,
                    value);
}

static INLINE void mpma_reg_CSI_CFG_0_field_set( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_CSI_CFG_0_field_set( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_CSI_CFG_0_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CSI_CFG_0,
                    mask,
                    PMC_CORE_REG_CSI_CFG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_CSI_CFG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_CSI_CFG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_CSI_CFG_0);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_CSI_CFG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_CSI_CFG_1_write( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_CSI_CFG_1_write( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_CSI_CFG_1_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CSI_CFG_1,
                    value);
}

static INLINE void mpma_reg_CSI_CFG_1_field_set( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_CSI_CFG_1_field_set( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_CSI_CFG_1_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CSI_CFG_1,
                    mask,
                    PMC_CORE_REG_CSI_CFG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_CSI_CFG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_CSI_CFG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_CSI_CFG_1);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_CSI_CFG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_GEN_CFG_1_write( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_GEN_CFG_1_write( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_GEN_CFG_1_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_GEN_CFG_1,
                    value);
}

static INLINE void mpma_reg_GEN_CFG_1_field_set( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_GEN_CFG_1_field_set( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32 mask,
                                                 UINT32 ofs,
                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_GEN_CFG_1_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_GEN_CFG_1,
                    mask,
                    PMC_CORE_REG_GEN_CFG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_GEN_CFG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_GEN_CFG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_GEN_CFG_1);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_GEN_CFG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_CH_CFG_array_write( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_CH_CFG_array_write( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_CH_CFG_array_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CH_CFG(N),
                    value);
}

static INLINE void mpma_reg_CH_CFG_array_field_set( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_CH_CFG_array_field_set( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_CH_CFG_array_field_set", N, mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CH_CFG(N),
                    mask,
                    PMC_CORE_REG_CH_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_CH_CFG_array_read( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_CH_CFG_array_read( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_CH_CFG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpma_reg_CH_CFG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpma_reg_CFG_RAM_0_array_write( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_CFG_RAM_0_array_write( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_CFG_RAM_0_array_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CFG_RAM_0(N),
                    value);
}

static INLINE void mpma_reg_CFG_RAM_0_array_field_set( mpma_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_CFG_RAM_0_array_field_set( mpma_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_CFG_RAM_0_array_field_set", N, mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CFG_RAM_0(N),
                    mask,
                    PMC_CORE_REG_CFG_RAM_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_CFG_RAM_0_array_read( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_CFG_RAM_0_array_read( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_CFG_RAM_0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpma_reg_CFG_RAM_0_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_write( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_field_set( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_field_set", mask, ofs, value );
    mpma_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_0_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_0_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_REG_0_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_0_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_0_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpma_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_REG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_REG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_OVR_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_0_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_0_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpma_reg_PSD_FIFO_OVR_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpma_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_1_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_1_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_REG_1_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_1_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_1_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpma_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_REG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_REG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_OVR_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_1_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_1_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpma_reg_PSD_FIFO_OVR_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpma_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_2_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_2_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_REG_2_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_2_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_OVR_INT_REG_2_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_OVR_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpma_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_REG_2_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_OVR_INT_REG_2_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_OVR_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_2_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_OVR_INT_REG_2_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpma_reg_PSD_FIFO_OVR_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpma_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_0_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_0_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_REG_0_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_0_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_0_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpma_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_REG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_REG_0_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_UDR_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_0_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_0_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpma_reg_PSD_FIFO_UDR_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpma_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_1_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_1_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_REG_1_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_1_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_1_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpma_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_REG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_REG_1_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_UDR_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_1_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_1_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpma_reg_PSD_FIFO_UDR_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpma_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_2_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_2_write( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_REG_2_write", value );
    mpma_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2,
                    value);
}

static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_2_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_reg_PSD_FIFO_UDR_INT_REG_2_action_on_write_field_set( mpma_buffer_t *b_ptr,
                                                                              mpma_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpma_reg_PSD_FIFO_UDR_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpma_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_REG_2_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_reg_PSD_FIFO_UDR_INT_REG_2_read( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpma_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpma_reg_PSD_FIFO_UDR_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_2_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_reg_PSD_FIFO_UDR_INT_REG_2_poll( mpma_buffer_t *b_ptr,
                                                                         mpma_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpma_reg_PSD_FIFO_UDR_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpma_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mpma_field_BWR_IND_SEL_set( mpma_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_BWR_IND_SEL_set( mpma_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_BWR_IND_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpma_field_BWR_IND_SEL_set", value );

    /* (0x00000000 bits 30) field BWR_IND_SEL of register PMC_CORE_REG_GEN_CFG */
    mpma_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_BWR_IND_SEL_MSK,
                                CORE_REG_GEN_CFG_BIT_BWR_IND_SEL_OFF,
                                value);
}

static INLINE UINT32 mpma_field_BWR_IND_SEL_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_BWR_IND_SEL_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 30) field BWR_IND_SEL of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_BWR_IND_SEL_MSK) >> CORE_REG_GEN_CFG_BIT_BWR_IND_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_BWR_IND_SEL_get", value );

    return value;
}
static INLINE void mpma_field_FORCE_DEFAULT_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_FORCE_DEFAULT_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_FORCE_DEFAULT_PKT_SIZE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpma_field_FORCE_DEFAULT_PKT_SIZE_set", value );

    /* (0x00000000 bits 28) field FORCE_DEFAULT_PKT_SIZE of register PMC_CORE_REG_GEN_CFG */
    mpma_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_FORCE_DEFAULT_PKT_SIZE_MSK,
                                CORE_REG_GEN_CFG_BIT_FORCE_DEFAULT_PKT_SIZE_OFF,
                                value);
}

static INLINE UINT32 mpma_field_FORCE_DEFAULT_PKT_SIZE_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_FORCE_DEFAULT_PKT_SIZE_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 28) field FORCE_DEFAULT_PKT_SIZE of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_FORCE_DEFAULT_PKT_SIZE_MSK) >> CORE_REG_GEN_CFG_BIT_FORCE_DEFAULT_PKT_SIZE_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_FORCE_DEFAULT_PKT_SIZE_get", value );

    return value;
}
static INLINE void mpma_field_DEFAULT_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_DEFAULT_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_DEFAULT_PKT_SIZE_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "mpma_field_DEFAULT_PKT_SIZE_set", value );

    /* (0x00000000 bits 25:16) field DEFAULT_PKT_SIZE of register PMC_CORE_REG_GEN_CFG */
    mpma_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_MSK,
                                CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_OFF,
                                value);
}

static INLINE UINT32 mpma_field_DEFAULT_PKT_SIZE_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_DEFAULT_PKT_SIZE_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 25:16) field DEFAULT_PKT_SIZE of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_MSK) >> CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_DEFAULT_PKT_SIZE_get", value );

    return value;
}
static INLINE void mpma_field_range_DEFAULT_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_DEFAULT_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_DEFAULT_PKT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_DEFAULT_PKT_SIZE_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_DEFAULT_PKT_SIZE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 25:16) field DEFAULT_PKT_SIZE of register PMC_CORE_REG_GEN_CFG */
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
        /* (0x00000000 bits 25:16) field DEFAULT_PKT_SIZE of register PMC_CORE_REG_GEN_CFG */
        mpma_reg_GEN_CFG_field_set( b_ptr,
                                    h_ptr,
                                    subfield_mask << (CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_OFF + subfield_offset),
                                    CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_OFF + subfield_offset,
                                    value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_DEFAULT_PKT_SIZE_get( mpma_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_DEFAULT_PKT_SIZE_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_DEFAULT_PKT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_DEFAULT_PKT_SIZE_get", stop_bit, 9 );
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
    /* (0x00000000 bits 25:16) field DEFAULT_PKT_SIZE of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_MSK)
                  >> CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_MSK, CORE_REG_GEN_CFG_BIT_DEFAULT_PKT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_DEFAULT_PKT_SIZE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_BUS_WIDTH_set( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_BUS_WIDTH_set( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_BUS_WIDTH_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mpma_field_BUS_WIDTH_set", value );

    /* (0x00000000 bits 13:12) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
    mpma_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK,
                                CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF,
                                value);
}

static INLINE UINT32 mpma_field_BUS_WIDTH_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_BUS_WIDTH_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 13:12) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK) >> CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_BUS_WIDTH_get", value );

    return value;
}
static INLINE void mpma_field_range_BUS_WIDTH_set( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_BUS_WIDTH_set( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_BUS_WIDTH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_BUS_WIDTH_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_BUS_WIDTH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 13:12) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
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
        /* (0x00000000 bits 13:12) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
        mpma_reg_GEN_CFG_field_set( b_ptr,
                                    h_ptr,
                                    subfield_mask << (CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF + subfield_offset),
                                    CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF + subfield_offset,
                                    value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_BUS_WIDTH_get( mpma_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_BUS_WIDTH_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_BUS_WIDTH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_BUS_WIDTH_get", stop_bit, 1 );
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
    /* (0x00000000 bits 13:12) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK)
                  >> CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK, CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_BUS_WIDTH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_DPI_DYNAMIC_LS_EN_set( mpma_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_DPI_DYNAMIC_LS_EN_set( mpma_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_DPI_DYNAMIC_LS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpma_field_DPI_DYNAMIC_LS_EN_set", value );

    /* (0x00000000 bits 8) field DPI_DYNAMIC_LS_EN of register PMC_CORE_REG_GEN_CFG */
    mpma_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_OFF,
                                value);
}

static INLINE UINT32 mpma_field_DPI_DYNAMIC_LS_EN_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_DPI_DYNAMIC_LS_EN_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 8) field DPI_DYNAMIC_LS_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_MSK) >> CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_DPI_DYNAMIC_LS_EN_get", value );

    return value;
}
static INLINE void mpma_field_DCI_DYNAMIC_LS_EN_set( mpma_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_DCI_DYNAMIC_LS_EN_set( mpma_buffer_t *b_ptr,
                                                     mpma_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_DCI_DYNAMIC_LS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpma_field_DCI_DYNAMIC_LS_EN_set", value );

    /* (0x00000000 bits 4) field DCI_DYNAMIC_LS_EN of register PMC_CORE_REG_GEN_CFG */
    mpma_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_OFF,
                                value);
}

static INLINE UINT32 mpma_field_DCI_DYNAMIC_LS_EN_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_DCI_DYNAMIC_LS_EN_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field DCI_DYNAMIC_LS_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_MSK) >> CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_DCI_DYNAMIC_LS_EN_get", value );

    return value;
}
static INLINE void mpma_field_MPMA_EN_set( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_MPMA_EN_set( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_MPMA_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpma_field_MPMA_EN_set", value );

    /* (0x00000000 bits 0) field MPMA_EN of register PMC_CORE_REG_GEN_CFG */
    mpma_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_MPMA_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_MPMA_EN_OFF,
                                value);
}

static INLINE UINT32 mpma_field_MPMA_EN_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_MPMA_EN_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field MPMA_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpma_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_MPMA_EN_MSK) >> CORE_REG_GEN_CFG_BIT_MPMA_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_MPMA_EN_get", value );

    return value;
}
static INLINE void mpma_field_CSI_SF_set( mpma_buffer_t *b_ptr,
                                          mpma_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_CSI_SF_set( mpma_buffer_t *b_ptr,
                                          mpma_handle_t *h_ptr,
                                          UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_CSI_SF_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mpma_field_CSI_SF_set", value );

    /* (0x00000004 bits 28:24) field CSI_SF of register PMC_CORE_REG_CSI_CFG_0 */
    mpma_reg_CSI_CFG_0_field_set( b_ptr,
                                  h_ptr,
                                  CORE_REG_CSI_CFG_0_BIT_CSI_SF_MSK,
                                  CORE_REG_CSI_CFG_0_BIT_CSI_SF_OFF,
                                  value);
}

static INLINE UINT32 mpma_field_CSI_SF_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_CSI_SF_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 28:24) field CSI_SF of register PMC_CORE_REG_CSI_CFG_0 */
    reg_value = mpma_reg_CSI_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_CSI_CFG_0_BIT_CSI_SF_MSK) >> CORE_REG_CSI_CFG_0_BIT_CSI_SF_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_CSI_SF_get", value );

    return value;
}
static INLINE void mpma_field_range_CSI_SF_set( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_CSI_SF_set( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_SF_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_SF_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_SF_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 28:24) field CSI_SF of register PMC_CORE_REG_CSI_CFG_0 */
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
        /* (0x00000004 bits 28:24) field CSI_SF of register PMC_CORE_REG_CSI_CFG_0 */
        mpma_reg_CSI_CFG_0_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (CORE_REG_CSI_CFG_0_BIT_CSI_SF_OFF + subfield_offset),
                                      CORE_REG_CSI_CFG_0_BIT_CSI_SF_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_CSI_SF_get( mpma_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_CSI_SF_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_SF_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_SF_get", stop_bit, 4 );
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
    /* (0x00000004 bits 28:24) field CSI_SF of register PMC_CORE_REG_CSI_CFG_0 */
    reg_value = mpma_reg_CSI_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CFG_0_BIT_CSI_SF_MSK)
                  >> CORE_REG_CSI_CFG_0_BIT_CSI_SF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CFG_0_BIT_CSI_SF_MSK, CORE_REG_CSI_CFG_0_BIT_CSI_SF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_SF_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_CSI_SD_set( mpma_buffer_t *b_ptr,
                                          mpma_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_CSI_SD_set( mpma_buffer_t *b_ptr,
                                          mpma_handle_t *h_ptr,
                                          UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_CSI_SD_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mpma_field_CSI_SD_set", value );

    /* (0x00000004 bits 20:16) field CSI_SD of register PMC_CORE_REG_CSI_CFG_0 */
    mpma_reg_CSI_CFG_0_field_set( b_ptr,
                                  h_ptr,
                                  CORE_REG_CSI_CFG_0_BIT_CSI_SD_MSK,
                                  CORE_REG_CSI_CFG_0_BIT_CSI_SD_OFF,
                                  value);
}

static INLINE UINT32 mpma_field_CSI_SD_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_CSI_SD_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 20:16) field CSI_SD of register PMC_CORE_REG_CSI_CFG_0 */
    reg_value = mpma_reg_CSI_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_CSI_CFG_0_BIT_CSI_SD_MSK) >> CORE_REG_CSI_CFG_0_BIT_CSI_SD_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_CSI_SD_get", value );

    return value;
}
static INLINE void mpma_field_range_CSI_SD_set( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_CSI_SD_set( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_SD_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_SD_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_SD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 20:16) field CSI_SD of register PMC_CORE_REG_CSI_CFG_0 */
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
        /* (0x00000004 bits 20:16) field CSI_SD of register PMC_CORE_REG_CSI_CFG_0 */
        mpma_reg_CSI_CFG_0_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (CORE_REG_CSI_CFG_0_BIT_CSI_SD_OFF + subfield_offset),
                                      CORE_REG_CSI_CFG_0_BIT_CSI_SD_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_CSI_SD_get( mpma_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_CSI_SD_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_SD_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_SD_get", stop_bit, 4 );
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
    /* (0x00000004 bits 20:16) field CSI_SD of register PMC_CORE_REG_CSI_CFG_0 */
    reg_value = mpma_reg_CSI_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CFG_0_BIT_CSI_SD_MSK)
                  >> CORE_REG_CSI_CFG_0_BIT_CSI_SD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CFG_0_BIT_CSI_SD_MSK, CORE_REG_CSI_CFG_0_BIT_CSI_SD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_SD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_CSI_NORMAL_set( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_CSI_NORMAL_set( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_CSI_NORMAL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mpma_field_CSI_NORMAL_set", value );

    /* (0x00000004 bits 12:8) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG_0 */
    mpma_reg_CSI_CFG_0_field_set( b_ptr,
                                  h_ptr,
                                  CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_MSK,
                                  CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_OFF,
                                  value);
}

static INLINE UINT32 mpma_field_CSI_NORMAL_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_CSI_NORMAL_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 12:8) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG_0 */
    reg_value = mpma_reg_CSI_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_MSK) >> CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_CSI_NORMAL_get", value );

    return value;
}
static INLINE void mpma_field_range_CSI_NORMAL_set( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_CSI_NORMAL_set( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_NORMAL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_NORMAL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_NORMAL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 12:8) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG_0 */
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
        /* (0x00000004 bits 12:8) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG_0 */
        mpma_reg_CSI_CFG_0_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_OFF + subfield_offset),
                                      CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_CSI_NORMAL_get( mpma_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_CSI_NORMAL_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_NORMAL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_NORMAL_get", stop_bit, 4 );
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
    /* (0x00000004 bits 12:8) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG_0 */
    reg_value = mpma_reg_CSI_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_MSK)
                  >> CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_MSK, CORE_REG_CSI_CFG_0_BIT_CSI_NORMAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_NORMAL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_CSI_FORCE_TO_set( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_CSI_FORCE_TO_set( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_CSI_FORCE_TO_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mpma_field_CSI_FORCE_TO_set", value );

    /* (0x00000004 bits 4:0) field CSI_FORCE_TO of register PMC_CORE_REG_CSI_CFG_0 */
    mpma_reg_CSI_CFG_0_field_set( b_ptr,
                                  h_ptr,
                                  CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_MSK,
                                  CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_OFF,
                                  value);
}

static INLINE UINT32 mpma_field_CSI_FORCE_TO_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_CSI_FORCE_TO_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 4:0) field CSI_FORCE_TO of register PMC_CORE_REG_CSI_CFG_0 */
    reg_value = mpma_reg_CSI_CFG_0_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_MSK) >> CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_CSI_FORCE_TO_get", value );

    return value;
}
static INLINE void mpma_field_range_CSI_FORCE_TO_set( mpma_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_CSI_FORCE_TO_set( mpma_buffer_t *b_ptr,
                                                      mpma_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_FORCE_TO_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_FORCE_TO_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_FORCE_TO_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 4:0) field CSI_FORCE_TO of register PMC_CORE_REG_CSI_CFG_0 */
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
        /* (0x00000004 bits 4:0) field CSI_FORCE_TO of register PMC_CORE_REG_CSI_CFG_0 */
        mpma_reg_CSI_CFG_0_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_OFF + subfield_offset),
                                      CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_CSI_FORCE_TO_get( mpma_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_CSI_FORCE_TO_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_FORCE_TO_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_FORCE_TO_get", stop_bit, 4 );
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
    /* (0x00000004 bits 4:0) field CSI_FORCE_TO of register PMC_CORE_REG_CSI_CFG_0 */
    reg_value = mpma_reg_CSI_CFG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_MSK)
                  >> CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_MSK, CORE_REG_CSI_CFG_0_BIT_CSI_FORCE_TO_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_FORCE_TO_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_CSI_FORCE_AWAY_set( mpma_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_CSI_FORCE_AWAY_set( mpma_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_CSI_FORCE_AWAY_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mpma_field_CSI_FORCE_AWAY_set", value );

    /* (0x00000008 bits 20:16) field CSI_FORCE_AWAY of register PMC_CORE_REG_CSI_CFG_1 */
    mpma_reg_CSI_CFG_1_field_set( b_ptr,
                                  h_ptr,
                                  CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_MSK,
                                  CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_OFF,
                                  value);
}

static INLINE UINT32 mpma_field_CSI_FORCE_AWAY_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_CSI_FORCE_AWAY_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 20:16) field CSI_FORCE_AWAY of register PMC_CORE_REG_CSI_CFG_1 */
    reg_value = mpma_reg_CSI_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_MSK) >> CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_CSI_FORCE_AWAY_get", value );

    return value;
}
static INLINE void mpma_field_range_CSI_FORCE_AWAY_set( mpma_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_CSI_FORCE_AWAY_set( mpma_buffer_t *b_ptr,
                                                        mpma_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_FORCE_AWAY_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_FORCE_AWAY_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_FORCE_AWAY_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 20:16) field CSI_FORCE_AWAY of register PMC_CORE_REG_CSI_CFG_1 */
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
        /* (0x00000008 bits 20:16) field CSI_FORCE_AWAY of register PMC_CORE_REG_CSI_CFG_1 */
        mpma_reg_CSI_CFG_1_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_OFF + subfield_offset),
                                      CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_CSI_FORCE_AWAY_get( mpma_buffer_t *b_ptr,
                                                          mpma_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_CSI_FORCE_AWAY_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_FORCE_AWAY_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_FORCE_AWAY_get", stop_bit, 4 );
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
    /* (0x00000008 bits 20:16) field CSI_FORCE_AWAY of register PMC_CORE_REG_CSI_CFG_1 */
    reg_value = mpma_reg_CSI_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_MSK)
                  >> CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_MSK, CORE_REG_CSI_CFG_1_BIT_CSI_FORCE_AWAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_FORCE_AWAY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_CSI_SSF_set( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_CSI_SSF_set( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_CSI_SSF_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mpma_field_CSI_SSF_set", value );

    /* (0x00000008 bits 4:0) field CSI_SSF of register PMC_CORE_REG_CSI_CFG_1 */
    mpma_reg_CSI_CFG_1_field_set( b_ptr,
                                  h_ptr,
                                  CORE_REG_CSI_CFG_1_BIT_CSI_SSF_MSK,
                                  CORE_REG_CSI_CFG_1_BIT_CSI_SSF_OFF,
                                  value);
}

static INLINE UINT32 mpma_field_CSI_SSF_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_CSI_SSF_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 4:0) field CSI_SSF of register PMC_CORE_REG_CSI_CFG_1 */
    reg_value = mpma_reg_CSI_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_CSI_CFG_1_BIT_CSI_SSF_MSK) >> CORE_REG_CSI_CFG_1_BIT_CSI_SSF_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_CSI_SSF_get", value );

    return value;
}
static INLINE void mpma_field_range_CSI_SSF_set( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_CSI_SSF_set( mpma_buffer_t *b_ptr,
                                                 mpma_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_SSF_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_SSF_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_SSF_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 4:0) field CSI_SSF of register PMC_CORE_REG_CSI_CFG_1 */
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
        /* (0x00000008 bits 4:0) field CSI_SSF of register PMC_CORE_REG_CSI_CFG_1 */
        mpma_reg_CSI_CFG_1_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (CORE_REG_CSI_CFG_1_BIT_CSI_SSF_OFF + subfield_offset),
                                      CORE_REG_CSI_CFG_1_BIT_CSI_SSF_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_CSI_SSF_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_CSI_SSF_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_CSI_SSF_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_CSI_SSF_get", stop_bit, 4 );
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
    /* (0x00000008 bits 4:0) field CSI_SSF of register PMC_CORE_REG_CSI_CFG_1 */
    reg_value = mpma_reg_CSI_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CFG_1_BIT_CSI_SSF_MSK)
                  >> CORE_REG_CSI_CFG_1_BIT_CSI_SSF_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CFG_1_BIT_CSI_SSF_MSK, CORE_REG_CSI_CFG_1_BIT_CSI_SSF_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_CSI_SSF_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_PSD_FIFO_FULL_THRES_set( mpma_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_PSD_FIFO_FULL_THRES_set( mpma_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_PSD_FIFO_FULL_THRES_set", value, 63);
    IOLOG( "%s <= 0x%08x", "mpma_field_PSD_FIFO_FULL_THRES_set", value );

    /* (0x0000000c bits 5:0) field PSD_FIFO_FULL_THRES of register PMC_CORE_REG_GEN_CFG_1 */
    mpma_reg_GEN_CFG_1_field_set( b_ptr,
                                  h_ptr,
                                  CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_MSK,
                                  CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_OFF,
                                  value);
}

static INLINE UINT32 mpma_field_PSD_FIFO_FULL_THRES_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_PSD_FIFO_FULL_THRES_get( mpma_buffer_t *b_ptr, mpma_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 5:0) field PSD_FIFO_FULL_THRES of register PMC_CORE_REG_GEN_CFG_1 */
    reg_value = mpma_reg_GEN_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_MSK) >> CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_OFF;
    IOLOG( "%s -> 0x%08x", "mpma_field_PSD_FIFO_FULL_THRES_get", value );

    return value;
}
static INLINE void mpma_field_range_PSD_FIFO_FULL_THRES_set( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_PSD_FIFO_FULL_THRES_set( mpma_buffer_t *b_ptr,
                                                             mpma_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_PSD_FIFO_FULL_THRES_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_PSD_FIFO_FULL_THRES_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_PSD_FIFO_FULL_THRES_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 5:0) field PSD_FIFO_FULL_THRES of register PMC_CORE_REG_GEN_CFG_1 */
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
        /* (0x0000000c bits 5:0) field PSD_FIFO_FULL_THRES of register PMC_CORE_REG_GEN_CFG_1 */
        mpma_reg_GEN_CFG_1_field_set( b_ptr,
                                      h_ptr,
                                      subfield_mask << (CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_OFF + subfield_offset),
                                      CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_OFF + subfield_offset,
                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_PSD_FIFO_FULL_THRES_get( mpma_buffer_t *b_ptr,
                                                               mpma_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_PSD_FIFO_FULL_THRES_get( mpma_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_PSD_FIFO_FULL_THRES_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_PSD_FIFO_FULL_THRES_get", stop_bit, 5 );
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
    /* (0x0000000c bits 5:0) field PSD_FIFO_FULL_THRES of register PMC_CORE_REG_GEN_CFG_1 */
    reg_value = mpma_reg_GEN_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_MSK)
                  >> CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_MSK, CORE_REG_GEN_CFG_1_BIT_PSD_FIFO_FULL_THRES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_PSD_FIFO_FULL_THRES_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void mpma_field_BWR_IND_set( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_BWR_IND_set( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_BWR_IND_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_BWR_IND_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_BWR_IND_set", N, value );

    /* ((0x00000030 + (N) * 0x8) bits 30) field BWR_IND of register PMC_CORE_REG_CH_CFG index N=0..95 */
    mpma_reg_CH_CFG_array_field_set( b_ptr,
                                     h_ptr,
                                     N,
                                     CORE_REG_CH_CFG_BIT_BWR_IND_MSK,
                                     CORE_REG_CH_CFG_BIT_BWR_IND_OFF,
                                     value);
}

static INLINE UINT32 mpma_field_BWR_IND_get( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_BWR_IND_get( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_BWR_IND_get", N, 95);
    /* ((0x00000030 + (N) * 0x8) bits 30) field BWR_IND of register PMC_CORE_REG_CH_CFG index N=0..95 */
    reg_value = mpma_reg_CH_CFG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CH_CFG_BIT_BWR_IND_MSK) >> CORE_REG_CH_CFG_BIT_BWR_IND_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_BWR_IND_get", N, value );

    return value;
}
static INLINE void mpma_field_FORCE_AWAY_set( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_FORCE_AWAY_set( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_FORCE_AWAY_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_FORCE_AWAY_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_FORCE_AWAY_set", N, value );

    /* ((0x00000030 + (N) * 0x8) bits 28) field FORCE_AWAY of register PMC_CORE_REG_CH_CFG index N=0..95 */
    mpma_reg_CH_CFG_array_field_set( b_ptr,
                                     h_ptr,
                                     N,
                                     CORE_REG_CH_CFG_BIT_FORCE_AWAY_MSK,
                                     CORE_REG_CH_CFG_BIT_FORCE_AWAY_OFF,
                                     value);
}

static INLINE UINT32 mpma_field_FORCE_AWAY_get( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_FORCE_AWAY_get( mpma_buffer_t *b_ptr,
                                                mpma_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_FORCE_AWAY_get", N, 95);
    /* ((0x00000030 + (N) * 0x8) bits 28) field FORCE_AWAY of register PMC_CORE_REG_CH_CFG index N=0..95 */
    reg_value = mpma_reg_CH_CFG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CH_CFG_BIT_FORCE_AWAY_MSK) >> CORE_REG_CH_CFG_BIT_FORCE_AWAY_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_FORCE_AWAY_get", N, value );

    return value;
}
static INLINE void mpma_field_FORCE_TO_set( mpma_buffer_t *b_ptr,
                                            mpma_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_FORCE_TO_set( mpma_buffer_t *b_ptr,
                                            mpma_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_FORCE_TO_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_FORCE_TO_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_FORCE_TO_set", N, value );

    /* ((0x00000030 + (N) * 0x8) bits 24) field FORCE_TO of register PMC_CORE_REG_CH_CFG index N=0..95 */
    mpma_reg_CH_CFG_array_field_set( b_ptr,
                                     h_ptr,
                                     N,
                                     CORE_REG_CH_CFG_BIT_FORCE_TO_MSK,
                                     CORE_REG_CH_CFG_BIT_FORCE_TO_OFF,
                                     value);
}

static INLINE UINT32 mpma_field_FORCE_TO_get( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_FORCE_TO_get( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_FORCE_TO_get", N, 95);
    /* ((0x00000030 + (N) * 0x8) bits 24) field FORCE_TO of register PMC_CORE_REG_CH_CFG index N=0..95 */
    reg_value = mpma_reg_CH_CFG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CH_CFG_BIT_FORCE_TO_MSK) >> CORE_REG_CH_CFG_BIT_FORCE_TO_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_FORCE_TO_get", N, value );

    return value;
}
static INLINE void mpma_field_DEFAULT_PKT_SIZE_EN_set( mpma_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_DEFAULT_PKT_SIZE_EN_set( mpma_buffer_t *b_ptr,
                                                       mpma_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_DEFAULT_PKT_SIZE_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_DEFAULT_PKT_SIZE_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_DEFAULT_PKT_SIZE_EN_set", N, value );

    /* ((0x00000030 + (N) * 0x8) bits 20) field DEFAULT_PKT_SIZE_EN of register PMC_CORE_REG_CH_CFG index N=0..95 */
    mpma_reg_CH_CFG_array_field_set( b_ptr,
                                     h_ptr,
                                     N,
                                     CORE_REG_CH_CFG_BIT_DEFAULT_PKT_SIZE_EN_MSK,
                                     CORE_REG_CH_CFG_BIT_DEFAULT_PKT_SIZE_EN_OFF,
                                     value);
}

static INLINE UINT32 mpma_field_DEFAULT_PKT_SIZE_EN_get( mpma_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_DEFAULT_PKT_SIZE_EN_get( mpma_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_DEFAULT_PKT_SIZE_EN_get", N, 95);
    /* ((0x00000030 + (N) * 0x8) bits 20) field DEFAULT_PKT_SIZE_EN of register PMC_CORE_REG_CH_CFG index N=0..95 */
    reg_value = mpma_reg_CH_CFG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CH_CFG_BIT_DEFAULT_PKT_SIZE_EN_MSK) >> CORE_REG_CH_CFG_BIT_DEFAULT_PKT_SIZE_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_DEFAULT_PKT_SIZE_EN_get", N, value );

    return value;
}
static INLINE void mpma_field_CH_EN_set( mpma_buffer_t *b_ptr,
                                         mpma_handle_t *h_ptr,
                                         UINT32  N,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_CH_EN_set( mpma_buffer_t *b_ptr,
                                         mpma_handle_t *h_ptr,
                                         UINT32  N,
                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_CH_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_CH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_CH_EN_set", N, value );

    /* ((0x00000030 + (N) * 0x8) bits 16) field CH_EN of register PMC_CORE_REG_CH_CFG index N=0..95 */
    mpma_reg_CH_CFG_array_field_set( b_ptr,
                                     h_ptr,
                                     N,
                                     CORE_REG_CH_CFG_BIT_CH_EN_MSK,
                                     CORE_REG_CH_CFG_BIT_CH_EN_OFF,
                                     value);
}

static INLINE UINT32 mpma_field_CH_EN_get( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_CH_EN_get( mpma_buffer_t *b_ptr,
                                           mpma_handle_t *h_ptr,
                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_CH_EN_get", N, 95);
    /* ((0x00000030 + (N) * 0x8) bits 16) field CH_EN of register PMC_CORE_REG_CH_CFG index N=0..95 */
    reg_value = mpma_reg_CH_CFG_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CH_CFG_BIT_CH_EN_MSK) >> CORE_REG_CH_CFG_BIT_CH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_CH_EN_get", N, value );

    return value;
}
static INLINE void mpma_field_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                            mpma_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                            mpma_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_PKT_SIZE_set", N, 95);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_PKT_SIZE_set", value, 1023);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_PKT_SIZE_set", N, value );

    /* ((0x00000400 + (N) * 0x4) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    mpma_reg_CFG_RAM_0_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_MSK,
                                        CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_OFF,
                                        value);
}

static INLINE UINT32 mpma_field_PKT_SIZE_get( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_PKT_SIZE_get( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_PKT_SIZE_get", N, 95);
    /* ((0x00000400 + (N) * 0x4) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    reg_value = mpma_reg_CFG_RAM_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_MSK) >> CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_PKT_SIZE_get", N, value );

    return value;
}
static INLINE void mpma_field_range_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_PKT_SIZE_set( mpma_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_range_PKT_SIZE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_PKT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_PKT_SIZE_set", stop_bit, 9 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_PKT_SIZE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x4) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x4) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
        mpma_reg_CFG_RAM_0_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_PKT_SIZE_get( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_PKT_SIZE_get( mpma_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_range_PKT_SIZE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_PKT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_PKT_SIZE_get", stop_bit, 9 );
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
    /* ((0x00000400 + (N) * 0x4) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    reg_value = mpma_reg_CFG_RAM_0_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_MSK)
                  >> CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_MSK, CORE_REG_CFG_RAM_0_BIT_PKT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_PKT_SIZE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_PSD_FIFO_REFILL_THRES_set( mpma_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_PSD_FIFO_REFILL_THRES_set( mpma_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_PSD_FIFO_REFILL_THRES_set", N, 95);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_PSD_FIFO_REFILL_THRES_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_PSD_FIFO_REFILL_THRES_set", N, value );

    /* ((0x00000400 + (N) * 0x4) bits 12:8) field PSD_FIFO_REFILL_THRES of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    mpma_reg_CFG_RAM_0_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_MSK,
                                        CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF,
                                        value);
}

static INLINE UINT32 mpma_field_PSD_FIFO_REFILL_THRES_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_PSD_FIFO_REFILL_THRES_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_PSD_FIFO_REFILL_THRES_get", N, 95);
    /* ((0x00000400 + (N) * 0x4) bits 12:8) field PSD_FIFO_REFILL_THRES of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    reg_value = mpma_reg_CFG_RAM_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_MSK) >> CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_PSD_FIFO_REFILL_THRES_get", N, value );

    return value;
}
static INLINE void mpma_field_range_PSD_FIFO_REFILL_THRES_set( mpma_buffer_t *b_ptr,
                                                               mpma_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_PSD_FIFO_REFILL_THRES_set( mpma_buffer_t *b_ptr,
                                                               mpma_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_range_PSD_FIFO_REFILL_THRES_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_PSD_FIFO_REFILL_THRES_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_PSD_FIFO_REFILL_THRES_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_PSD_FIFO_REFILL_THRES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x4) bits 12:8) field PSD_FIFO_REFILL_THRES of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x4) bits 12:8) field PSD_FIFO_REFILL_THRES of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
        mpma_reg_CFG_RAM_0_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_PSD_FIFO_REFILL_THRES_get( mpma_buffer_t *b_ptr,
                                                                 mpma_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_PSD_FIFO_REFILL_THRES_get( mpma_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_range_PSD_FIFO_REFILL_THRES_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_PSD_FIFO_REFILL_THRES_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_PSD_FIFO_REFILL_THRES_get", stop_bit, 4 );
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
    /* ((0x00000400 + (N) * 0x4) bits 12:8) field PSD_FIFO_REFILL_THRES of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    reg_value = mpma_reg_CFG_RAM_0_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_MSK)
                  >> CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_MSK, CORE_REG_CFG_RAM_0_BIT_PSD_FIFO_REFILL_THRES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_PSD_FIFO_REFILL_THRES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_field_TS_ENABLE_set( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_TS_ENABLE_set( mpma_buffer_t *b_ptr,
                                             mpma_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_TS_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_TS_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_TS_ENABLE_set", N, value );

    /* ((0x00000400 + (N) * 0x4) bits 4) field TS_ENABLE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    mpma_reg_CFG_RAM_0_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_0_BIT_TS_ENABLE_MSK,
                                        CORE_REG_CFG_RAM_0_BIT_TS_ENABLE_OFF,
                                        value);
}

static INLINE UINT32 mpma_field_TS_ENABLE_get( mpma_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_TS_ENABLE_get( mpma_buffer_t *b_ptr,
                                               mpma_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_TS_ENABLE_get", N, 95);
    /* ((0x00000400 + (N) * 0x4) bits 4) field TS_ENABLE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    reg_value = mpma_reg_CFG_RAM_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_0_BIT_TS_ENABLE_MSK) >> CORE_REG_CFG_RAM_0_BIT_TS_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_TS_ENABLE_get", N, value );

    return value;
}
static INLINE void mpma_field_MKR_MODE_set( mpma_buffer_t *b_ptr,
                                            mpma_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_MKR_MODE_set( mpma_buffer_t *b_ptr,
                                            mpma_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_MKR_MODE_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpma_field_MKR_MODE_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "mpma_field_MKR_MODE_set", N, value );

    /* ((0x00000400 + (N) * 0x4) bits 1:0) field MKR_MODE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    mpma_reg_CFG_RAM_0_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_0_BIT_MKR_MODE_MSK,
                                        CORE_REG_CFG_RAM_0_BIT_MKR_MODE_OFF,
                                        value);
}

static INLINE UINT32 mpma_field_MKR_MODE_get( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_MKR_MODE_get( mpma_buffer_t *b_ptr,
                                              mpma_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_MKR_MODE_get", N, 95);
    /* ((0x00000400 + (N) * 0x4) bits 1:0) field MKR_MODE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    reg_value = mpma_reg_CFG_RAM_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_0_BIT_MKR_MODE_MSK) >> CORE_REG_CFG_RAM_0_BIT_MKR_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpma_field_MKR_MODE_get", N, value );

    return value;
}
static INLINE void mpma_field_range_MKR_MODE_set( mpma_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_field_range_MKR_MODE_set( mpma_buffer_t *b_ptr,
                                                  mpma_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_range_MKR_MODE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_MKR_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_MKR_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_MKR_MODE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000400 + (N) * 0x4) bits 1:0) field MKR_MODE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
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
        /* ((0x00000400 + (N) * 0x4) bits 1:0) field MKR_MODE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
        mpma_reg_CFG_RAM_0_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_0_BIT_MKR_MODE_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_0_BIT_MKR_MODE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_field_range_MKR_MODE_get( mpma_buffer_t *b_ptr,
                                                    mpma_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_field_range_MKR_MODE_get( mpma_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpma_field_range_MKR_MODE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_field_range_MKR_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_field_range_MKR_MODE_get", stop_bit, 1 );
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
    /* ((0x00000400 + (N) * 0x4) bits 1:0) field MKR_MODE of register PMC_CORE_REG_CFG_RAM_0 index N=0..95 */
    reg_value = mpma_reg_CFG_RAM_0_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_0_BIT_MKR_MODE_MSK)
                  >> CORE_REG_CFG_RAM_0_BIT_MKR_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_0_BIT_MKR_MODE_MSK, CORE_REG_CFG_RAM_0_BIT_MKR_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpma_field_range_MKR_MODE_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void mpma_lfield_PSD_FIFO_OVR_E_set( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpma_lfield_PSD_FIFO_OVR_E_set( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_OVR_E_set", value[2] , value[1] , value[0] );

    /* (0x00000640 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0 */
    mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_field_set( b_ptr,
                                                  h_ptr,
                                                  CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_MSK,
                                                  CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_OFF,
                                                  value[0]);

    /* (0x00000644 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1 */
    mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_MSK,
                                                  CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_OFF,
                                                  value[1]);

    /* (0x00000648 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2 */
    mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_field_set( b_ptr,
                                                  h_ptr,
                                                  CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_MSK,
                                                  CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_OFF,
                                                  value[2]);
}

static INLINE void mpma_lfield_PSD_FIFO_OVR_E_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpma_lfield_PSD_FIFO_OVR_E_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00000640 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0 */
    reg_value = mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_MSK) >> CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_OFF;
    value[0] |= field_value;

    /* (0x00000644 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1 */
    reg_value = mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_MSK) >> CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_OFF;
    value[1] |= field_value;

    /* (0x00000648 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2 */
    reg_value = mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_MSK) >> CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_OVR_E_get", value[2] , value[1] , value[0] );

}
static INLINE void mpma_lfield_range_PSD_FIFO_OVR_E_set( mpma_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_lfield_range_PSD_FIFO_OVR_E_set( mpma_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_OVR_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_OVR_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_OVR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000640 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0 */
    if (start_bit <= 31) {
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
        /* (0x00000640 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0 */
        mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_OFF + subfield_offset),
                                                      CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000644 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000644 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1 */
        mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_OFF + subfield_offset),
                                                      CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000648 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000648 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2 */
        mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_OFF + subfield_offset),
                                                      CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_lfield_range_PSD_FIFO_OVR_E_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_lfield_range_PSD_FIFO_OVR_E_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_OVR_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_OVR_E_get", stop_bit, 95 );
    if (start_bit <= 31) {
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
    /* (0x00000640 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0 */
        reg_value = mpma_reg_PSD_FIFO_OVR_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_MSK)
                      >> CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_MSK, CORE_REG_PSD_FIFO_OVR_INT_EN_REG_0_BIT_PSD_FIFO_OVR_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000644 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1 */
        reg_value = mpma_reg_PSD_FIFO_OVR_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_MSK)
                      >> CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_MSK, CORE_REG_PSD_FIFO_OVR_INT_EN_REG_1_BIT_PSD_FIFO_OVR_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000648 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_E of register PMC_CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2 */
        reg_value = mpma_reg_PSD_FIFO_OVR_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_MSK)
                      >> CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_MSK, CORE_REG_PSD_FIFO_OVR_INT_EN_REG_2_BIT_PSD_FIFO_OVR_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_OVR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpma_lfield_PSD_FIFO_UDR_E_set( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpma_lfield_PSD_FIFO_UDR_E_set( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_UDR_E_set", value[2] , value[1] , value[0] );

    /* (0x0000064c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0 */
    mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_field_set( b_ptr,
                                                  h_ptr,
                                                  CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_MSK,
                                                  CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_OFF,
                                                  value[0]);

    /* (0x00000650 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1 */
    mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_MSK,
                                                  CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_OFF,
                                                  value[1]);

    /* (0x00000654 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2 */
    mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_field_set( b_ptr,
                                                  h_ptr,
                                                  CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_MSK,
                                                  CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_OFF,
                                                  value[2]);
}

static INLINE void mpma_lfield_PSD_FIFO_UDR_E_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpma_lfield_PSD_FIFO_UDR_E_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000064c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0 */
    reg_value = mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_MSK) >> CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_OFF;
    value[0] |= field_value;

    /* (0x00000650 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1 */
    reg_value = mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_MSK) >> CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_OFF;
    value[1] |= field_value;

    /* (0x00000654 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2 */
    reg_value = mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_MSK) >> CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_UDR_E_get", value[2] , value[1] , value[0] );

}
static INLINE void mpma_lfield_range_PSD_FIFO_UDR_E_set( mpma_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_lfield_range_PSD_FIFO_UDR_E_set( mpma_buffer_t *b_ptr,
                                                         mpma_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_UDR_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_UDR_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_UDR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000064c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0 */
    if (start_bit <= 31) {
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
        /* (0x0000064c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0 */
        mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_OFF + subfield_offset),
                                                      CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000650 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000650 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1 */
        mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_OFF + subfield_offset),
                                                      CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000654 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000654 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2 */
        mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_OFF + subfield_offset),
                                                      CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_lfield_range_PSD_FIFO_UDR_E_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_lfield_range_PSD_FIFO_UDR_E_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_UDR_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_UDR_E_get", stop_bit, 95 );
    if (start_bit <= 31) {
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
    /* (0x0000064c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0 */
        reg_value = mpma_reg_PSD_FIFO_UDR_INT_EN_REG_0_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_MSK)
                      >> CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_MSK, CORE_REG_PSD_FIFO_UDR_INT_EN_REG_0_BIT_PSD_FIFO_UDR_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000650 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1 */
        reg_value = mpma_reg_PSD_FIFO_UDR_INT_EN_REG_1_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_MSK)
                      >> CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_MSK, CORE_REG_PSD_FIFO_UDR_INT_EN_REG_1_BIT_PSD_FIFO_UDR_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000654 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_E of register PMC_CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2 */
        reg_value = mpma_reg_PSD_FIFO_UDR_INT_EN_REG_2_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_MSK)
                      >> CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_MSK, CORE_REG_PSD_FIFO_UDR_INT_EN_REG_2_BIT_PSD_FIFO_UDR_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_UDR_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void mpma_lfield_PSD_FIFO_OVR_I_set_to_clear( mpma_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpma_lfield_PSD_FIFO_OVR_I_set_to_clear( mpma_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_OVR_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x00000600 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 */
    mpma_reg_PSD_FIFO_OVR_INT_REG_0_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_MSK,
                                                               CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF,
                                                               value[0]);

    /* (0x00000604 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1 */
    mpma_reg_PSD_FIFO_OVR_INT_REG_1_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_MSK,
                                                               CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF,
                                                               value[1]);

    /* (0x00000608 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2 */
    mpma_reg_PSD_FIFO_OVR_INT_REG_2_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_MSK,
                                                               CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF,
                                                               value[2]);
}

static INLINE void mpma_lfield_PSD_FIFO_OVR_I_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpma_lfield_PSD_FIFO_OVR_I_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00000600 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 */
    reg_value = mpma_reg_PSD_FIFO_OVR_INT_REG_0_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_MSK) >> CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF;
    value[0] |= field_value;

    /* (0x00000604 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1 */
    reg_value = mpma_reg_PSD_FIFO_OVR_INT_REG_1_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_MSK) >> CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF;
    value[1] |= field_value;

    /* (0x00000608 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2 */
    reg_value = mpma_reg_PSD_FIFO_OVR_INT_REG_2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_MSK) >> CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_OVR_I_get", value[2] , value[1] , value[0] );

}
static INLINE void mpma_lfield_range_PSD_FIFO_OVR_I_set_to_clear( mpma_buffer_t *b_ptr,
                                                                  mpma_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_lfield_range_PSD_FIFO_OVR_I_set_to_clear( mpma_buffer_t *b_ptr,
                                                                  mpma_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_OVR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_OVR_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_OVR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000600 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 */
    if (start_bit <= 31) {
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
        /* (0x00000600 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 */
        mpma_reg_PSD_FIFO_OVR_INT_REG_0_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                                   CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000604 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000604 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1 */
        mpma_reg_PSD_FIFO_OVR_INT_REG_1_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                                   CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000608 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000608 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2 */
        mpma_reg_PSD_FIFO_OVR_INT_REG_2_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                                   CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_lfield_range_PSD_FIFO_OVR_I_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_lfield_range_PSD_FIFO_OVR_I_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_OVR_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_OVR_I_get", stop_bit, 95 );
    if (start_bit <= 31) {
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
    /* (0x00000600 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 */
        reg_value = mpma_reg_PSD_FIFO_OVR_INT_REG_0_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_MSK)
                      >> CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_MSK, CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000604 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1 */
        reg_value = mpma_reg_PSD_FIFO_OVR_INT_REG_1_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_MSK)
                      >> CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_MSK, CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000608 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2 */
        reg_value = mpma_reg_PSD_FIFO_OVR_INT_REG_2_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_MSK)
                      >> CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_MSK, CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_OVR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpma_lfield_range_PSD_FIFO_OVR_I_poll( mpma_buffer_t *b_ptr,
                                                                          mpma_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_lfield_range_PSD_FIFO_OVR_I_poll( mpma_buffer_t *b_ptr,
                                                                          mpma_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_OVR_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_OVR_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_OVR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000600 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 */
    if (start_bit <= 31) {
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
        /* (0x00000600 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 */
        return mpma_reg_PSD_FIFO_OVR_INT_REG_0_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                     value << (CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000604 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000604 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1 */
        return mpma_reg_PSD_FIFO_OVR_INT_REG_1_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                     value << (CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000608 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000608 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2 */
        return mpma_reg_PSD_FIFO_OVR_INT_REG_2_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                     value << (CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpma_lfield_PSD_FIFO_OVR_I_poll( mpma_buffer_t *b_ptr,
                                                                    mpma_handle_t *h_ptr,
                                                                    UINT32 value[3],
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_lfield_PSD_FIFO_OVR_I_poll( mpma_buffer_t *b_ptr,
                                                                    mpma_handle_t *h_ptr,
                                                                    UINT32 value[3],
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_OVR_I_poll", value[2] , value[1] , value[0] );

    /* (0x00000600 bits 31:0) bits 0:31 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_0 */
    return mpma_reg_PSD_FIFO_OVR_INT_REG_0_poll( b_ptr,
                                                 h_ptr,
                                                 CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_MSK,
                                                 (value[0]<<CORE_REG_PSD_FIFO_OVR_INT_REG_0_BIT_PSD_FIFO_OVR_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);

    /* (0x00000604 bits 31:0) bits 32:63 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_1 */
    return mpma_reg_PSD_FIFO_OVR_INT_REG_1_poll( b_ptr,
                                                 h_ptr,
                                                 CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_MSK,
                                                 (value[1]<<CORE_REG_PSD_FIFO_OVR_INT_REG_1_BIT_PSD_FIFO_OVR_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);

    /* (0x00000608 bits 31:0) bits 64:95 use field PSD_FIFO_OVR_I of register PMC_CORE_REG_PSD_FIFO_OVR_INT_REG_2 */
    return mpma_reg_PSD_FIFO_OVR_INT_REG_2_poll( b_ptr,
                                                 h_ptr,
                                                 CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_MSK,
                                                 (value[2]<<CORE_REG_PSD_FIFO_OVR_INT_REG_2_BIT_PSD_FIFO_OVR_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void mpma_lfield_PSD_FIFO_UDR_I_set_to_clear( mpma_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpma_lfield_PSD_FIFO_UDR_I_set_to_clear( mpma_buffer_t *b_ptr,
                                                            mpma_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_UDR_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x0000060c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 */
    mpma_reg_PSD_FIFO_UDR_INT_REG_0_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_MSK,
                                                               CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF,
                                                               value[0]);

    /* (0x00000610 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1 */
    mpma_reg_PSD_FIFO_UDR_INT_REG_1_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_MSK,
                                                               CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF,
                                                               value[1]);

    /* (0x00000614 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2 */
    mpma_reg_PSD_FIFO_UDR_INT_REG_2_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_MSK,
                                                               CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF,
                                                               value[2]);
}

static INLINE void mpma_lfield_PSD_FIFO_UDR_I_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpma_lfield_PSD_FIFO_UDR_I_get( mpma_buffer_t *b_ptr,
                                                   mpma_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000060c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 */
    reg_value = mpma_reg_PSD_FIFO_UDR_INT_REG_0_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_MSK) >> CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF;
    value[0] |= field_value;

    /* (0x00000610 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1 */
    reg_value = mpma_reg_PSD_FIFO_UDR_INT_REG_1_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_MSK) >> CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF;
    value[1] |= field_value;

    /* (0x00000614 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2 */
    reg_value = mpma_reg_PSD_FIFO_UDR_INT_REG_2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_MSK) >> CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_UDR_I_get", value[2] , value[1] , value[0] );

}
static INLINE void mpma_lfield_range_PSD_FIFO_UDR_I_set_to_clear( mpma_buffer_t *b_ptr,
                                                                  mpma_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpma_lfield_range_PSD_FIFO_UDR_I_set_to_clear( mpma_buffer_t *b_ptr,
                                                                  mpma_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_UDR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_UDR_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_UDR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000060c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 */
    if (start_bit <= 31) {
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
        /* (0x0000060c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 */
        mpma_reg_PSD_FIFO_UDR_INT_REG_0_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                                   CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000610 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000610 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1 */
        mpma_reg_PSD_FIFO_UDR_INT_REG_1_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                                   CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000614 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000614 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2 */
        mpma_reg_PSD_FIFO_UDR_INT_REG_2_action_on_write_field_set( b_ptr,
                                                                   h_ptr,
                                                                   subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                                   CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mpma_lfield_range_PSD_FIFO_UDR_I_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpma_lfield_range_PSD_FIFO_UDR_I_get( mpma_buffer_t *b_ptr,
                                                           mpma_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_UDR_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_UDR_I_get", stop_bit, 95 );
    if (start_bit <= 31) {
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
    /* (0x0000060c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 */
        reg_value = mpma_reg_PSD_FIFO_UDR_INT_REG_0_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_MSK)
                      >> CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_MSK, CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 32 - start_bit;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000610 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1 */
        reg_value = mpma_reg_PSD_FIFO_UDR_INT_REG_1_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_MSK)
                      >> CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_MSK, CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 64 - start_bit;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000614 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2 */
        reg_value = mpma_reg_PSD_FIFO_UDR_INT_REG_2_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_MSK)
                      >> CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_MSK, CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_UDR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpma_lfield_range_PSD_FIFO_UDR_I_poll( mpma_buffer_t *b_ptr,
                                                                          mpma_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_lfield_range_PSD_FIFO_UDR_I_poll( mpma_buffer_t *b_ptr,
                                                                          mpma_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpma_lfield_range_PSD_FIFO_UDR_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpma_lfield_range_PSD_FIFO_UDR_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpma_lfield_range_PSD_FIFO_UDR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000060c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 */
    if (start_bit <= 31) {
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
        /* (0x0000060c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 */
        return mpma_reg_PSD_FIFO_UDR_INT_REG_0_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                     value << (CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000610 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1 */
    if ((start_bit <= 63) && (stop_bit >= 32)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 32) {
            subfield_offset = start_bit - 32;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 32;
        }
        if (stop_bit < 63) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 63;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000610 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1 */
        return mpma_reg_PSD_FIFO_UDR_INT_REG_1_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                     value << (CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000614 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2 */
    if ((start_bit <= 95) && (stop_bit >= 64)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 64) {
            subfield_offset = start_bit - 64;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 64;
        }
        if (stop_bit < 95) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 95;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000614 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2 */
        return mpma_reg_PSD_FIFO_UDR_INT_REG_2_poll( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                     value << (CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF + subfield_offset),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpma_lfield_PSD_FIFO_UDR_I_poll( mpma_buffer_t *b_ptr,
                                                                    mpma_handle_t *h_ptr,
                                                                    UINT32 value[3],
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpma_lfield_PSD_FIFO_UDR_I_poll( mpma_buffer_t *b_ptr,
                                                                    mpma_handle_t *h_ptr,
                                                                    UINT32 value[3],
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpma_lfield_PSD_FIFO_UDR_I_poll", value[2] , value[1] , value[0] );

    /* (0x0000060c bits 31:0) bits 0:31 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_0 */
    return mpma_reg_PSD_FIFO_UDR_INT_REG_0_poll( b_ptr,
                                                 h_ptr,
                                                 CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_MSK,
                                                 (value[0]<<CORE_REG_PSD_FIFO_UDR_INT_REG_0_BIT_PSD_FIFO_UDR_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);

    /* (0x00000610 bits 31:0) bits 32:63 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_1 */
    return mpma_reg_PSD_FIFO_UDR_INT_REG_1_poll( b_ptr,
                                                 h_ptr,
                                                 CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_MSK,
                                                 (value[1]<<CORE_REG_PSD_FIFO_UDR_INT_REG_1_BIT_PSD_FIFO_UDR_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);

    /* (0x00000614 bits 31:0) bits 64:95 use field PSD_FIFO_UDR_I of register PMC_CORE_REG_PSD_FIFO_UDR_INT_REG_2 */
    return mpma_reg_PSD_FIFO_UDR_INT_REG_2_poll( b_ptr,
                                                 h_ptr,
                                                 CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_MSK,
                                                 (value[2]<<CORE_REG_PSD_FIFO_UDR_INT_REG_2_BIT_PSD_FIFO_UDR_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MPMA_IO_INLINE_H */
