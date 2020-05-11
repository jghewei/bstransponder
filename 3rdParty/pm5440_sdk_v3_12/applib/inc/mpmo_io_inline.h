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
 *     and register accessor functions for the mpmo block
 *****************************************************************************/
#ifndef _MPMO_IO_INLINE_H
#define _MPMO_IO_INLINE_H

#include "mpmo_loc.h"
#include "mpmo_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MPMO_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mpmo
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
} mpmo_buffer_t;
static INLINE void mpmo_buffer_init( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mpmo_buffer_init( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mpmo_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mpmo_buffer_flush( mpmo_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mpmo_buffer_flush( mpmo_buffer_t *b_ptr )
{
    IOLOG( "mpmo_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mpmo_reg_read( mpmo_buffer_t *b_ptr,
                                    mpmo_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_read( mpmo_buffer_t *b_ptr,
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
static INLINE void mpmo_reg_write( mpmo_buffer_t *b_ptr,
                                   mpmo_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_write( mpmo_buffer_t *b_ptr,
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

static INLINE void mpmo_field_set( mpmo_buffer_t *b_ptr,
                                   mpmo_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_set( mpmo_buffer_t *b_ptr,
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

static INLINE void mpmo_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_action_on_write_field_set( mpmo_buffer_t *b_ptr,
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

static INLINE void mpmo_burst_read( mpmo_buffer_t *b_ptr,
                                    mpmo_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void mpmo_burst_read( mpmo_buffer_t *b_ptr,
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

static INLINE void mpmo_burst_write( mpmo_buffer_t *b_ptr,
                                     mpmo_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void mpmo_burst_write( mpmo_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mpmo_poll( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_poll( mpmo_buffer_t *b_ptr,
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
 *  register access functions for mpmo
 * ==================================================================================
 */

static INLINE void mpmo_reg_GEN_CFG_write( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_GEN_CFG_write( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_GEN_CFG_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_GEN_CFG,
                    value);
}

static INLINE void mpmo_reg_GEN_CFG_field_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_GEN_CFG_field_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_GEN_CFG_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_GEN_CFG,
                    mask,
                    PMC_CORE_REG_GEN_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_GEN_CFG_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_GEN_CFG_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_GEN_CFG);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_GEN_CFG_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_CSI_CFG_write( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CFG_write( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CSI_CFG_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CSI_CFG,
                    value);
}

static INLINE void mpmo_reg_CSI_CFG_field_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CFG_field_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CSI_CFG_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CSI_CFG,
                    mask,
                    PMC_CORE_REG_CSI_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_CSI_CFG_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CSI_CFG_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_CSI_CFG);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_CSI_CFG_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_AGE_FILT_CTRL_write( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_AGE_FILT_CTRL_write( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_AGE_FILT_CTRL_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_AGE_FILT_CTRL,
                    value);
}

static INLINE void mpmo_reg_AGE_FILT_CTRL_field_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_AGE_FILT_CTRL_field_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_AGE_FILT_CTRL_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_AGE_FILT_CTRL,
                    mask,
                    PMC_CORE_REG_AGE_FILT_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_AGE_FILT_CTRL_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_AGE_FILT_CTRL_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_AGE_FILT_CTRL);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_AGE_FILT_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_DEBUG_CTRL_write( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_DEBUG_CTRL_write( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_DEBUG_CTRL_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_DEBUG_CTRL,
                    value);
}

static INLINE void mpmo_reg_DEBUG_CTRL_field_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_DEBUG_CTRL_field_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_DEBUG_CTRL_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_DEBUG_CTRL,
                    mask,
                    PMC_CORE_REG_DEBUG_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_DEBUG_CTRL_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_DEBUG_CTRL_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_DEBUG_CTRL);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_DEBUG_CTRL_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_DEBUG_CTRL_poll( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_DEBUG_CTRL_poll( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_DEBUG_CTRL_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_CORE_REG_DEBUG_CTRL,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_DATA_AVAIL_CTRL_write( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_DATA_AVAIL_CTRL_write( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_DATA_AVAIL_CTRL_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_DATA_AVAIL_CTRL,
                    value);
}

static INLINE void mpmo_reg_DATA_AVAIL_CTRL_field_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_DATA_AVAIL_CTRL_field_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_DATA_AVAIL_CTRL_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_DATA_AVAIL_CTRL,
                    mask,
                    PMC_CORE_REG_DATA_AVAIL_CTRL_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_DATA_AVAIL_CTRL_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_DATA_AVAIL_CTRL_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_DATA_AVAIL_CTRL);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_DATA_AVAIL_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_CKCTL_CTRL_REG_array_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CKCTL_CTRL_REG_array_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CKCTL_CTRL_REG_array_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CKCTL_CTRL_REG(N),
                    value);
}

static INLINE void mpmo_reg_CKCTL_CTRL_REG_array_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CKCTL_CTRL_REG_array_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CKCTL_CTRL_REG_array_field_set", N, mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_CKCTL_CTRL_REG(N),
                    mask,
                    PMC_CORE_REG_CKCTL_CTRL_REG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_CKCTL_CTRL_REG_array_read( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CKCTL_CTRL_REG_array_read( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_CKCTL_CTRL_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_reg_CKCTL_CTRL_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_reg_MPMO_DP_CH_CFG_0_array_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_MPMO_DP_CH_CFG_0_array_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_MPMO_DP_CH_CFG_0_array_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_MPMO_DP_CH_CFG_0(N),
                    value);
}

static INLINE void mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set", N, mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_MPMO_DP_CH_CFG_0(N),
                    mask,
                    PMC_CORE_REG_MPMO_DP_CH_CFG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_MPMO_DP_CH_CFG_0_array_read( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_MPMO_DP_CH_CFG_0_array_read( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_MPMO_DP_CH_CFG_0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_reg_MPMO_DP_CH_CFG_0_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_reg_MPMO_DP_CH_CFG_1_array_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_MPMO_DP_CH_CFG_1_array_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_MPMO_DP_CH_CFG_1_array_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_MPMO_DP_CH_CFG_1(N),
                    value);
}

static INLINE void mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set", N, mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_CORE_REG_MPMO_DP_CH_CFG_1(N),
                    mask,
                    PMC_CORE_REG_MPMO_DP_CH_CFG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_MPMO_DP_CH_CFG_1_array_read( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_MPMO_DP_CH_CFG_1_array_read( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_CORE_REG_MPMO_DP_CH_CFG_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_reg_MPMO_DP_CH_CFG_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CSI_CHG_INT_EN_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_0,
                    value);
}

static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CSI_CHG_INT_EN_REG_0_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_CSI_CHG_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CSI_CHG_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CSI_CHG_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_CSI_CHG_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CSI_CHG_INT_EN_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_1,
                    value);
}

static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CSI_CHG_INT_EN_REG_1_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_CSI_CHG_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CSI_CHG_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CSI_CHG_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_CSI_CHG_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CSI_CHG_INT_EN_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_2,
                    value);
}

static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CSI_CHG_INT_EN_REG_2_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_CSI_CHG_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_CSI_CHG_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CSI_CHG_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CSI_CHG_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_CSI_CHG_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_GEN_INT_EN_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_0,
                    value);
}

static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_GEN_INT_EN_REG_0_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SSF_GEN_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_GEN_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_GEN_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_GEN_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_GEN_INT_EN_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_1,
                    value);
}

static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_GEN_INT_EN_REG_1_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SSF_GEN_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_GEN_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_GEN_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_GEN_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_GEN_INT_EN_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_2,
                    value);
}

static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_GEN_INT_EN_REG_2_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_SSF_GEN_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SSF_GEN_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_GEN_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_GEN_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_GEN_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_0,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_0_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_1,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_1_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_2,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_2_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_3_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_3_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_3_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_3,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_3_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_3_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_3_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_3,
                    mask,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_3_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_3_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_3_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_EN_REG_3);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_EN_REG_3_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_4_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_4_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_4_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_4,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_4_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_4_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_4_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_4,
                    mask,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_4_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_4_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_4_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_EN_REG_4);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_EN_REG_4_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_5_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_5_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_5_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_5,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_5_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_EN_REG_5_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_EN_REG_5_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_5,
                    mask,
                    PMC_CORE_REG_PKT_DROP_INT_EN_REG_5_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_5_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_EN_REG_5_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_EN_REG_5);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_EN_REG_5_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_EOP_ERR_INT_EN_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_0,
                    value);
}

static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_EOP_ERR_INT_EN_REG_0_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_EOP_ERR_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_EOP_ERR_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_EOP_ERR_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_EOP_ERR_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_EOP_ERR_INT_EN_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_1,
                    value);
}

static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_EOP_ERR_INT_EN_REG_1_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_EOP_ERR_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_EOP_ERR_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_EOP_ERR_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_EOP_ERR_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_EOP_ERR_INT_EN_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_2,
                    value);
}

static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_EOP_ERR_INT_EN_REG_2_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_EOP_ERR_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_EOP_ERR_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_EOP_ERR_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_EOP_ERR_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_EOP_ERR_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SOP_ERR_INT_EN_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_0,
                    value);
}

static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SOP_ERR_INT_EN_REG_0_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SOP_ERR_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SOP_ERR_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SOP_ERR_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SOP_ERR_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SOP_ERR_INT_EN_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_1,
                    value);
}

static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SOP_ERR_INT_EN_REG_1_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SOP_ERR_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SOP_ERR_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SOP_ERR_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SOP_ERR_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SOP_ERR_INT_EN_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_2,
                    value);
}

static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SOP_ERR_INT_EN_REG_2_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_SOP_ERR_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SOP_ERR_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SOP_ERR_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SOP_ERR_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SOP_ERR_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_BWR_RAMP_INT_EN_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0,
                    value);
}

static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_BWR_RAMP_INT_EN_REG_0_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_BWR_RAMP_INT_EN_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1,
                    value);
}

static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_BWR_RAMP_INT_EN_REG_1_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_BWR_RAMP_INT_EN_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2,
                    value);
}

static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_BWR_RAMP_INT_EN_REG_2_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_0_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_STATE_INT_EN_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_0,
                    value);
}

static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_0_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_STATE_INT_EN_REG_0_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_0,
                    mask,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_0_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SSF_STATE_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_EN_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_EN_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_EN_REG_0_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_1_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_STATE_INT_EN_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_1,
                    value);
}

static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_1_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_STATE_INT_EN_REG_1_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_1,
                    mask,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SSF_STATE_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_EN_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_EN_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_EN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_2_write( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_STATE_INT_EN_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_2,
                    value);
}

static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_EN_REG_2_field_set( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_STATE_INT_EN_REG_2_field_set", mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_2,
                    mask,
                    PMC_CORE_REG_SSF_STATE_INT_EN_REG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_SSF_STATE_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_EN_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_EN_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_EN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void mpmo_reg_CSI_CHG_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CSI_CHG_INT_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_REG_0,
                    value);
}

static INLINE void mpmo_reg_CSI_CHG_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CSI_CHG_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_CSI_CHG_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_CSI_CHG_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CSI_CHG_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CSI_CHG_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_CSI_CHG_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_CSI_CHG_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_CSI_CHG_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_CSI_CHG_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CSI_CHG_INT_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_REG_1,
                    value);
}

static INLINE void mpmo_reg_CSI_CHG_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CSI_CHG_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_CSI_CHG_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_CSI_CHG_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CSI_CHG_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CSI_CHG_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_CSI_CHG_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_CSI_CHG_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_CSI_CHG_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_CSI_CHG_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CSI_CHG_INT_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CSI_CHG_INT_REG_2,
                    value);
}

static INLINE void mpmo_reg_CSI_CHG_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CSI_CHG_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CSI_CHG_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_CSI_CHG_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_CSI_CHG_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CSI_CHG_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CSI_CHG_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_CSI_CHG_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_CSI_CHG_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_CSI_CHG_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SSF_GEN_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_GEN_INT_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_REG_0,
                    value);
}

static INLINE void mpmo_reg_SSF_GEN_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_GEN_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SSF_GEN_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SSF_GEN_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_GEN_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_GEN_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_GEN_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_GEN_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_GEN_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SSF_GEN_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_GEN_INT_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_REG_1,
                    value);
}

static INLINE void mpmo_reg_SSF_GEN_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_GEN_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SSF_GEN_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SSF_GEN_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_GEN_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_GEN_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_GEN_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_GEN_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_GEN_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SSF_GEN_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_GEN_INT_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_GEN_INT_REG_2,
                    value);
}

static INLINE void mpmo_reg_SSF_GEN_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_GEN_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_GEN_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SSF_GEN_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SSF_GEN_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_GEN_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_GEN_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_GEN_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_GEN_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_GEN_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_PKT_DROP_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_REG_0,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PKT_DROP_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_PKT_DROP_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PKT_DROP_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_PKT_DROP_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_REG_1,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PKT_DROP_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_PKT_DROP_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PKT_DROP_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_PKT_DROP_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_REG_2,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PKT_DROP_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_PKT_DROP_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PKT_DROP_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_PKT_DROP_INT_REG_3_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_3_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_REG_3_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_REG_3,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_REG_3_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_3_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_REG_3_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PKT_DROP_INT_REG_3,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_3_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_3_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_REG_3);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_REG_3_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_3_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_3_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_PKT_DROP_INT_REG_3_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PKT_DROP_INT_REG_3,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_PKT_DROP_INT_REG_4_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_4_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_REG_4_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_REG_4,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_REG_4_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_4_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_REG_4_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PKT_DROP_INT_REG_4,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_4_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_4_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_REG_4);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_REG_4_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_4_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_4_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_PKT_DROP_INT_REG_4_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PKT_DROP_INT_REG_4,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_PKT_DROP_INT_REG_5_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_5_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_PKT_DROP_INT_REG_5_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_PKT_DROP_INT_REG_5,
                    value);
}

static INLINE void mpmo_reg_PKT_DROP_INT_REG_5_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_PKT_DROP_INT_REG_5_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_PKT_DROP_INT_REG_5_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_PKT_DROP_INT_REG_5,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_5_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_PKT_DROP_INT_REG_5_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_PKT_DROP_INT_REG_5);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_PKT_DROP_INT_REG_5_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_5_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_5_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_PKT_DROP_INT_REG_5_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_PKT_DROP_INT_REG_5,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_EOP_ERR_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_EOP_ERR_INT_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_REG_0,
                    value);
}

static INLINE void mpmo_reg_EOP_ERR_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_EOP_ERR_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_EOP_ERR_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_EOP_ERR_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_EOP_ERR_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_EOP_ERR_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_EOP_ERR_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_EOP_ERR_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_EOP_ERR_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_EOP_ERR_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_EOP_ERR_INT_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_REG_1,
                    value);
}

static INLINE void mpmo_reg_EOP_ERR_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_EOP_ERR_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_EOP_ERR_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_EOP_ERR_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_EOP_ERR_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_EOP_ERR_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_EOP_ERR_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_EOP_ERR_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_EOP_ERR_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_EOP_ERR_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_EOP_ERR_INT_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_EOP_ERR_INT_REG_2,
                    value);
}

static INLINE void mpmo_reg_EOP_ERR_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_EOP_ERR_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_EOP_ERR_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_EOP_ERR_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_EOP_ERR_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_EOP_ERR_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_EOP_ERR_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_EOP_ERR_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_EOP_ERR_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_EOP_ERR_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SOP_ERR_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SOP_ERR_INT_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_REG_0,
                    value);
}

static INLINE void mpmo_reg_SOP_ERR_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SOP_ERR_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SOP_ERR_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SOP_ERR_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SOP_ERR_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SOP_ERR_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SOP_ERR_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SOP_ERR_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SOP_ERR_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SOP_ERR_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SOP_ERR_INT_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_REG_1,
                    value);
}

static INLINE void mpmo_reg_SOP_ERR_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SOP_ERR_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SOP_ERR_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SOP_ERR_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SOP_ERR_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SOP_ERR_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SOP_ERR_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SOP_ERR_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SOP_ERR_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SOP_ERR_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SOP_ERR_INT_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SOP_ERR_INT_REG_2,
                    value);
}

static INLINE void mpmo_reg_SOP_ERR_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SOP_ERR_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SOP_ERR_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SOP_ERR_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SOP_ERR_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SOP_ERR_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SOP_ERR_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SOP_ERR_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                    mpmo_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SOP_ERR_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SOP_ERR_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_BWR_RAMP_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_BWR_RAMP_INT_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_REG_0,
                    value);
}

static INLINE void mpmo_reg_BWR_RAMP_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_BWR_RAMP_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_BWR_RAMP_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_BWR_RAMP_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_BWR_RAMP_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_BWR_RAMP_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_BWR_RAMP_INT_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_REG_1,
                    value);
}

static INLINE void mpmo_reg_BWR_RAMP_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_BWR_RAMP_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_BWR_RAMP_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_BWR_RAMP_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_BWR_RAMP_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_BWR_RAMP_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_BWR_RAMP_INT_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_BWR_RAMP_INT_REG_2,
                    value);
}

static INLINE void mpmo_reg_BWR_RAMP_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_BWR_RAMP_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                          mpmo_handle_t *h_ptr,
                                                                          UINT32 mask,
                                                                          UINT32 ofs,
                                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_BWR_RAMP_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_BWR_RAMP_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_BWR_RAMP_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_BWR_RAMP_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SSF_STATE_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_REG_0_write( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_STATE_INT_REG_0_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_REG_0,
                    value);
}

static INLINE void mpmo_reg_SSF_STATE_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                           mpmo_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_REG_0_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                           mpmo_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_STATE_INT_REG_0_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SSF_STATE_INT_REG_0,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SSF_STATE_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_STATE_INT_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_STATE_INT_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SSF_STATE_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_REG_1_write( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_STATE_INT_REG_1_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_REG_1,
                    value);
}

static INLINE void mpmo_reg_SSF_STATE_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                           mpmo_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_REG_1_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                           mpmo_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_STATE_INT_REG_1_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SSF_STATE_INT_REG_1,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SSF_STATE_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_STATE_INT_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_STATE_INT_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_SSF_STATE_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_REG_2_write( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_SSF_STATE_INT_REG_2_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_SSF_STATE_INT_REG_2,
                    value);
}

static INLINE void mpmo_reg_SSF_STATE_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                           mpmo_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_SSF_STATE_INT_REG_2_action_on_write_field_set( mpmo_buffer_t *b_ptr,
                                                                           mpmo_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_SSF_STATE_INT_REG_2_action_on_write_field_set", mask, ofs, value );
    mpmo_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_CORE_REG_SSF_STATE_INT_REG_2,
                                    mask,
                                    ofs,
                                    value);

}

static INLINE UINT32 mpmo_reg_SSF_STATE_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_STATE_INT_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_STATE_INT_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_V_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_V_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_V_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_V_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_BWR_RAMP_INT_V_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_BWR_RAMP_INT_V_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_V_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_V_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_V_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_V_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_BWR_RAMP_INT_V_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_BWR_RAMP_INT_V_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_V_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_BWR_RAMP_INT_V_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_BWR_RAMP_INT_V_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_BWR_RAMP_INT_V_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_BWR_RAMP_INT_V_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_BWR_RAMP_INT_V_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 mpmo_reg_SSF_STATE_INT_V_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_V_REG_0_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_V_REG_0);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_V_REG_0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                        mpmo_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_0_poll( mpmo_buffer_t *b_ptr,
                                                                        mpmo_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_STATE_INT_V_REG_0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_STATE_INT_V_REG_0,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 mpmo_reg_SSF_STATE_INT_V_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_V_REG_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_V_REG_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_V_REG_1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                        mpmo_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_1_poll( mpmo_buffer_t *b_ptr,
                                                                        mpmo_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_STATE_INT_V_REG_1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_STATE_INT_V_REG_1,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE UINT32 mpmo_reg_SSF_STATE_INT_V_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_SSF_STATE_INT_V_REG_2_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_SSF_STATE_INT_V_REG_2);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_SSF_STATE_INT_V_REG_2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                        mpmo_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_2_poll( mpmo_buffer_t *b_ptr,
                                                                        mpmo_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "mpmo_reg_SSF_STATE_INT_V_REG_2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return mpmo_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_CORE_REG_SSF_STATE_INT_V_REG_2,
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void mpmo_reg_CFG_RAM_1_array_write( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CFG_RAM_1_array_write( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CFG_RAM_1_array_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CFG_RAM_1(N),
                    value);
}

static INLINE void mpmo_reg_CFG_RAM_1_array_field_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CFG_RAM_1_array_field_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CFG_RAM_1_array_field_set", N, mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CFG_RAM_1(N),
                    mask,
                    PMC_CORE_REG_CFG_RAM_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_CFG_RAM_1_array_read( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CFG_RAM_1_array_read( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CFG_RAM_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_reg_CFG_RAM_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_reg_CFG_RAM_2_array_write( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CFG_RAM_2_array_write( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CFG_RAM_2_array_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CFG_RAM_2(N),
                    value);
}

static INLINE void mpmo_reg_CFG_RAM_2_array_field_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CFG_RAM_2_array_field_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CFG_RAM_2_array_field_set", N, mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CFG_RAM_2(N),
                    mask,
                    PMC_CORE_REG_CFG_RAM_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_CFG_RAM_2_array_read( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CFG_RAM_2_array_read( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CFG_RAM_2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_reg_CFG_RAM_2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void mpmo_reg_CFG_RAM_3_array_write( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CFG_RAM_3_array_write( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mpmo_reg_CFG_RAM_3_array_write", value );
    mpmo_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CFG_RAM_3(N),
                    value);
}

static INLINE void mpmo_reg_CFG_RAM_3_array_field_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_reg_CFG_RAM_3_array_field_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "mpmo_reg_CFG_RAM_3_array_field_set", N, mask, ofs, value );
    mpmo_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_CORE_REG_CFG_RAM_3(N),
                    mask,
                    PMC_CORE_REG_CFG_RAM_3_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mpmo_reg_CFG_RAM_3_array_read( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_CFG_RAM_3_array_read( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_CFG_RAM_3(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mpmo_reg_CFG_RAM_3_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mpmo_reg_DEBUG_DATA_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_DEBUG_DATA_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_DEBUG_DATA);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_DEBUG_DATA_read", reg_value);
    return reg_value;
}

static INLINE UINT32 mpmo_reg_DEBUG_DATA_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_reg_DEBUG_DATA_1_read( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mpmo_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_CORE_REG_DEBUG_DATA_1);

    IOLOG( "%s -> 0x%08x;", "mpmo_reg_DEBUG_DATA_1_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mpmo_field_RAMP_EXTEND_EN_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_RAMP_EXTEND_EN_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_RAMP_EXTEND_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_RAMP_EXTEND_EN_set", value );

    /* (0x00000000 bits 30) field RAMP_EXTEND_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_RAMP_EXTEND_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_RAMP_EXTEND_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_RAMP_EXTEND_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_RAMP_EXTEND_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 30) field RAMP_EXTEND_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_RAMP_EXTEND_EN_MSK) >> CORE_REG_GEN_CFG_BIT_RAMP_EXTEND_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_RAMP_EXTEND_EN_get", value );

    return value;
}
static INLINE void mpmo_field_ZONE_SEL_set( mpmo_buffer_t *b_ptr,
                                            mpmo_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_ZONE_SEL_set( mpmo_buffer_t *b_ptr,
                                            mpmo_handle_t *h_ptr,
                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_ZONE_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_ZONE_SEL_set", value );

    /* (0x00000000 bits 28) field ZONE_SEL of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_ZONE_SEL_MSK,
                                CORE_REG_GEN_CFG_BIT_ZONE_SEL_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_ZONE_SEL_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_ZONE_SEL_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 28) field ZONE_SEL of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_ZONE_SEL_MSK) >> CORE_REG_GEN_CFG_BIT_ZONE_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_ZONE_SEL_get", value );

    return value;
}
static INLINE void mpmo_field_SEQ_CHK_EN_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_SEQ_CHK_EN_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_SEQ_CHK_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_SEQ_CHK_EN_set", value );

    /* (0x00000000 bits 24) field SEQ_CHK_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_SEQ_CHK_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_SEQ_CHK_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_SEQ_CHK_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_SEQ_CHK_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 24) field SEQ_CHK_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_SEQ_CHK_EN_MSK) >> CORE_REG_GEN_CFG_BIT_SEQ_CHK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_SEQ_CHK_EN_get", value );

    return value;
}
static INLINE void mpmo_field_PACER_MODE_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_PACER_MODE_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_PACER_MODE_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mpmo_field_PACER_MODE_set", value );

    /* (0x00000000 bits 21:20) field PACER_MODE of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_PACER_MODE_MSK,
                                CORE_REG_GEN_CFG_BIT_PACER_MODE_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_PACER_MODE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_PACER_MODE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 21:20) field PACER_MODE of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_PACER_MODE_MSK) >> CORE_REG_GEN_CFG_BIT_PACER_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_PACER_MODE_get", value );

    return value;
}
static INLINE void mpmo_field_range_PACER_MODE_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_PACER_MODE_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_PACER_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_PACER_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_PACER_MODE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 21:20) field PACER_MODE of register PMC_CORE_REG_GEN_CFG */
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
        /* (0x00000000 bits 21:20) field PACER_MODE of register PMC_CORE_REG_GEN_CFG */
        mpmo_reg_GEN_CFG_field_set( b_ptr,
                                    h_ptr,
                                    subfield_mask << (CORE_REG_GEN_CFG_BIT_PACER_MODE_OFF + subfield_offset),
                                    CORE_REG_GEN_CFG_BIT_PACER_MODE_OFF + subfield_offset,
                                    value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_PACER_MODE_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_PACER_MODE_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_PACER_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_PACER_MODE_get", stop_bit, 1 );
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
    /* (0x00000000 bits 21:20) field PACER_MODE of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_GEN_CFG_BIT_PACER_MODE_MSK)
                  >> CORE_REG_GEN_CFG_BIT_PACER_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_GEN_CFG_BIT_PACER_MODE_MSK, CORE_REG_GEN_CFG_BIT_PACER_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_PACER_MODE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_BWR_IND_MODE_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_BWR_IND_MODE_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_BWR_IND_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_BWR_IND_MODE_set", value );

    /* (0x00000000 bits 19) field BWR_IND_MODE of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_BWR_IND_MODE_MSK,
                                CORE_REG_GEN_CFG_BIT_BWR_IND_MODE_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_BWR_IND_MODE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_BWR_IND_MODE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 19) field BWR_IND_MODE of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_BWR_IND_MODE_MSK) >> CORE_REG_GEN_CFG_BIT_BWR_IND_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_BWR_IND_MODE_get", value );

    return value;
}
static INLINE void mpmo_field_RAMP_STOP_MODE_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_RAMP_STOP_MODE_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_RAMP_STOP_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_RAMP_STOP_MODE_set", value );

    /* (0x00000000 bits 18) field RAMP_STOP_MODE of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_RAMP_STOP_MODE_MSK,
                                CORE_REG_GEN_CFG_BIT_RAMP_STOP_MODE_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_RAMP_STOP_MODE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_RAMP_STOP_MODE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 18) field RAMP_STOP_MODE of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_RAMP_STOP_MODE_MSK) >> CORE_REG_GEN_CFG_BIT_RAMP_STOP_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_RAMP_STOP_MODE_get", value );

    return value;
}
static INLINE void mpmo_field_AGE_SQUELCH_EN_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_AGE_SQUELCH_EN_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_AGE_SQUELCH_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_AGE_SQUELCH_EN_set", value );

    /* (0x00000000 bits 17) field AGE_SQUELCH_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_AGE_SQUELCH_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_AGE_SQUELCH_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_AGE_SQUELCH_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_AGE_SQUELCH_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 17) field AGE_SQUELCH_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_AGE_SQUELCH_EN_MSK) >> CORE_REG_GEN_CFG_BIT_AGE_SQUELCH_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_AGE_SQUELCH_EN_get", value );

    return value;
}
static INLINE void mpmo_field_CN_SQUELCH_EN_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CN_SQUELCH_EN_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CN_SQUELCH_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_CN_SQUELCH_EN_set", value );

    /* (0x00000000 bits 16) field CN_SQUELCH_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_CN_SQUELCH_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_CN_SQUELCH_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_CN_SQUELCH_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CN_SQUELCH_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 16) field CN_SQUELCH_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_CN_SQUELCH_EN_MSK) >> CORE_REG_GEN_CFG_BIT_CN_SQUELCH_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_CN_SQUELCH_EN_get", value );

    return value;
}
static INLINE void mpmo_field_AGE_FILTER_EN_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_AGE_FILTER_EN_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_AGE_FILTER_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_AGE_FILTER_EN_set", value );

    /* (0x00000000 bits 14) field AGE_FILTER_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_AGE_FILTER_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_AGE_FILTER_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_AGE_FILTER_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_AGE_FILTER_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 14) field AGE_FILTER_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_AGE_FILTER_EN_MSK) >> CORE_REG_GEN_CFG_BIT_AGE_FILTER_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_AGE_FILTER_EN_get", value );

    return value;
}
static INLINE void mpmo_field_CN_BIT_MODE_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CN_BIT_MODE_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CN_BIT_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_CN_BIT_MODE_set", value );

    /* (0x00000000 bits 13) field CN_BIT_MODE of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_CN_BIT_MODE_MSK,
                                CORE_REG_GEN_CFG_BIT_CN_BIT_MODE_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_CN_BIT_MODE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CN_BIT_MODE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 13) field CN_BIT_MODE of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_CN_BIT_MODE_MSK) >> CORE_REG_GEN_CFG_BIT_CN_BIT_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_CN_BIT_MODE_get", value );

    return value;
}
static INLINE void mpmo_field_SSF_GEN_EN_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_SSF_GEN_EN_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_SSF_GEN_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_SSF_GEN_EN_set", value );

    /* (0x00000000 bits 12) field SSF_GEN_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_SSF_GEN_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_SSF_GEN_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_SSF_GEN_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_SSF_GEN_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 12) field SSF_GEN_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_SSF_GEN_EN_MSK) >> CORE_REG_GEN_CFG_BIT_SSF_GEN_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_SSF_GEN_EN_get", value );

    return value;
}
static INLINE void mpmo_field_SSF_THRES_EN_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_SSF_THRES_EN_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_SSF_THRES_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_SSF_THRES_EN_set", value );

    /* (0x00000000 bits 10) field SSF_THRES_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_SSF_THRES_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_SSF_THRES_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_SSF_THRES_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_SSF_THRES_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 10) field SSF_THRES_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_SSF_THRES_EN_MSK) >> CORE_REG_GEN_CFG_BIT_SSF_THRES_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_SSF_THRES_EN_get", value );

    return value;
}
static INLINE void mpmo_field_BUS_WIDTH_set( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_BUS_WIDTH_set( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_BUS_WIDTH_set", value, 3);
    IOLOG( "%s <= 0x%08x", "mpmo_field_BUS_WIDTH_set", value );

    /* (0x00000000 bits 9:8) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK,
                                CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_BUS_WIDTH_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_BUS_WIDTH_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 9:8) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK) >> CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_BUS_WIDTH_get", value );

    return value;
}
static INLINE void mpmo_field_range_BUS_WIDTH_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_BUS_WIDTH_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_BUS_WIDTH_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_BUS_WIDTH_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_BUS_WIDTH_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 9:8) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
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
        /* (0x00000000 bits 9:8) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
        mpmo_reg_GEN_CFG_field_set( b_ptr,
                                    h_ptr,
                                    subfield_mask << (CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF + subfield_offset),
                                    CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF + subfield_offset,
                                    value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_BUS_WIDTH_get( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_BUS_WIDTH_get( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_BUS_WIDTH_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_BUS_WIDTH_get", stop_bit, 1 );
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
    /* (0x00000000 bits 9:8) field BUS_WIDTH of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK)
                  >> CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_GEN_CFG_BIT_BUS_WIDTH_MSK, CORE_REG_GEN_CFG_BIT_BUS_WIDTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_BUS_WIDTH_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_DPI_DYNAMIC_LS_EN_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DPI_DYNAMIC_LS_EN_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DPI_DYNAMIC_LS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_DPI_DYNAMIC_LS_EN_set", value );

    /* (0x00000000 bits 5) field DPI_DYNAMIC_LS_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_DPI_DYNAMIC_LS_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DPI_DYNAMIC_LS_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 5) field DPI_DYNAMIC_LS_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_MSK) >> CORE_REG_GEN_CFG_BIT_DPI_DYNAMIC_LS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_DPI_DYNAMIC_LS_EN_get", value );

    return value;
}
static INLINE void mpmo_field_DCI_DYNAMIC_LS_EN_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DCI_DYNAMIC_LS_EN_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DCI_DYNAMIC_LS_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_DCI_DYNAMIC_LS_EN_set", value );

    /* (0x00000000 bits 4) field DCI_DYNAMIC_LS_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_DCI_DYNAMIC_LS_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DCI_DYNAMIC_LS_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field DCI_DYNAMIC_LS_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_MSK) >> CORE_REG_GEN_CFG_BIT_DCI_DYNAMIC_LS_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_DCI_DYNAMIC_LS_EN_get", value );

    return value;
}
static INLINE void mpmo_field_MPMO_EN_set( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_MPMO_EN_set( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_MPMO_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_MPMO_EN_set", value );

    /* (0x00000000 bits 0) field MPMO_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_MPMO_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_MPMO_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_MPMO_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_MPMO_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field MPMO_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_MPMO_EN_MSK) >> CORE_REG_GEN_CFG_BIT_MPMO_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_MPMO_EN_get", value );

    return value;
}
static INLINE void mpmo_field_CN_TARGET_DECIMAL_EN_set( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CN_TARGET_DECIMAL_EN_set( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CN_TARGET_DECIMAL_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_CN_TARGET_DECIMAL_EN_set", value );

    /* (0x00000000 bits 29) field CN_TARGET_DECIMAL_EN of register PMC_CORE_REG_GEN_CFG */
    mpmo_reg_GEN_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_GEN_CFG_BIT_CN_TARGET_DECIMAL_EN_MSK,
                                CORE_REG_GEN_CFG_BIT_CN_TARGET_DECIMAL_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_CN_TARGET_DECIMAL_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CN_TARGET_DECIMAL_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 29) field CN_TARGET_DECIMAL_EN of register PMC_CORE_REG_GEN_CFG */
    reg_value = mpmo_reg_GEN_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_GEN_CFG_BIT_CN_TARGET_DECIMAL_EN_MSK) >> CORE_REG_GEN_CFG_BIT_CN_TARGET_DECIMAL_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_CN_TARGET_DECIMAL_EN_get", value );

    return value;
}
static INLINE void mpmo_field_CSI_MASK_EN_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CSI_MASK_EN_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CSI_MASK_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_CSI_MASK_EN_set", value );

    /* (0x00000004 bits 31) field CSI_MASK_EN of register PMC_CORE_REG_CSI_CFG */
    mpmo_reg_CSI_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_CSI_CFG_BIT_CSI_MASK_EN_MSK,
                                CORE_REG_CSI_CFG_BIT_CSI_MASK_EN_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_CSI_MASK_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CSI_MASK_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 31) field CSI_MASK_EN of register PMC_CORE_REG_CSI_CFG */
    reg_value = mpmo_reg_CSI_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_CSI_CFG_BIT_CSI_MASK_EN_MSK) >> CORE_REG_CSI_CFG_BIT_CSI_MASK_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_CSI_MASK_EN_get", value );

    return value;
}
static INLINE void mpmo_field_CSI_NORMAL_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CSI_NORMAL_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CSI_NORMAL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mpmo_field_CSI_NORMAL_set", value );

    /* (0x00000004 bits 4:0) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG */
    mpmo_reg_CSI_CFG_field_set( b_ptr,
                                h_ptr,
                                CORE_REG_CSI_CFG_BIT_CSI_NORMAL_MSK,
                                CORE_REG_CSI_CFG_BIT_CSI_NORMAL_OFF,
                                value);
}

static INLINE UINT32 mpmo_field_CSI_NORMAL_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CSI_NORMAL_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 4:0) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG */
    reg_value = mpmo_reg_CSI_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_CSI_CFG_BIT_CSI_NORMAL_MSK) >> CORE_REG_CSI_CFG_BIT_CSI_NORMAL_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_CSI_NORMAL_get", value );

    return value;
}
static INLINE void mpmo_field_range_CSI_NORMAL_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_CSI_NORMAL_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CSI_NORMAL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CSI_NORMAL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CSI_NORMAL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000004 bits 4:0) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG */
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
        /* (0x00000004 bits 4:0) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG */
        mpmo_reg_CSI_CFG_field_set( b_ptr,
                                    h_ptr,
                                    subfield_mask << (CORE_REG_CSI_CFG_BIT_CSI_NORMAL_OFF + subfield_offset),
                                    CORE_REG_CSI_CFG_BIT_CSI_NORMAL_OFF + subfield_offset,
                                    value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_CSI_NORMAL_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_CSI_NORMAL_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CSI_NORMAL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CSI_NORMAL_get", stop_bit, 4 );
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
    /* (0x00000004 bits 4:0) field CSI_NORMAL of register PMC_CORE_REG_CSI_CFG */
    reg_value = mpmo_reg_CSI_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CFG_BIT_CSI_NORMAL_MSK)
                  >> CORE_REG_CSI_CFG_BIT_CSI_NORMAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CFG_BIT_CSI_NORMAL_MSK, CORE_REG_CSI_CFG_BIT_CSI_NORMAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CSI_NORMAL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_AGE_FILT_PERIOD_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_AGE_FILT_PERIOD_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_AGE_FILT_PERIOD_set", value, 16777215);
    IOLOG( "%s <= 0x%08x", "mpmo_field_AGE_FILT_PERIOD_set", value );

    /* (0x0000000c bits 23:0) field AGE_FILT_PERIOD of register PMC_CORE_REG_AGE_FILT_CTRL */
    mpmo_reg_AGE_FILT_CTRL_field_set( b_ptr,
                                      h_ptr,
                                      CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_MSK,
                                      CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_OFF,
                                      value);
}

static INLINE UINT32 mpmo_field_AGE_FILT_PERIOD_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_AGE_FILT_PERIOD_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 23:0) field AGE_FILT_PERIOD of register PMC_CORE_REG_AGE_FILT_CTRL */
    reg_value = mpmo_reg_AGE_FILT_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_MSK) >> CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_AGE_FILT_PERIOD_get", value );

    return value;
}
static INLINE void mpmo_field_range_AGE_FILT_PERIOD_set( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_AGE_FILT_PERIOD_set( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_FILT_PERIOD_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_FILT_PERIOD_set", stop_bit, 23 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_FILT_PERIOD_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 23:0) field AGE_FILT_PERIOD of register PMC_CORE_REG_AGE_FILT_CTRL */
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
        /* (0x0000000c bits 23:0) field AGE_FILT_PERIOD of register PMC_CORE_REG_AGE_FILT_CTRL */
        mpmo_reg_AGE_FILT_CTRL_field_set( b_ptr,
                                          h_ptr,
                                          subfield_mask << (CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_OFF + subfield_offset),
                                          CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_OFF + subfield_offset,
                                          value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_AGE_FILT_PERIOD_get( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_AGE_FILT_PERIOD_get( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_FILT_PERIOD_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_FILT_PERIOD_get", stop_bit, 23 );
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
    /* (0x0000000c bits 23:0) field AGE_FILT_PERIOD of register PMC_CORE_REG_AGE_FILT_CTRL */
    reg_value = mpmo_reg_AGE_FILT_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_MSK)
                  >> CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_MSK, CORE_REG_AGE_FILT_CTRL_BIT_AGE_FILT_PERIOD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_FILT_PERIOD_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_DEBUG_DATA_REQ_ID_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DEBUG_DATA_REQ_ID_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DEBUG_DATA_REQ_ID_set", value, 127);
    IOLOG( "%s <= 0x%08x", "mpmo_field_DEBUG_DATA_REQ_ID_set", value );

    /* (0x00000028 bits 16:22) field DEBUG_DATA_REQ_ID of register PMC_CORE_REG_DEBUG_CTRL */
    mpmo_reg_DEBUG_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_MSK,
                                   CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF,
                                   value);
}

static INLINE UINT32 mpmo_field_DEBUG_DATA_REQ_ID_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DEBUG_DATA_REQ_ID_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 16:22) field DEBUG_DATA_REQ_ID of register PMC_CORE_REG_DEBUG_CTRL */
    reg_value = mpmo_reg_DEBUG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_MSK) >> CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_DEBUG_DATA_REQ_ID_get", value );

    return value;
}
static INLINE void mpmo_field_range_DEBUG_DATA_REQ_ID_set( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_DEBUG_DATA_REQ_ID_set( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_DEBUG_DATA_REQ_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_DEBUG_DATA_REQ_ID_set", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_DEBUG_DATA_REQ_ID_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000028 bits 16:22) field DEBUG_DATA_REQ_ID of register PMC_CORE_REG_DEBUG_CTRL */
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
        /* (0x00000028 bits 16:22) field DEBUG_DATA_REQ_ID of register PMC_CORE_REG_DEBUG_CTRL */
        mpmo_reg_DEBUG_CTRL_field_set( b_ptr,
                                       h_ptr,
                                       subfield_mask << (CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF + subfield_offset),
                                       CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF + subfield_offset,
                                       value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_DEBUG_DATA_REQ_ID_get( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_DEBUG_DATA_REQ_ID_get( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_DEBUG_DATA_REQ_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_DEBUG_DATA_REQ_ID_get", stop_bit, 6 );
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
    /* (0x00000028 bits 16:22) field DEBUG_DATA_REQ_ID of register PMC_CORE_REG_DEBUG_CTRL */
    reg_value = mpmo_reg_DEBUG_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_MSK)
                  >> CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_MSK, CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_DEBUG_DATA_REQ_ID_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_field_range_DEBUG_DATA_REQ_ID_poll( mpmo_buffer_t *b_ptr,
                                                                            mpmo_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_field_range_DEBUG_DATA_REQ_ID_poll( mpmo_buffer_t *b_ptr,
                                                                            mpmo_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_DEBUG_DATA_REQ_ID_poll", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_DEBUG_DATA_REQ_ID_poll", stop_bit, 6 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_DEBUG_DATA_REQ_ID_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000028 bits 16:22) field DEBUG_DATA_REQ_ID of register PMC_CORE_REG_DEBUG_CTRL */
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
        if (stop_bit < 6) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 6;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000028 bits 16:22) field DEBUG_DATA_REQ_ID of register PMC_CORE_REG_DEBUG_CTRL */
        return mpmo_reg_DEBUG_CTRL_poll( b_ptr,
                                         h_ptr,
                                         subfield_mask << (CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF + subfield_offset),
                                         value << (CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF + subfield_offset),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_ID_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_ID_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mpmo_field_DEBUG_DATA_REQ_ID_poll", value );

    /* (0x00000028 bits 16:22) field DEBUG_DATA_REQ_ID of register PMC_CORE_REG_DEBUG_CTRL */
    return mpmo_reg_DEBUG_CTRL_poll( b_ptr,
                                     h_ptr,
                                     CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_MSK,
                                     (value<<CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ID_OFF),
                                     cmp,
                                     max_count,
                                     num_failed_polls,
                                     delay_between_polls_in_microseconds);
}

static INLINE void mpmo_field_DATA_AVAIL_CONT_HO_EN_set( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DATA_AVAIL_CONT_HO_EN_set( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DATA_AVAIL_CONT_HO_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_DATA_AVAIL_CONT_HO_EN_set", value );

    /* (0x00000008 bits 28) field DATA_AVAIL_CONT_HO_EN of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    mpmo_reg_DATA_AVAIL_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_CONT_HO_EN_MSK,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_CONT_HO_EN_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_DATA_AVAIL_CONT_HO_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DATA_AVAIL_CONT_HO_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 28) field DATA_AVAIL_CONT_HO_EN of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    reg_value = mpmo_reg_DATA_AVAIL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_CONT_HO_EN_MSK) >> CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_CONT_HO_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_DATA_AVAIL_CONT_HO_EN_get", value );

    return value;
}
static INLINE void mpmo_field_DATA_AVAIL_SQUELCH_EN_set( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DATA_AVAIL_SQUELCH_EN_set( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DATA_AVAIL_SQUELCH_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_DATA_AVAIL_SQUELCH_EN_set", value );

    /* (0x00000008 bits 24) field DATA_AVAIL_SQUELCH_EN of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    mpmo_reg_DATA_AVAIL_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_SQUELCH_EN_MSK,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_SQUELCH_EN_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_DATA_AVAIL_SQUELCH_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DATA_AVAIL_SQUELCH_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 24) field DATA_AVAIL_SQUELCH_EN of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    reg_value = mpmo_reg_DATA_AVAIL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_SQUELCH_EN_MSK) >> CORE_REG_DATA_AVAIL_CTRL_BIT_DATA_AVAIL_SQUELCH_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_DATA_AVAIL_SQUELCH_EN_get", value );

    return value;
}
static INLINE void mpmo_field_HOLDOFF_ENB_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_HOLDOFF_ENB_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_HOLDOFF_ENB_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_HOLDOFF_ENB_set", value );

    /* (0x00000008 bits 20) field HOLDOFF_ENB of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    mpmo_reg_DATA_AVAIL_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_HOLDOFF_ENB_MSK,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_HOLDOFF_ENB_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_HOLDOFF_ENB_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_HOLDOFF_ENB_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 20) field HOLDOFF_ENB of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    reg_value = mpmo_reg_DATA_AVAIL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DATA_AVAIL_CTRL_BIT_HOLDOFF_ENB_MSK) >> CORE_REG_DATA_AVAIL_CTRL_BIT_HOLDOFF_ENB_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_HOLDOFF_ENB_get", value );

    return value;
}
static INLINE void mpmo_field_FORCE_DATA_AVAIL_EN_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_FORCE_DATA_AVAIL_EN_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_FORCE_DATA_AVAIL_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_FORCE_DATA_AVAIL_EN_set", value );

    /* (0x00000008 bits 16) field FORCE_DATA_AVAIL_EN of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    mpmo_reg_DATA_AVAIL_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_FORCE_DATA_AVAIL_EN_MSK,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_FORCE_DATA_AVAIL_EN_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_FORCE_DATA_AVAIL_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_FORCE_DATA_AVAIL_EN_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 16) field FORCE_DATA_AVAIL_EN of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    reg_value = mpmo_reg_DATA_AVAIL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DATA_AVAIL_CTRL_BIT_FORCE_DATA_AVAIL_EN_MSK) >> CORE_REG_DATA_AVAIL_CTRL_BIT_FORCE_DATA_AVAIL_EN_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_FORCE_DATA_AVAIL_EN_get", value );

    return value;
}
static INLINE void mpmo_field_BURST_SIZE_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_BURST_SIZE_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_BURST_SIZE_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "mpmo_field_BURST_SIZE_set", value );

    /* (0x00000008 bits 9:0) field BURST_SIZE of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    mpmo_reg_DATA_AVAIL_CTRL_field_set( b_ptr,
                                        h_ptr,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_MSK,
                                        CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_BURST_SIZE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_BURST_SIZE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 9:0) field BURST_SIZE of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    reg_value = mpmo_reg_DATA_AVAIL_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_MSK) >> CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_BURST_SIZE_get", value );

    return value;
}
static INLINE void mpmo_field_range_BURST_SIZE_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_BURST_SIZE_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_BURST_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_BURST_SIZE_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_BURST_SIZE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000008 bits 9:0) field BURST_SIZE of register PMC_CORE_REG_DATA_AVAIL_CTRL */
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
        /* (0x00000008 bits 9:0) field BURST_SIZE of register PMC_CORE_REG_DATA_AVAIL_CTRL */
        mpmo_reg_DATA_AVAIL_CTRL_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_OFF + subfield_offset),
                                            CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_BURST_SIZE_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_BURST_SIZE_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_BURST_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_BURST_SIZE_get", stop_bit, 9 );
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
    /* (0x00000008 bits 9:0) field BURST_SIZE of register PMC_CORE_REG_DATA_AVAIL_CTRL */
    reg_value = mpmo_reg_DATA_AVAIL_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_MSK)
                  >> CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_MSK, CORE_REG_DATA_AVAIL_CTRL_BIT_BURST_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_BURST_SIZE_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size26
 * ==================================================================================
 */
static INLINE void mpmo_field_CKCTL_EN_set( mpmo_buffer_t *b_ptr,
                                            mpmo_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CKCTL_EN_set( mpmo_buffer_t *b_ptr,
                                            mpmo_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 25)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CKCTL_EN_set", N, 25);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CKCTL_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_CKCTL_EN_set", N, value );

    /* ((0x00000040 + (N) * 0x4) bits 16) field CKCTL_EN of register PMC_CORE_REG_CKCTL_CTRL_REG index N=0..25 */
    mpmo_reg_CKCTL_CTRL_REG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_EN_MSK,
                                             CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_EN_OFF,
                                             value);
}

static INLINE UINT32 mpmo_field_CKCTL_EN_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CKCTL_EN_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 25)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CKCTL_EN_get", N, 25);
    /* ((0x00000040 + (N) * 0x4) bits 16) field CKCTL_EN of register PMC_CORE_REG_CKCTL_CTRL_REG index N=0..25 */
    reg_value = mpmo_reg_CKCTL_CTRL_REG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_EN_MSK) >> CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_CKCTL_EN_get", N, value );

    return value;
}
static INLINE void mpmo_field_CKCTL_PORT_ID_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CKCTL_PORT_ID_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 25)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CKCTL_PORT_ID_set", N, 25);
    if (value > 127)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CKCTL_PORT_ID_set", value, 127);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_CKCTL_PORT_ID_set", N, value );

    /* ((0x00000040 + (N) * 0x4) bits 0:6) field CKCTL_PORT_ID of register PMC_CORE_REG_CKCTL_CTRL_REG index N=0..25 */
    mpmo_reg_CKCTL_CTRL_REG_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_MSK,
                                             CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_OFF,
                                             value);
}

static INLINE UINT32 mpmo_field_CKCTL_PORT_ID_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CKCTL_PORT_ID_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 25)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CKCTL_PORT_ID_get", N, 25);
    /* ((0x00000040 + (N) * 0x4) bits 0:6) field CKCTL_PORT_ID of register PMC_CORE_REG_CKCTL_CTRL_REG index N=0..25 */
    reg_value = mpmo_reg_CKCTL_CTRL_REG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    value = (reg_value & CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_MSK) >> CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_CKCTL_PORT_ID_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_CKCTL_PORT_ID_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_CKCTL_PORT_ID_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 25)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_CKCTL_PORT_ID_set", N, 25);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CKCTL_PORT_ID_set", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CKCTL_PORT_ID_set", stop_bit, 6 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CKCTL_PORT_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000040 + (N) * 0x4) bits 0:6) field CKCTL_PORT_ID of register PMC_CORE_REG_CKCTL_CTRL_REG index N=0..25 */
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
        /* ((0x00000040 + (N) * 0x4) bits 0:6) field CKCTL_PORT_ID of register PMC_CORE_REG_CKCTL_CTRL_REG index N=0..25 */
        mpmo_reg_CKCTL_CTRL_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 subfield_mask << (CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_OFF + subfield_offset),
                                                 CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_CKCTL_PORT_ID_get( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_CKCTL_PORT_ID_get( mpmo_buffer_t *b_ptr,
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

    if (N > 25)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_CKCTL_PORT_ID_get", N, 25);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CKCTL_PORT_ID_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CKCTL_PORT_ID_get", stop_bit, 6 );
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
    /* ((0x00000040 + (N) * 0x4) bits 0:6) field CKCTL_PORT_ID of register PMC_CORE_REG_CKCTL_CTRL_REG index N=0..25 */
    reg_value = mpmo_reg_CKCTL_CTRL_REG_array_read( b_ptr,
                                                    h_ptr,
                                                    N);
    field_value = (reg_value & CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_MSK)
                  >> CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_MSK, CORE_REG_CKCTL_CTRL_REG_BIT_CKCTL_PORT_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CKCTL_PORT_ID_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void mpmo_field_XOFF_THRES_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_XOFF_THRES_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_XOFF_THRES_set", N, 95);
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_XOFF_THRES_set", value, 4095);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_XOFF_THRES_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 0:11) field XOFF_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_XOFF_THRES_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_XOFF_THRES_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_XOFF_THRES_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 0:11) field XOFF_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_0_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_MSK) >> CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_XOFF_THRES_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_XOFF_THRES_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_XOFF_THRES_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_XOFF_THRES_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_XOFF_THRES_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_XOFF_THRES_set", stop_bit, 11 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_XOFF_THRES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000100 + (N) * 0x10) bits 0:11) field XOFF_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000100 + (N) * 0x10) bits 0:11) field XOFF_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
        mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_OFF + subfield_offset),
                                                   CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_XOFF_THRES_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_XOFF_THRES_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_XOFF_THRES_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_XOFF_THRES_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_XOFF_THRES_get", stop_bit, 11 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 11) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 11;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000100 + (N) * 0x10) bits 0:11) field XOFF_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_0_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_MSK)
                  >> CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_MSK, CORE_REG_MPMO_DP_CH_CFG_0_BIT_XOFF_THRES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_XOFF_THRES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_DATA_AVAIL_CTRL_EN_set( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DATA_AVAIL_CTRL_EN_set( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_DATA_AVAIL_CTRL_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DATA_AVAIL_CTRL_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_DATA_AVAIL_CTRL_EN_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 28) field DATA_AVAIL_CTRL_EN of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_CTRL_EN_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_CTRL_EN_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_DATA_AVAIL_CTRL_EN_get( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DATA_AVAIL_CTRL_EN_get( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_DATA_AVAIL_CTRL_EN_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 28) field DATA_AVAIL_CTRL_EN of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_0_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_CTRL_EN_MSK) >> CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_CTRL_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_DATA_AVAIL_CTRL_EN_get", N, value );

    return value;
}
static INLINE void mpmo_field_DATA_AVAIL_THRES_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DATA_AVAIL_THRES_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_DATA_AVAIL_THRES_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DATA_AVAIL_THRES_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_DATA_AVAIL_THRES_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 23:16) field DATA_AVAIL_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_DATA_AVAIL_THRES_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DATA_AVAIL_THRES_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_DATA_AVAIL_THRES_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 23:16) field DATA_AVAIL_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_0_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_MSK) >> CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_DATA_AVAIL_THRES_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_DATA_AVAIL_THRES_set( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_DATA_AVAIL_THRES_set( mpmo_buffer_t *b_ptr,
                                                          mpmo_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_DATA_AVAIL_THRES_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_DATA_AVAIL_THRES_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_DATA_AVAIL_THRES_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_DATA_AVAIL_THRES_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000100 + (N) * 0x10) bits 23:16) field DATA_AVAIL_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
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
        /* ((0x00000100 + (N) * 0x10) bits 23:16) field DATA_AVAIL_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
        mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_OFF + subfield_offset),
                                                   CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_DATA_AVAIL_THRES_get( mpmo_buffer_t *b_ptr,
                                                            mpmo_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_DATA_AVAIL_THRES_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_DATA_AVAIL_THRES_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_DATA_AVAIL_THRES_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_DATA_AVAIL_THRES_get", stop_bit, 7 );
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
    /* ((0x00000100 + (N) * 0x10) bits 23:16) field DATA_AVAIL_THRES of register PMC_CORE_REG_MPMO_DP_CH_CFG_0 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_0_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_MSK)
                  >> CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_MSK, CORE_REG_MPMO_DP_CH_CFG_0_BIT_DATA_AVAIL_THRES_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_DATA_AVAIL_THRES_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_PKT_SIZE_set( mpmo_buffer_t *b_ptr,
                                            mpmo_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_PKT_SIZE_set( mpmo_buffer_t *b_ptr,
                                            mpmo_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_PKT_SIZE_set", N, 95);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_PKT_SIZE_set", value, 1023);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_PKT_SIZE_set", N, value );

    /* ((0x00000104 + (N) * 0x10) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_PKT_SIZE_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_PKT_SIZE_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_PKT_SIZE_get", N, 95);
    /* ((0x00000104 + (N) * 0x10) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_1_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_MSK) >> CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_PKT_SIZE_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_PKT_SIZE_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_PKT_SIZE_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_PKT_SIZE_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_PKT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_PKT_SIZE_set", stop_bit, 9 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_PKT_SIZE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000104 + (N) * 0x10) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
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
        /* ((0x00000104 + (N) * 0x10) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
        mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_OFF + subfield_offset),
                                                   CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_PKT_SIZE_get( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_PKT_SIZE_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_PKT_SIZE_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_PKT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_PKT_SIZE_get", stop_bit, 9 );
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
    /* ((0x00000104 + (N) * 0x10) bits 25:16) field PKT_SIZE of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_1_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_MSK)
                  >> CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_MSK, CORE_REG_MPMO_DP_CH_CFG_1_BIT_PKT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_PKT_SIZE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_SSF_CN_CENTER_EN_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_SSF_CN_CENTER_EN_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_SSF_CN_CENTER_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_SSF_CN_CENTER_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_SSF_CN_CENTER_EN_set", N, value );

    /* ((0x00000104 + (N) * 0x10) bits 12) field SSF_CN_CENTER_EN of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_SSF_CN_CENTER_EN_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_SSF_CN_CENTER_EN_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_SSF_CN_CENTER_EN_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_SSF_CN_CENTER_EN_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_SSF_CN_CENTER_EN_get", N, 95);
    /* ((0x00000104 + (N) * 0x10) bits 12) field SSF_CN_CENTER_EN of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_1_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_1_BIT_SSF_CN_CENTER_EN_MSK) >> CORE_REG_MPMO_DP_CH_CFG_1_BIT_SSF_CN_CENTER_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_SSF_CN_CENTER_EN_get", N, value );

    return value;
}
static INLINE void mpmo_field_TS_ENABLE_set( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_TS_ENABLE_set( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_TS_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_TS_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_TS_ENABLE_set", N, value );

    /* ((0x00000104 + (N) * 0x10) bits 8) field TS_ENABLE of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_TS_ENABLE_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_TS_ENABLE_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_TS_ENABLE_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_TS_ENABLE_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_TS_ENABLE_get", N, 95);
    /* ((0x00000104 + (N) * 0x10) bits 8) field TS_ENABLE of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_1_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_1_BIT_TS_ENABLE_MSK) >> CORE_REG_MPMO_DP_CH_CFG_1_BIT_TS_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_TS_ENABLE_get", N, value );

    return value;
}
static INLINE void mpmo_field_RAMP_EN_set( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_RAMP_EN_set( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_RAMP_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_RAMP_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_RAMP_EN_set", N, value );

    /* ((0x00000104 + (N) * 0x10) bits 4) field RAMP_EN of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_EN_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_EN_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_RAMP_EN_get( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_RAMP_EN_get( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_RAMP_EN_get", N, 95);
    /* ((0x00000104 + (N) * 0x10) bits 4) field RAMP_EN of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_1_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_EN_MSK) >> CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_RAMP_EN_get", N, value );

    return value;
}
static INLINE void mpmo_field_CH_EN_set( mpmo_buffer_t *b_ptr,
                                         mpmo_handle_t *h_ptr,
                                         UINT32  N,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CH_EN_set( mpmo_buffer_t *b_ptr,
                                         mpmo_handle_t *h_ptr,
                                         UINT32  N,
                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CH_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CH_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_CH_EN_set", N, value );

    /* ((0x00000104 + (N) * 0x10) bits 0) field CH_EN of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_CH_EN_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_CH_EN_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_CH_EN_get( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CH_EN_get( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CH_EN_get", N, 95);
    /* ((0x00000104 + (N) * 0x10) bits 0) field CH_EN of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_1_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_1_BIT_CH_EN_MSK) >> CORE_REG_MPMO_DP_CH_CFG_1_BIT_CH_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_CH_EN_get", N, value );

    return value;
}
static INLINE void mpmo_field_RAMP_UP_DWN2_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_RAMP_UP_DWN2_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_RAMP_UP_DWN2_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_RAMP_UP_DWN2_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_RAMP_UP_DWN2_set", N, value );

    /* ((0x00000104 + (N) * 0x10) bits 2) field RAMP_UP_DWN2 of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_UP_DWN2_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_UP_DWN2_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_RAMP_UP_DWN2_get( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_RAMP_UP_DWN2_get( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_RAMP_UP_DWN2_get", N, 95);
    /* ((0x00000104 + (N) * 0x10) bits 2) field RAMP_UP_DWN2 of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_1_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_UP_DWN2_MSK) >> CORE_REG_MPMO_DP_CH_CFG_1_BIT_RAMP_UP_DWN2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_RAMP_UP_DWN2_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void mpmo_field_DEBUG_DATA_REQ_ABORT_set( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DEBUG_DATA_REQ_ABORT_set( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DEBUG_DATA_REQ_ABORT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_DEBUG_DATA_REQ_ABORT_set", value );

    /* (0x00000028 bits 28) field DEBUG_DATA_REQ_ABORT of register PMC_CORE_REG_DEBUG_CTRL */
    mpmo_reg_DEBUG_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ABORT_MSK,
                                   CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ABORT_OFF,
                                   value);
}

static INLINE UINT32 mpmo_field_DEBUG_DATA_REQ_ABORT_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DEBUG_DATA_REQ_ABORT_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 28) field DEBUG_DATA_REQ_ABORT of register PMC_CORE_REG_DEBUG_CTRL */
    reg_value = mpmo_reg_DEBUG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ABORT_MSK) >> CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ABORT_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_DEBUG_DATA_REQ_ABORT_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_ABORT_poll( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_ABORT_poll( mpmo_buffer_t *b_ptr,
                                                                         mpmo_handle_t *h_ptr,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mpmo_field_DEBUG_DATA_REQ_ABORT_poll", value );

    /* (0x00000028 bits 28) field DEBUG_DATA_REQ_ABORT of register PMC_CORE_REG_DEBUG_CTRL */
    return mpmo_reg_DEBUG_CTRL_poll( b_ptr,
                                     h_ptr,
                                     CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ABORT_MSK,
                                     (value<<CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_ABORT_OFF),
                                     cmp,
                                     max_count,
                                     num_failed_polls,
                                     delay_between_polls_in_microseconds);
}

static INLINE void mpmo_field_DEBUG_DATA_REQ_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_DEBUG_DATA_REQ_set( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_DEBUG_DATA_REQ_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mpmo_field_DEBUG_DATA_REQ_set", value );

    /* (0x00000028 bits 24) field DEBUG_DATA_REQ of register PMC_CORE_REG_DEBUG_CTRL */
    mpmo_reg_DEBUG_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_MSK,
                                   CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_OFF,
                                   value);
}

static INLINE UINT32 mpmo_field_DEBUG_DATA_REQ_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_DEBUG_DATA_REQ_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 24) field DEBUG_DATA_REQ of register PMC_CORE_REG_DEBUG_CTRL */
    reg_value = mpmo_reg_DEBUG_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_MSK) >> CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_DEBUG_DATA_REQ_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_poll( mpmo_buffer_t *b_ptr,
                                                                   mpmo_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_poll( mpmo_buffer_t *b_ptr,
                                                                   mpmo_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "mpmo_field_DEBUG_DATA_REQ_poll", value );

    /* (0x00000028 bits 24) field DEBUG_DATA_REQ of register PMC_CORE_REG_DEBUG_CTRL */
    return mpmo_reg_DEBUG_CTRL_poll( b_ptr,
                                     h_ptr,
                                     CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_MSK,
                                     (value<<CORE_REG_DEBUG_CTRL_BIT_DEBUG_DATA_REQ_OFF),
                                     cmp,
                                     max_count,
                                     num_failed_polls,
                                     delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge_N_size96
 * ==================================================================================
 */
static INLINE void mpmo_field_HOLDOFF_TRIG_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_HOLDOFF_TRIG_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_HOLDOFF_TRIG_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_HOLDOFF_TRIG_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_HOLDOFF_TRIG_set", N, value );

    /* ((0x00000104 + (N) * 0x10) bits 28) field HOLDOFF_TRIG of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_HOLDOFF_TRIG_MSK,
                                               CORE_REG_MPMO_DP_CH_CFG_1_BIT_HOLDOFF_TRIG_OFF,
                                               value);
}

static INLINE UINT32 mpmo_field_HOLDOFF_TRIG_get( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_HOLDOFF_TRIG_get( mpmo_buffer_t *b_ptr,
                                                  mpmo_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_HOLDOFF_TRIG_get", N, 95);
    /* ((0x00000104 + (N) * 0x10) bits 28) field HOLDOFF_TRIG of register PMC_CORE_REG_MPMO_DP_CH_CFG_1 index N=0..95 */
    reg_value = mpmo_reg_MPMO_DP_CH_CFG_1_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & CORE_REG_MPMO_DP_CH_CFG_1_BIT_HOLDOFF_TRIG_MSK) >> CORE_REG_MPMO_DP_CH_CFG_1_BIT_HOLDOFF_TRIG_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_HOLDOFF_TRIG_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void mpmo_lfield_CSI_CHG_E_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_CSI_CHG_E_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_CSI_CHG_E_set", value[2] , value[1] , value[0] );

    /* (0x00001664 bits 0:31) bits 0:11 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_0 */
    mpmo_reg_CSI_CHG_INT_EN_REG_0_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_MSK,
                                             CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_OFF,
                                             value[0]);

    /* (0x00001680 bits 31:0) bits 32:63 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_1 */
    mpmo_reg_CSI_CHG_INT_EN_REG_1_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_MSK,
                                             CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_OFF,
                                             value[1]);

    /* (0x00001684 bits 31:0) bits 64:95 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_2 */
    mpmo_reg_CSI_CHG_INT_EN_REG_2_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_MSK,
                                             CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_OFF,
                                             value[2]);
}

static INLINE void mpmo_lfield_CSI_CHG_E_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_CSI_CHG_E_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001664 bits 0:31) bits 0:11 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_0 */
    reg_value = mpmo_reg_CSI_CHG_INT_EN_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_MSK) >> CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_OFF;
    value[0] |= field_value;

    /* (0x00001680 bits 31:0) bits 32:63 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_1 */
    reg_value = mpmo_reg_CSI_CHG_INT_EN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_MSK) >> CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_OFF;
    value[1] |= field_value;

    /* (0x00001684 bits 31:0) bits 64:95 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_2 */
    reg_value = mpmo_reg_CSI_CHG_INT_EN_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_MSK) >> CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_CSI_CHG_E_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_CSI_CHG_E_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_CSI_CHG_E_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_CSI_CHG_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_CSI_CHG_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_CSI_CHG_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001664 bits 0:31) bits 0:11 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001664 bits 0:31) bits 0:11 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_0 */
        mpmo_reg_CSI_CHG_INT_EN_REG_0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_OFF + subfield_offset),
                                                 CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001680 bits 31:0) bits 32:63 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_1 */
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
        /* (0x00001680 bits 31:0) bits 32:63 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_1 */
        mpmo_reg_CSI_CHG_INT_EN_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_OFF + subfield_offset),
                                                 CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001684 bits 31:0) bits 64:95 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_2 */
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
        /* (0x00001684 bits 31:0) bits 64:95 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_2 */
        mpmo_reg_CSI_CHG_INT_EN_REG_2_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_OFF + subfield_offset),
                                                 CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_CSI_CHG_E_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_CSI_CHG_E_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_CSI_CHG_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_CSI_CHG_E_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001664 bits 0:31) bits 0:11 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_0 */
        reg_value = mpmo_reg_CSI_CHG_INT_EN_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_MSK)
                      >> CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_MSK, CORE_REG_CSI_CHG_INT_EN_REG_0_BIT_CSI_CHG_E_OFF, field_value );
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
    /* (0x00001680 bits 31:0) bits 32:63 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_1 */
        reg_value = mpmo_reg_CSI_CHG_INT_EN_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_MSK)
                      >> CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_MSK, CORE_REG_CSI_CHG_INT_EN_REG_1_BIT_CSI_CHG_E_OFF, field_value );
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
    /* (0x00001684 bits 31:0) bits 64:95 use field CSI_CHG_E of register PMC_CORE_REG_CSI_CHG_INT_EN_REG_2 */
        reg_value = mpmo_reg_CSI_CHG_INT_EN_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_MSK)
                      >> CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_MSK, CORE_REG_CSI_CHG_INT_EN_REG_2_BIT_CSI_CHG_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_CSI_CHG_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_lfield_SSF_GEN_E_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_GEN_E_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_GEN_E_set", value[2] , value[1] , value[0] );

    /* (0x00001688 bits 0:31) bits 0:11 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_0 */
    mpmo_reg_SSF_GEN_INT_EN_REG_0_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_MSK,
                                             CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_OFF,
                                             value[0]);

    /* (0x0000168c bits 31:0) bits 32:63 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_1 */
    mpmo_reg_SSF_GEN_INT_EN_REG_1_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_MSK,
                                             CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_OFF,
                                             value[1]);

    /* (0x000016a0 bits 31:0) bits 64:95 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_2 */
    mpmo_reg_SSF_GEN_INT_EN_REG_2_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_MSK,
                                             CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_OFF,
                                             value[2]);
}

static INLINE void mpmo_lfield_SSF_GEN_E_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_GEN_E_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001688 bits 0:31) bits 0:11 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_0 */
    reg_value = mpmo_reg_SSF_GEN_INT_EN_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_MSK) >> CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_OFF;
    value[0] |= field_value;

    /* (0x0000168c bits 31:0) bits 32:63 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_1 */
    reg_value = mpmo_reg_SSF_GEN_INT_EN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_MSK) >> CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_OFF;
    value[1] |= field_value;

    /* (0x000016a0 bits 31:0) bits 64:95 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_2 */
    reg_value = mpmo_reg_SSF_GEN_INT_EN_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_MSK) >> CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_GEN_E_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_SSF_GEN_E_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_SSF_GEN_E_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_GEN_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_GEN_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_GEN_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001688 bits 0:31) bits 0:11 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001688 bits 0:31) bits 0:11 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_0 */
        mpmo_reg_SSF_GEN_INT_EN_REG_0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_OFF + subfield_offset),
                                                 CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000168c bits 31:0) bits 32:63 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_1 */
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
        /* (0x0000168c bits 31:0) bits 32:63 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_1 */
        mpmo_reg_SSF_GEN_INT_EN_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_OFF + subfield_offset),
                                                 CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016a0 bits 31:0) bits 64:95 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_2 */
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
        /* (0x000016a0 bits 31:0) bits 64:95 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_2 */
        mpmo_reg_SSF_GEN_INT_EN_REG_2_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_OFF + subfield_offset),
                                                 CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_SSF_GEN_E_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_SSF_GEN_E_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_GEN_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_GEN_E_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001688 bits 0:31) bits 0:11 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_0 */
        reg_value = mpmo_reg_SSF_GEN_INT_EN_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_MSK)
                      >> CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_MSK, CORE_REG_SSF_GEN_INT_EN_REG_0_BIT_SSF_GEN_E_OFF, field_value );
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
    /* (0x0000168c bits 31:0) bits 32:63 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_1 */
        reg_value = mpmo_reg_SSF_GEN_INT_EN_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_MSK)
                      >> CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_MSK, CORE_REG_SSF_GEN_INT_EN_REG_1_BIT_SSF_GEN_E_OFF, field_value );
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
    /* (0x000016a0 bits 31:0) bits 64:95 use field SSF_GEN_E of register PMC_CORE_REG_SSF_GEN_INT_EN_REG_2 */
        reg_value = mpmo_reg_SSF_GEN_INT_EN_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_MSK)
                      >> CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_MSK, CORE_REG_SSF_GEN_INT_EN_REG_2_BIT_SSF_GEN_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_GEN_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_lfield_PKT_DROP_E_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[6] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_PKT_DROP_E_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[6] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "mpmo_lfield_PKT_DROP_E_set", value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

    /* (0x000016a4 bits 0:31) bits 0:23 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_0 */
    mpmo_reg_PKT_DROP_INT_EN_REG_0_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_MSK,
                                              CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_OFF,
                                              value[0]);

    /* (0x000016a8 bits 31:0) bits 32:63 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_1 */
    mpmo_reg_PKT_DROP_INT_EN_REG_1_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_MSK,
                                              CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_OFF,
                                              value[1]);

    /* (0x000016ac bits 31:0) bits 64:95 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_2 */
    mpmo_reg_PKT_DROP_INT_EN_REG_2_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_MSK,
                                              CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_OFF,
                                              value[2]);

    /* (0x000016b0 bits 31:0) bits 96:127 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_3 */
    mpmo_reg_PKT_DROP_INT_EN_REG_3_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_MSK,
                                              CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_OFF,
                                              value[3]);

    /* (0x000016b4 bits 31:0) bits 128:159 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_4 */
    mpmo_reg_PKT_DROP_INT_EN_REG_4_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_MSK,
                                              CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_OFF,
                                              value[4]);

    /* (0x000016b8 bits 31:0) bits 160:191 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_5 */
    mpmo_reg_PKT_DROP_INT_EN_REG_5_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_MSK,
                                              CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_OFF,
                                              value[5]);
}

static INLINE void mpmo_lfield_PKT_DROP_E_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[6] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_PKT_DROP_E_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[6] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<6;i++)
        value[i] = 0;

    /* (0x000016a4 bits 0:31) bits 0:23 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_0 */
    reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_MSK) >> CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_OFF;
    value[0] |= field_value;

    /* (0x000016a8 bits 31:0) bits 32:63 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_1 */
    reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_MSK) >> CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_OFF;
    value[1] |= field_value;

    /* (0x000016ac bits 31:0) bits 64:95 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_2 */
    reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_MSK) >> CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_OFF;
    value[2] |= field_value;

    /* (0x000016b0 bits 31:0) bits 96:127 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_3 */
    reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_3_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_MSK) >> CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_OFF;
    value[3] |= field_value;

    /* (0x000016b4 bits 31:0) bits 128:159 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_4 */
    reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_4_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_MSK) >> CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_OFF;
    value[4] |= field_value;

    /* (0x000016b8 bits 31:0) bits 160:191 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_5 */
    reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_5_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_MSK) >> CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_OFF;
    value[5] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "mpmo_lfield_PKT_DROP_E_get", value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_PKT_DROP_E_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_PKT_DROP_E_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_PKT_DROP_E_set", stop_bit, start_bit );
    if (stop_bit > 191) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_PKT_DROP_E_set", stop_bit, 191 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_PKT_DROP_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016a4 bits 0:31) bits 0:23 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_0 */
    if (start_bit <= 23) {
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
        /* (0x000016a4 bits 0:31) bits 0:23 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_0 */
        mpmo_reg_PKT_DROP_INT_EN_REG_0_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_OFF + subfield_offset),
                                                  CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016a8 bits 31:0) bits 32:63 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_1 */
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
        /* (0x000016a8 bits 31:0) bits 32:63 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_1 */
        mpmo_reg_PKT_DROP_INT_EN_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_OFF + subfield_offset),
                                                  CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016ac bits 31:0) bits 64:95 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_2 */
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
        /* (0x000016ac bits 31:0) bits 64:95 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_2 */
        mpmo_reg_PKT_DROP_INT_EN_REG_2_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_OFF + subfield_offset),
                                                  CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016b0 bits 31:0) bits 96:127 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_3 */
    if ((start_bit <= 127) && (stop_bit >= 96)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 96) {
            subfield_offset = start_bit - 96;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 96 - start_bit;
            subfield_start_bit = 96;
        }
        if (stop_bit < 127) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 127;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000016b0 bits 31:0) bits 96:127 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_3 */
        mpmo_reg_PKT_DROP_INT_EN_REG_3_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_OFF + subfield_offset),
                                                  CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016b4 bits 31:0) bits 128:159 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_4 */
    if ((start_bit <= 159) && (stop_bit >= 128)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 128) {
            subfield_offset = start_bit - 128;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 128 - start_bit;
            subfield_start_bit = 128;
        }
        if (stop_bit < 159) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 159;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000016b4 bits 31:0) bits 128:159 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_4 */
        mpmo_reg_PKT_DROP_INT_EN_REG_4_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_OFF + subfield_offset),
                                                  CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016b8 bits 31:0) bits 160:191 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_5 */
    if ((start_bit <= 191) && (stop_bit >= 160)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 160) {
            subfield_offset = start_bit - 160;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 160 - start_bit;
            subfield_start_bit = 160;
        }
        if (stop_bit < 191) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 191;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000016b8 bits 31:0) bits 160:191 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_5 */
        mpmo_reg_PKT_DROP_INT_EN_REG_5_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_OFF + subfield_offset),
                                                  CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_PKT_DROP_E_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_PKT_DROP_E_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_PKT_DROP_E_get", stop_bit, start_bit );
    if (stop_bit > 191) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_PKT_DROP_E_get", stop_bit, 191 );
    if (start_bit <= 23) {
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
    /* (0x000016a4 bits 0:31) bits 0:23 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_0 */
        reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_MSK)
                      >> CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_MSK, CORE_REG_PKT_DROP_INT_EN_REG_0_BIT_PKT_DROP_E_OFF, field_value );
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
    /* (0x000016a8 bits 31:0) bits 32:63 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_1 */
        reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_MSK)
                      >> CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_MSK, CORE_REG_PKT_DROP_INT_EN_REG_1_BIT_PKT_DROP_E_OFF, field_value );
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
    /* (0x000016ac bits 31:0) bits 64:95 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_2 */
        reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_MSK)
                      >> CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_MSK, CORE_REG_PKT_DROP_INT_EN_REG_2_BIT_PKT_DROP_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 127) && (stop_bit >= 96)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 96) {
            subfield_offset = start_bit - 96;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 96 - start_bit;
            subfield_start_bit = 96;
        }
        if (stop_bit < 127) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 127;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000016b0 bits 31:0) bits 96:127 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_3 */
        reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_3_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_MSK)
                      >> CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_MSK, CORE_REG_PKT_DROP_INT_EN_REG_3_BIT_PKT_DROP_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 159) && (stop_bit >= 128)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 128) {
            subfield_offset = start_bit - 128;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 128 - start_bit;
            subfield_start_bit = 128;
        }
        if (stop_bit < 159) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 159;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000016b4 bits 31:0) bits 128:159 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_4 */
        reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_4_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_MSK)
                      >> CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_MSK, CORE_REG_PKT_DROP_INT_EN_REG_4_BIT_PKT_DROP_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 191) && (stop_bit >= 160)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 160) {
            subfield_offset = start_bit - 160;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 160 - start_bit;
            subfield_start_bit = 160;
        }
        if (stop_bit < 191) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 191;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000016b8 bits 31:0) bits 160:191 use field PKT_DROP_E of register PMC_CORE_REG_PKT_DROP_INT_EN_REG_5 */
        reg_value = mpmo_reg_PKT_DROP_INT_EN_REG_5_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_MSK)
                      >> CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_MSK, CORE_REG_PKT_DROP_INT_EN_REG_5_BIT_PKT_DROP_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_PKT_DROP_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_lfield_EOP_ERR_E_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_EOP_ERR_E_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_EOP_ERR_E_set", value[2] , value[1] , value[0] );

    /* (0x000016bc bits 0:31) bits 0:11 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_0 */
    mpmo_reg_EOP_ERR_INT_EN_REG_0_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_MSK,
                                             CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_OFF,
                                             value[0]);

    /* (0x000016c0 bits 31:0) bits 32:63 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_1 */
    mpmo_reg_EOP_ERR_INT_EN_REG_1_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_MSK,
                                             CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_OFF,
                                             value[1]);

    /* (0x000016c4 bits 31:0) bits 64:95 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_2 */
    mpmo_reg_EOP_ERR_INT_EN_REG_2_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_MSK,
                                             CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_OFF,
                                             value[2]);
}

static INLINE void mpmo_lfield_EOP_ERR_E_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_EOP_ERR_E_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x000016bc bits 0:31) bits 0:11 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_0 */
    reg_value = mpmo_reg_EOP_ERR_INT_EN_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_MSK) >> CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_OFF;
    value[0] |= field_value;

    /* (0x000016c0 bits 31:0) bits 32:63 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_1 */
    reg_value = mpmo_reg_EOP_ERR_INT_EN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_MSK) >> CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_OFF;
    value[1] |= field_value;

    /* (0x000016c4 bits 31:0) bits 64:95 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_2 */
    reg_value = mpmo_reg_EOP_ERR_INT_EN_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_MSK) >> CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_EOP_ERR_E_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_EOP_ERR_E_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_EOP_ERR_E_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_EOP_ERR_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_EOP_ERR_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_EOP_ERR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016bc bits 0:31) bits 0:11 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000016bc bits 0:31) bits 0:11 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_0 */
        mpmo_reg_EOP_ERR_INT_EN_REG_0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_OFF + subfield_offset),
                                                 CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016c0 bits 31:0) bits 32:63 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_1 */
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
        /* (0x000016c0 bits 31:0) bits 32:63 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_1 */
        mpmo_reg_EOP_ERR_INT_EN_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_OFF + subfield_offset),
                                                 CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016c4 bits 31:0) bits 64:95 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_2 */
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
        /* (0x000016c4 bits 31:0) bits 64:95 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_2 */
        mpmo_reg_EOP_ERR_INT_EN_REG_2_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_OFF + subfield_offset),
                                                 CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_EOP_ERR_E_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_EOP_ERR_E_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_EOP_ERR_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_EOP_ERR_E_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000016bc bits 0:31) bits 0:11 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_0 */
        reg_value = mpmo_reg_EOP_ERR_INT_EN_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_MSK)
                      >> CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_MSK, CORE_REG_EOP_ERR_INT_EN_REG_0_BIT_EOP_ERR_E_OFF, field_value );
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
    /* (0x000016c0 bits 31:0) bits 32:63 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_1 */
        reg_value = mpmo_reg_EOP_ERR_INT_EN_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_MSK)
                      >> CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_MSK, CORE_REG_EOP_ERR_INT_EN_REG_1_BIT_EOP_ERR_E_OFF, field_value );
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
    /* (0x000016c4 bits 31:0) bits 64:95 use field EOP_ERR_E of register PMC_CORE_REG_EOP_ERR_INT_EN_REG_2 */
        reg_value = mpmo_reg_EOP_ERR_INT_EN_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_MSK)
                      >> CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_MSK, CORE_REG_EOP_ERR_INT_EN_REG_2_BIT_EOP_ERR_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_EOP_ERR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_lfield_SOP_ERR_E_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SOP_ERR_E_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SOP_ERR_E_set", value[2] , value[1] , value[0] );

    /* (0x000016c8 bits 0:31) bits 0:11 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_0 */
    mpmo_reg_SOP_ERR_INT_EN_REG_0_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_MSK,
                                             CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_OFF,
                                             value[0]);

    /* (0x000016cc bits 31:0) bits 32:63 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_1 */
    mpmo_reg_SOP_ERR_INT_EN_REG_1_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_MSK,
                                             CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_OFF,
                                             value[1]);

    /* (0x000016d0 bits 31:0) bits 64:95 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_2 */
    mpmo_reg_SOP_ERR_INT_EN_REG_2_field_set( b_ptr,
                                             h_ptr,
                                             CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_MSK,
                                             CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_OFF,
                                             value[2]);
}

static INLINE void mpmo_lfield_SOP_ERR_E_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SOP_ERR_E_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x000016c8 bits 0:31) bits 0:11 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_0 */
    reg_value = mpmo_reg_SOP_ERR_INT_EN_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_MSK) >> CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_OFF;
    value[0] |= field_value;

    /* (0x000016cc bits 31:0) bits 32:63 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_1 */
    reg_value = mpmo_reg_SOP_ERR_INT_EN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_MSK) >> CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_OFF;
    value[1] |= field_value;

    /* (0x000016d0 bits 31:0) bits 64:95 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_2 */
    reg_value = mpmo_reg_SOP_ERR_INT_EN_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_MSK) >> CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SOP_ERR_E_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_SOP_ERR_E_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_SOP_ERR_E_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SOP_ERR_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SOP_ERR_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SOP_ERR_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016c8 bits 0:31) bits 0:11 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x000016c8 bits 0:31) bits 0:11 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_0 */
        mpmo_reg_SOP_ERR_INT_EN_REG_0_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_OFF + subfield_offset),
                                                 CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016cc bits 31:0) bits 32:63 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_1 */
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
        /* (0x000016cc bits 31:0) bits 32:63 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_1 */
        mpmo_reg_SOP_ERR_INT_EN_REG_1_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_OFF + subfield_offset),
                                                 CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000016d0 bits 31:0) bits 64:95 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_2 */
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
        /* (0x000016d0 bits 31:0) bits 64:95 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_2 */
        mpmo_reg_SOP_ERR_INT_EN_REG_2_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_OFF + subfield_offset),
                                                 CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_SOP_ERR_E_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_SOP_ERR_E_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SOP_ERR_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SOP_ERR_E_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x000016c8 bits 0:31) bits 0:11 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_0 */
        reg_value = mpmo_reg_SOP_ERR_INT_EN_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_MSK)
                      >> CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_MSK, CORE_REG_SOP_ERR_INT_EN_REG_0_BIT_SOP_ERR_E_OFF, field_value );
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
    /* (0x000016cc bits 31:0) bits 32:63 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_1 */
        reg_value = mpmo_reg_SOP_ERR_INT_EN_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_MSK)
                      >> CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_MSK, CORE_REG_SOP_ERR_INT_EN_REG_1_BIT_SOP_ERR_E_OFF, field_value );
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
    /* (0x000016d0 bits 31:0) bits 64:95 use field SOP_ERR_E of register PMC_CORE_REG_SOP_ERR_INT_EN_REG_2 */
        reg_value = mpmo_reg_SOP_ERR_INT_EN_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_MSK)
                      >> CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_MSK, CORE_REG_SOP_ERR_INT_EN_REG_2_BIT_SOP_ERR_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SOP_ERR_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_lfield_BWR_RAMP_E_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_BWR_RAMP_E_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_BWR_RAMP_E_set", value[2] , value[1] , value[0] );

    /* (0x00001708 bits 0:31) bits 0:11 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0 */
    mpmo_reg_BWR_RAMP_INT_EN_REG_0_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_MSK,
                                              CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_OFF,
                                              value[0]);

    /* (0x0000170c bits 31:0) bits 32:63 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1 */
    mpmo_reg_BWR_RAMP_INT_EN_REG_1_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_MSK,
                                              CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_OFF,
                                              value[1]);

    /* (0x00001710 bits 31:0) bits 64:95 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2 */
    mpmo_reg_BWR_RAMP_INT_EN_REG_2_field_set( b_ptr,
                                              h_ptr,
                                              CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_MSK,
                                              CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_OFF,
                                              value[2]);
}

static INLINE void mpmo_lfield_BWR_RAMP_E_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_BWR_RAMP_E_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001708 bits 0:31) bits 0:11 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0 */
    reg_value = mpmo_reg_BWR_RAMP_INT_EN_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_MSK) >> CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_OFF;
    value[0] |= field_value;

    /* (0x0000170c bits 31:0) bits 32:63 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1 */
    reg_value = mpmo_reg_BWR_RAMP_INT_EN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_MSK) >> CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_OFF;
    value[1] |= field_value;

    /* (0x00001710 bits 31:0) bits 64:95 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2 */
    reg_value = mpmo_reg_BWR_RAMP_INT_EN_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_MSK) >> CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_BWR_RAMP_E_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_BWR_RAMP_E_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_BWR_RAMP_E_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_BWR_RAMP_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_BWR_RAMP_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_BWR_RAMP_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001708 bits 0:31) bits 0:11 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001708 bits 0:31) bits 0:11 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0 */
        mpmo_reg_BWR_RAMP_INT_EN_REG_0_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_OFF + subfield_offset),
                                                  CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000170c bits 31:0) bits 32:63 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1 */
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
        /* (0x0000170c bits 31:0) bits 32:63 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1 */
        mpmo_reg_BWR_RAMP_INT_EN_REG_1_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_OFF + subfield_offset),
                                                  CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001710 bits 31:0) bits 64:95 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2 */
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
        /* (0x00001710 bits 31:0) bits 64:95 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2 */
        mpmo_reg_BWR_RAMP_INT_EN_REG_2_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_OFF + subfield_offset),
                                                  CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_BWR_RAMP_E_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_BWR_RAMP_E_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_BWR_RAMP_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_BWR_RAMP_E_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001708 bits 0:31) bits 0:11 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_0 */
        reg_value = mpmo_reg_BWR_RAMP_INT_EN_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_MSK)
                      >> CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_MSK, CORE_REG_BWR_RAMP_INT_EN_REG_0_BIT_BWR_RAMP_E_OFF, field_value );
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
    /* (0x0000170c bits 31:0) bits 32:63 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_1 */
        reg_value = mpmo_reg_BWR_RAMP_INT_EN_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_MSK)
                      >> CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_MSK, CORE_REG_BWR_RAMP_INT_EN_REG_1_BIT_BWR_RAMP_E_OFF, field_value );
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
    /* (0x00001710 bits 31:0) bits 64:95 use field BWR_RAMP_E of register PMC_CORE_REG_BWR_RAMP_INT_EN_REG_2 */
        reg_value = mpmo_reg_BWR_RAMP_INT_EN_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_MSK)
                      >> CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_MSK, CORE_REG_BWR_RAMP_INT_EN_REG_2_BIT_BWR_RAMP_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_BWR_RAMP_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_lfield_SSF_STATE_E_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_STATE_E_set( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_STATE_E_set", value[2] , value[1] , value[0] );

    /* (0x0000173c bits 31:0) bits 0:31 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_0 */
    mpmo_reg_SSF_STATE_INT_EN_REG_0_field_set( b_ptr,
                                               h_ptr,
                                               CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_MSK,
                                               CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_OFF,
                                               value[0]);

    /* (0x00001740 bits 31:0) bits 32:63 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_1 */
    mpmo_reg_SSF_STATE_INT_EN_REG_1_field_set( b_ptr,
                                               h_ptr,
                                               CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_MSK,
                                               CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_OFF,
                                               value[1]);

    /* (0x00001744 bits 0:31) bits 64:79 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_2 */
    mpmo_reg_SSF_STATE_INT_EN_REG_2_field_set( b_ptr,
                                               h_ptr,
                                               CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_MSK,
                                               CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_OFF,
                                               value[2]);
}

static INLINE void mpmo_lfield_SSF_STATE_E_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_STATE_E_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000173c bits 31:0) bits 0:31 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_0 */
    reg_value = mpmo_reg_SSF_STATE_INT_EN_REG_0_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_MSK) >> CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_OFF;
    value[0] |= field_value;

    /* (0x00001740 bits 31:0) bits 32:63 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_1 */
    reg_value = mpmo_reg_SSF_STATE_INT_EN_REG_1_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_MSK) >> CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_OFF;
    value[1] |= field_value;

    /* (0x00001744 bits 0:31) bits 64:79 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_2 */
    reg_value = mpmo_reg_SSF_STATE_INT_EN_REG_2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_MSK) >> CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_STATE_E_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_SSF_STATE_E_set( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_SSF_STATE_E_set( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_STATE_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_STATE_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_STATE_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000173c bits 31:0) bits 0:31 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_0 */
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
        /* (0x0000173c bits 31:0) bits 0:31 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_0 */
        mpmo_reg_SSF_STATE_INT_EN_REG_0_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_OFF + subfield_offset),
                                                   CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001740 bits 31:0) bits 32:63 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_1 */
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
        /* (0x00001740 bits 31:0) bits 32:63 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_1 */
        mpmo_reg_SSF_STATE_INT_EN_REG_1_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_OFF + subfield_offset),
                                                   CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001744 bits 0:31) bits 64:79 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_2 */
    if ((start_bit <= 79) && (stop_bit >= 64)) {
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
        if (stop_bit < 79) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 79;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001744 bits 0:31) bits 64:79 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_2 */
        mpmo_reg_SSF_STATE_INT_EN_REG_2_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_OFF + subfield_offset),
                                                   CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_SSF_STATE_E_get( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_SSF_STATE_E_get( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_STATE_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_STATE_E_get", stop_bit, 95 );
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
    /* (0x0000173c bits 31:0) bits 0:31 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_0 */
        reg_value = mpmo_reg_SSF_STATE_INT_EN_REG_0_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_MSK)
                      >> CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_MSK, CORE_REG_SSF_STATE_INT_EN_REG_0_BIT_SSF_STATE_E_OFF, field_value );
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
    /* (0x00001740 bits 31:0) bits 32:63 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_1 */
        reg_value = mpmo_reg_SSF_STATE_INT_EN_REG_1_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_MSK)
                      >> CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_MSK, CORE_REG_SSF_STATE_INT_EN_REG_1_BIT_SSF_STATE_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 79) && (stop_bit >= 64)) {
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
        if (stop_bit < 79) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 79;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001744 bits 0:31) bits 64:79 use field SSF_STATE_E of register PMC_CORE_REG_SSF_STATE_INT_EN_REG_2 */
        reg_value = mpmo_reg_SSF_STATE_INT_EN_REG_2_read( b_ptr,
                                                          h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_MSK)
                      >> CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_MSK, CORE_REG_SSF_STATE_INT_EN_REG_2_BIT_SSF_STATE_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_STATE_E_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void mpmo_lfield_CSI_CHG_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_CSI_CHG_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_CSI_CHG_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x00001600 bits 0:31) bits 0:11 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_0 */
    mpmo_reg_CSI_CHG_INT_REG_0_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_MSK,
                                                          CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF,
                                                          value[0]);

    /* (0x00001604 bits 31:0) bits 32:63 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_1 */
    mpmo_reg_CSI_CHG_INT_REG_1_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_MSK,
                                                          CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF,
                                                          value[1]);

    /* (0x00001608 bits 31:0) bits 64:95 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_2 */
    mpmo_reg_CSI_CHG_INT_REG_2_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_MSK,
                                                          CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF,
                                                          value[2]);
}

static INLINE void mpmo_lfield_CSI_CHG_I_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_CSI_CHG_I_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001600 bits 0:31) bits 0:11 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_0 */
    reg_value = mpmo_reg_CSI_CHG_INT_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_MSK) >> CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF;
    value[0] |= field_value;

    /* (0x00001604 bits 31:0) bits 32:63 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_1 */
    reg_value = mpmo_reg_CSI_CHG_INT_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_MSK) >> CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF;
    value[1] |= field_value;

    /* (0x00001608 bits 31:0) bits 64:95 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_2 */
    reg_value = mpmo_reg_CSI_CHG_INT_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_MSK) >> CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_CSI_CHG_I_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_CSI_CHG_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_CSI_CHG_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_CSI_CHG_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_CSI_CHG_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_CSI_CHG_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001600 bits 0:31) bits 0:11 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001600 bits 0:31) bits 0:11 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_0 */
        mpmo_reg_CSI_CHG_INT_REG_0_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                              CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001604 bits 31:0) bits 32:63 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_1 */
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
        /* (0x00001604 bits 31:0) bits 32:63 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_1 */
        mpmo_reg_CSI_CHG_INT_REG_1_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                              CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001608 bits 31:0) bits 64:95 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_2 */
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
        /* (0x00001608 bits 31:0) bits 64:95 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_2 */
        mpmo_reg_CSI_CHG_INT_REG_2_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                              CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_CSI_CHG_I_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_CSI_CHG_I_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_CSI_CHG_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_CSI_CHG_I_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001600 bits 0:31) bits 0:11 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_0 */
        reg_value = mpmo_reg_CSI_CHG_INT_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_MSK)
                      >> CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_MSK, CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF, field_value );
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
    /* (0x00001604 bits 31:0) bits 32:63 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_1 */
        reg_value = mpmo_reg_CSI_CHG_INT_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_MSK)
                      >> CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_MSK, CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF, field_value );
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
    /* (0x00001608 bits 31:0) bits 64:95 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_2 */
        reg_value = mpmo_reg_CSI_CHG_INT_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_MSK)
                      >> CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_MSK, CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_CSI_CHG_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_CSI_CHG_I_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_CSI_CHG_I_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_CSI_CHG_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_CSI_CHG_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_CSI_CHG_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001600 bits 0:31) bits 0:11 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001600 bits 0:31) bits 0:11 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_0 */
        return mpmo_reg_CSI_CHG_INT_REG_0_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                value << (CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001604 bits 31:0) bits 32:63 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_1 */
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
        /* (0x00001604 bits 31:0) bits 32:63 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_1 */
        return mpmo_reg_CSI_CHG_INT_REG_1_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                value << (CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001608 bits 31:0) bits 64:95 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_2 */
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
        /* (0x00001608 bits 31:0) bits 64:95 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_2 */
        return mpmo_reg_CSI_CHG_INT_REG_2_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                value << (CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_CSI_CHG_I_poll( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_CSI_CHG_I_poll( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpmo_lfield_CSI_CHG_I_poll", value[2] , value[1] , value[0] );

    /* (0x00001600 bits 0:31) bits 0:11 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_0 */
    return mpmo_reg_CSI_CHG_INT_REG_0_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_MSK,
                                            (value[0]<<CORE_REG_CSI_CHG_INT_REG_0_BIT_CSI_CHG_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);

    /* (0x00001604 bits 31:0) bits 32:63 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_1 */
    return mpmo_reg_CSI_CHG_INT_REG_1_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_MSK,
                                            (value[1]<<CORE_REG_CSI_CHG_INT_REG_1_BIT_CSI_CHG_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);

    /* (0x00001608 bits 31:0) bits 64:95 use field CSI_CHG_I of register PMC_CORE_REG_CSI_CHG_INT_REG_2 */
    return mpmo_reg_CSI_CHG_INT_REG_2_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_MSK,
                                            (value[2]<<CORE_REG_CSI_CHG_INT_REG_2_BIT_CSI_CHG_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void mpmo_lfield_SSF_GEN_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_GEN_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_GEN_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x0000160c bits 0:31) bits 0:11 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_0 */
    mpmo_reg_SSF_GEN_INT_REG_0_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_MSK,
                                                          CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF,
                                                          value[0]);

    /* (0x00001610 bits 31:0) bits 32:63 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_1 */
    mpmo_reg_SSF_GEN_INT_REG_1_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_MSK,
                                                          CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF,
                                                          value[1]);

    /* (0x00001614 bits 31:0) bits 64:95 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_2 */
    mpmo_reg_SSF_GEN_INT_REG_2_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_MSK,
                                                          CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF,
                                                          value[2]);
}

static INLINE void mpmo_lfield_SSF_GEN_I_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_GEN_I_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000160c bits 0:31) bits 0:11 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_0 */
    reg_value = mpmo_reg_SSF_GEN_INT_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_MSK) >> CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF;
    value[0] |= field_value;

    /* (0x00001610 bits 31:0) bits 32:63 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_1 */
    reg_value = mpmo_reg_SSF_GEN_INT_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_MSK) >> CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF;
    value[1] |= field_value;

    /* (0x00001614 bits 31:0) bits 64:95 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_2 */
    reg_value = mpmo_reg_SSF_GEN_INT_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_MSK) >> CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_GEN_I_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_SSF_GEN_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_SSF_GEN_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_GEN_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_GEN_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_GEN_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000160c bits 0:31) bits 0:11 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000160c bits 0:31) bits 0:11 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_0 */
        mpmo_reg_SSF_GEN_INT_REG_0_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                              CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001610 bits 31:0) bits 32:63 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_1 */
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
        /* (0x00001610 bits 31:0) bits 32:63 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_1 */
        mpmo_reg_SSF_GEN_INT_REG_1_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                              CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001614 bits 31:0) bits 64:95 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_2 */
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
        /* (0x00001614 bits 31:0) bits 64:95 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_2 */
        mpmo_reg_SSF_GEN_INT_REG_2_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                              CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_SSF_GEN_I_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_SSF_GEN_I_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_GEN_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_GEN_I_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000160c bits 0:31) bits 0:11 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_0 */
        reg_value = mpmo_reg_SSF_GEN_INT_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_MSK)
                      >> CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_MSK, CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF, field_value );
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
    /* (0x00001610 bits 31:0) bits 32:63 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_1 */
        reg_value = mpmo_reg_SSF_GEN_INT_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_MSK)
                      >> CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_MSK, CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF, field_value );
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
    /* (0x00001614 bits 31:0) bits 64:95 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_2 */
        reg_value = mpmo_reg_SSF_GEN_INT_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_MSK)
                      >> CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_MSK, CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_GEN_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_GEN_I_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_GEN_I_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_GEN_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_GEN_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_GEN_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000160c bits 0:31) bits 0:11 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000160c bits 0:31) bits 0:11 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_0 */
        return mpmo_reg_SSF_GEN_INT_REG_0_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                value << (CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001610 bits 31:0) bits 32:63 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_1 */
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
        /* (0x00001610 bits 31:0) bits 32:63 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_1 */
        return mpmo_reg_SSF_GEN_INT_REG_1_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                value << (CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001614 bits 31:0) bits 64:95 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_2 */
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
        /* (0x00001614 bits 31:0) bits 64:95 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_2 */
        return mpmo_reg_SSF_GEN_INT_REG_2_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                value << (CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_GEN_I_poll( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_GEN_I_poll( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_GEN_I_poll", value[2] , value[1] , value[0] );

    /* (0x0000160c bits 0:31) bits 0:11 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_0 */
    return mpmo_reg_SSF_GEN_INT_REG_0_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_MSK,
                                            (value[0]<<CORE_REG_SSF_GEN_INT_REG_0_BIT_SSF_GEN_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);

    /* (0x00001610 bits 31:0) bits 32:63 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_1 */
    return mpmo_reg_SSF_GEN_INT_REG_1_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_MSK,
                                            (value[1]<<CORE_REG_SSF_GEN_INT_REG_1_BIT_SSF_GEN_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);

    /* (0x00001614 bits 31:0) bits 64:95 use field SSF_GEN_I of register PMC_CORE_REG_SSF_GEN_INT_REG_2 */
    return mpmo_reg_SSF_GEN_INT_REG_2_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_MSK,
                                            (value[2]<<CORE_REG_SSF_GEN_INT_REG_2_BIT_SSF_GEN_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void mpmo_lfield_PKT_DROP_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value[6] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_PKT_DROP_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value[6] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "mpmo_lfield_PKT_DROP_I_set_to_clear", value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

    /* (0x00001618 bits 0:31) bits 0:23 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_0 */
    mpmo_reg_PKT_DROP_INT_REG_0_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_MSK,
                                                           CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF,
                                                           value[0]);

    /* (0x0000161c bits 31:0) bits 32:63 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_1 */
    mpmo_reg_PKT_DROP_INT_REG_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_MSK,
                                                           CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF,
                                                           value[1]);

    /* (0x00001620 bits 31:0) bits 64:95 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_2 */
    mpmo_reg_PKT_DROP_INT_REG_2_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_MSK,
                                                           CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF,
                                                           value[2]);

    /* (0x00001624 bits 31:0) bits 96:127 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_3 */
    mpmo_reg_PKT_DROP_INT_REG_3_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_MSK,
                                                           CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF,
                                                           value[3]);

    /* (0x00001628 bits 31:0) bits 128:159 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_4 */
    mpmo_reg_PKT_DROP_INT_REG_4_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_MSK,
                                                           CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF,
                                                           value[4]);

    /* (0x0000162c bits 31:0) bits 160:191 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_5 */
    mpmo_reg_PKT_DROP_INT_REG_5_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_MSK,
                                                           CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF,
                                                           value[5]);
}

static INLINE void mpmo_lfield_PKT_DROP_I_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[6] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_PKT_DROP_I_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[6] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<6;i++)
        value[i] = 0;

    /* (0x00001618 bits 0:31) bits 0:23 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_0 */
    reg_value = mpmo_reg_PKT_DROP_INT_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_MSK) >> CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF;
    value[0] |= field_value;

    /* (0x0000161c bits 31:0) bits 32:63 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_1 */
    reg_value = mpmo_reg_PKT_DROP_INT_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_MSK) >> CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF;
    value[1] |= field_value;

    /* (0x00001620 bits 31:0) bits 64:95 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_2 */
    reg_value = mpmo_reg_PKT_DROP_INT_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_MSK) >> CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF;
    value[2] |= field_value;

    /* (0x00001624 bits 31:0) bits 96:127 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_3 */
    reg_value = mpmo_reg_PKT_DROP_INT_REG_3_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_MSK) >> CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF;
    value[3] |= field_value;

    /* (0x00001628 bits 31:0) bits 128:159 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_4 */
    reg_value = mpmo_reg_PKT_DROP_INT_REG_4_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_MSK) >> CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF;
    value[4] |= field_value;

    /* (0x0000162c bits 31:0) bits 160:191 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_5 */
    reg_value = mpmo_reg_PKT_DROP_INT_REG_5_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_MSK) >> CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF;
    value[5] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "mpmo_lfield_PKT_DROP_I_get", value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_PKT_DROP_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_PKT_DROP_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_PKT_DROP_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 191) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_PKT_DROP_I_set_to_clear", stop_bit, 191 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_PKT_DROP_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001618 bits 0:31) bits 0:23 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_0 */
    if (start_bit <= 23) {
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
        /* (0x00001618 bits 0:31) bits 0:23 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_0 */
        mpmo_reg_PKT_DROP_INT_REG_0_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                               CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000161c bits 31:0) bits 32:63 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_1 */
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
        /* (0x0000161c bits 31:0) bits 32:63 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_1 */
        mpmo_reg_PKT_DROP_INT_REG_1_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                               CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001620 bits 31:0) bits 64:95 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_2 */
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
        /* (0x00001620 bits 31:0) bits 64:95 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_2 */
        mpmo_reg_PKT_DROP_INT_REG_2_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                               CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001624 bits 31:0) bits 96:127 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_3 */
    if ((start_bit <= 127) && (stop_bit >= 96)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 96) {
            subfield_offset = start_bit - 96;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 96 - start_bit;
            subfield_start_bit = 96;
        }
        if (stop_bit < 127) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 127;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001624 bits 31:0) bits 96:127 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_3 */
        mpmo_reg_PKT_DROP_INT_REG_3_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                               CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001628 bits 31:0) bits 128:159 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_4 */
    if ((start_bit <= 159) && (stop_bit >= 128)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 128) {
            subfield_offset = start_bit - 128;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 128 - start_bit;
            subfield_start_bit = 128;
        }
        if (stop_bit < 159) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 159;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001628 bits 31:0) bits 128:159 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_4 */
        mpmo_reg_PKT_DROP_INT_REG_4_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                               CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000162c bits 31:0) bits 160:191 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_5 */
    if ((start_bit <= 191) && (stop_bit >= 160)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 160) {
            subfield_offset = start_bit - 160;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 160 - start_bit;
            subfield_start_bit = 160;
        }
        if (stop_bit < 191) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 191;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000162c bits 31:0) bits 160:191 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_5 */
        mpmo_reg_PKT_DROP_INT_REG_5_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                               CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_PKT_DROP_I_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_PKT_DROP_I_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_PKT_DROP_I_get", stop_bit, start_bit );
    if (stop_bit > 191) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_PKT_DROP_I_get", stop_bit, 191 );
    if (start_bit <= 23) {
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
    /* (0x00001618 bits 0:31) bits 0:23 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_0 */
        reg_value = mpmo_reg_PKT_DROP_INT_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_MSK)
                      >> CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_MSK, CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF, field_value );
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
    /* (0x0000161c bits 31:0) bits 32:63 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_1 */
        reg_value = mpmo_reg_PKT_DROP_INT_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_MSK)
                      >> CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_MSK, CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF, field_value );
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
    /* (0x00001620 bits 31:0) bits 64:95 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_2 */
        reg_value = mpmo_reg_PKT_DROP_INT_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_MSK)
                      >> CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_MSK, CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 127) && (stop_bit >= 96)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 96) {
            subfield_offset = start_bit - 96;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 96 - start_bit;
            subfield_start_bit = 96;
        }
        if (stop_bit < 127) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 127;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001624 bits 31:0) bits 96:127 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_3 */
        reg_value = mpmo_reg_PKT_DROP_INT_REG_3_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_MSK)
                      >> CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_MSK, CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 159) && (stop_bit >= 128)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 128) {
            subfield_offset = start_bit - 128;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 128 - start_bit;
            subfield_start_bit = 128;
        }
        if (stop_bit < 159) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 159;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001628 bits 31:0) bits 128:159 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_4 */
        reg_value = mpmo_reg_PKT_DROP_INT_REG_4_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_MSK)
                      >> CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_MSK, CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 191) && (stop_bit >= 160)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 160) {
            subfield_offset = start_bit - 160;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 160 - start_bit;
            subfield_start_bit = 160;
        }
        if (stop_bit < 191) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 191;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000162c bits 31:0) bits 160:191 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_5 */
        reg_value = mpmo_reg_PKT_DROP_INT_REG_5_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_MSK)
                      >> CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_MSK, CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_PKT_DROP_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_PKT_DROP_I_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_PKT_DROP_I_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_PKT_DROP_I_poll", stop_bit, start_bit );
    if (stop_bit > 191) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_PKT_DROP_I_poll", stop_bit, 191 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_PKT_DROP_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001618 bits 0:31) bits 0:23 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_0 */
    if (start_bit <= 23) {
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
        if (stop_bit < 23) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 23;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001618 bits 0:31) bits 0:23 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_0 */
        return mpmo_reg_PKT_DROP_INT_REG_0_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000161c bits 31:0) bits 32:63 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_1 */
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
        /* (0x0000161c bits 31:0) bits 32:63 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_1 */
        return mpmo_reg_PKT_DROP_INT_REG_1_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001620 bits 31:0) bits 64:95 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_2 */
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
        /* (0x00001620 bits 31:0) bits 64:95 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_2 */
        return mpmo_reg_PKT_DROP_INT_REG_2_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001624 bits 31:0) bits 96:127 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_3 */
    if ((start_bit <= 127) && (stop_bit >= 96)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 96) {
            subfield_offset = start_bit - 96;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 96;
        }
        if (stop_bit < 127) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 127;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001624 bits 31:0) bits 96:127 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_3 */
        return mpmo_reg_PKT_DROP_INT_REG_3_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001628 bits 31:0) bits 128:159 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_4 */
    if ((start_bit <= 159) && (stop_bit >= 128)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 128) {
            subfield_offset = start_bit - 128;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 128;
        }
        if (stop_bit < 159) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 159;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001628 bits 31:0) bits 128:159 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_4 */
        return mpmo_reg_PKT_DROP_INT_REG_4_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000162c bits 31:0) bits 160:191 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_5 */
    if ((start_bit <= 191) && (stop_bit >= 160)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 160) {
            subfield_offset = start_bit - 160;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 160;
        }
        if (stop_bit < 191) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 191;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000162c bits 31:0) bits 160:191 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_5 */
        return mpmo_reg_PKT_DROP_INT_REG_5_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_PKT_DROP_I_poll( mpmo_buffer_t *b_ptr,
                                                                mpmo_handle_t *h_ptr,
                                                                UINT32 value[6],
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_PKT_DROP_I_poll( mpmo_buffer_t *b_ptr,
                                                                mpmo_handle_t *h_ptr,
                                                                UINT32 value[6],
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "mpmo_lfield_PKT_DROP_I_poll", value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

    /* (0x00001618 bits 0:31) bits 0:23 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_0 */
    return mpmo_reg_PKT_DROP_INT_REG_0_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_MSK,
                                             (value[0]<<CORE_REG_PKT_DROP_INT_REG_0_BIT_PKT_DROP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);

    /* (0x0000161c bits 31:0) bits 32:63 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_1 */
    return mpmo_reg_PKT_DROP_INT_REG_1_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_MSK,
                                             (value[1]<<CORE_REG_PKT_DROP_INT_REG_1_BIT_PKT_DROP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);

    /* (0x00001620 bits 31:0) bits 64:95 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_2 */
    return mpmo_reg_PKT_DROP_INT_REG_2_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_MSK,
                                             (value[2]<<CORE_REG_PKT_DROP_INT_REG_2_BIT_PKT_DROP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);

    /* (0x00001624 bits 31:0) bits 96:127 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_3 */
    return mpmo_reg_PKT_DROP_INT_REG_3_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_MSK,
                                             (value[3]<<CORE_REG_PKT_DROP_INT_REG_3_BIT_PKT_DROP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);

    /* (0x00001628 bits 31:0) bits 128:159 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_4 */
    return mpmo_reg_PKT_DROP_INT_REG_4_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_MSK,
                                             (value[4]<<CORE_REG_PKT_DROP_INT_REG_4_BIT_PKT_DROP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);

    /* (0x0000162c bits 31:0) bits 160:191 use field PKT_DROP_I of register PMC_CORE_REG_PKT_DROP_INT_REG_5 */
    return mpmo_reg_PKT_DROP_INT_REG_5_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_MSK,
                                             (value[5]<<CORE_REG_PKT_DROP_INT_REG_5_BIT_PKT_DROP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void mpmo_lfield_EOP_ERR_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_EOP_ERR_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_EOP_ERR_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x00001630 bits 0:31) bits 0:11 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_0 */
    mpmo_reg_EOP_ERR_INT_REG_0_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_MSK,
                                                          CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF,
                                                          value[0]);

    /* (0x00001634 bits 31:0) bits 32:63 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_1 */
    mpmo_reg_EOP_ERR_INT_REG_1_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_MSK,
                                                          CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF,
                                                          value[1]);

    /* (0x00001638 bits 31:0) bits 64:95 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_2 */
    mpmo_reg_EOP_ERR_INT_REG_2_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_MSK,
                                                          CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF,
                                                          value[2]);
}

static INLINE void mpmo_lfield_EOP_ERR_I_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_EOP_ERR_I_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001630 bits 0:31) bits 0:11 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_0 */
    reg_value = mpmo_reg_EOP_ERR_INT_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_MSK) >> CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF;
    value[0] |= field_value;

    /* (0x00001634 bits 31:0) bits 32:63 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_1 */
    reg_value = mpmo_reg_EOP_ERR_INT_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_MSK) >> CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF;
    value[1] |= field_value;

    /* (0x00001638 bits 31:0) bits 64:95 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_2 */
    reg_value = mpmo_reg_EOP_ERR_INT_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_MSK) >> CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_EOP_ERR_I_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_EOP_ERR_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_EOP_ERR_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_EOP_ERR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_EOP_ERR_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_EOP_ERR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001630 bits 0:31) bits 0:11 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001630 bits 0:31) bits 0:11 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_0 */
        mpmo_reg_EOP_ERR_INT_REG_0_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                              CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001634 bits 31:0) bits 32:63 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_1 */
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
        /* (0x00001634 bits 31:0) bits 32:63 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_1 */
        mpmo_reg_EOP_ERR_INT_REG_1_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                              CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001638 bits 31:0) bits 64:95 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_2 */
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
        /* (0x00001638 bits 31:0) bits 64:95 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_2 */
        mpmo_reg_EOP_ERR_INT_REG_2_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                              CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_EOP_ERR_I_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_EOP_ERR_I_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_EOP_ERR_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_EOP_ERR_I_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001630 bits 0:31) bits 0:11 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_0 */
        reg_value = mpmo_reg_EOP_ERR_INT_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_MSK)
                      >> CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_MSK, CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF, field_value );
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
    /* (0x00001634 bits 31:0) bits 32:63 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_1 */
        reg_value = mpmo_reg_EOP_ERR_INT_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_MSK)
                      >> CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_MSK, CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF, field_value );
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
    /* (0x00001638 bits 31:0) bits 64:95 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_2 */
        reg_value = mpmo_reg_EOP_ERR_INT_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_MSK)
                      >> CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_MSK, CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_EOP_ERR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_EOP_ERR_I_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_EOP_ERR_I_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_EOP_ERR_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_EOP_ERR_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_EOP_ERR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001630 bits 0:31) bits 0:11 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001630 bits 0:31) bits 0:11 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_0 */
        return mpmo_reg_EOP_ERR_INT_REG_0_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                value << (CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001634 bits 31:0) bits 32:63 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_1 */
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
        /* (0x00001634 bits 31:0) bits 32:63 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_1 */
        return mpmo_reg_EOP_ERR_INT_REG_1_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                value << (CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001638 bits 31:0) bits 64:95 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_2 */
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
        /* (0x00001638 bits 31:0) bits 64:95 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_2 */
        return mpmo_reg_EOP_ERR_INT_REG_2_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                value << (CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_EOP_ERR_I_poll( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_EOP_ERR_I_poll( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpmo_lfield_EOP_ERR_I_poll", value[2] , value[1] , value[0] );

    /* (0x00001630 bits 0:31) bits 0:11 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_0 */
    return mpmo_reg_EOP_ERR_INT_REG_0_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_MSK,
                                            (value[0]<<CORE_REG_EOP_ERR_INT_REG_0_BIT_EOP_ERR_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);

    /* (0x00001634 bits 31:0) bits 32:63 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_1 */
    return mpmo_reg_EOP_ERR_INT_REG_1_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_MSK,
                                            (value[1]<<CORE_REG_EOP_ERR_INT_REG_1_BIT_EOP_ERR_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);

    /* (0x00001638 bits 31:0) bits 64:95 use field EOP_ERR_I of register PMC_CORE_REG_EOP_ERR_INT_REG_2 */
    return mpmo_reg_EOP_ERR_INT_REG_2_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_MSK,
                                            (value[2]<<CORE_REG_EOP_ERR_INT_REG_2_BIT_EOP_ERR_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void mpmo_lfield_SOP_ERR_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SOP_ERR_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SOP_ERR_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x0000163c bits 0:31) bits 0:11 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_0 */
    mpmo_reg_SOP_ERR_INT_REG_0_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_MSK,
                                                          CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF,
                                                          value[0]);

    /* (0x00001640 bits 31:0) bits 32:63 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_1 */
    mpmo_reg_SOP_ERR_INT_REG_1_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_MSK,
                                                          CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF,
                                                          value[1]);

    /* (0x00001644 bits 31:0) bits 64:95 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_2 */
    mpmo_reg_SOP_ERR_INT_REG_2_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_MSK,
                                                          CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF,
                                                          value[2]);
}

static INLINE void mpmo_lfield_SOP_ERR_I_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SOP_ERR_I_get( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000163c bits 0:31) bits 0:11 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_0 */
    reg_value = mpmo_reg_SOP_ERR_INT_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_MSK) >> CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF;
    value[0] |= field_value;

    /* (0x00001640 bits 31:0) bits 32:63 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_1 */
    reg_value = mpmo_reg_SOP_ERR_INT_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_MSK) >> CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF;
    value[1] |= field_value;

    /* (0x00001644 bits 31:0) bits 64:95 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_2 */
    reg_value = mpmo_reg_SOP_ERR_INT_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_MSK) >> CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SOP_ERR_I_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_SOP_ERR_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_SOP_ERR_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SOP_ERR_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SOP_ERR_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SOP_ERR_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000163c bits 0:31) bits 0:11 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000163c bits 0:31) bits 0:11 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_0 */
        mpmo_reg_SOP_ERR_INT_REG_0_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                              CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001640 bits 31:0) bits 32:63 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_1 */
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
        /* (0x00001640 bits 31:0) bits 32:63 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_1 */
        mpmo_reg_SOP_ERR_INT_REG_1_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                              CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001644 bits 31:0) bits 64:95 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_2 */
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
        /* (0x00001644 bits 31:0) bits 64:95 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_2 */
        mpmo_reg_SOP_ERR_INT_REG_2_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                              CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_SOP_ERR_I_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_SOP_ERR_I_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SOP_ERR_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SOP_ERR_I_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000163c bits 0:31) bits 0:11 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_0 */
        reg_value = mpmo_reg_SOP_ERR_INT_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_MSK)
                      >> CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_MSK, CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF, field_value );
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
    /* (0x00001640 bits 31:0) bits 32:63 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_1 */
        reg_value = mpmo_reg_SOP_ERR_INT_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_MSK)
                      >> CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_MSK, CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF, field_value );
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
    /* (0x00001644 bits 31:0) bits 64:95 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_2 */
        reg_value = mpmo_reg_SOP_ERR_INT_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_MSK)
                      >> CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_MSK, CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SOP_ERR_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_SOP_ERR_I_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_SOP_ERR_I_poll( mpmo_buffer_t *b_ptr,
                                                                     mpmo_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SOP_ERR_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SOP_ERR_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SOP_ERR_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000163c bits 0:31) bits 0:11 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000163c bits 0:31) bits 0:11 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_0 */
        return mpmo_reg_SOP_ERR_INT_REG_0_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                value << (CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001640 bits 31:0) bits 32:63 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_1 */
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
        /* (0x00001640 bits 31:0) bits 32:63 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_1 */
        return mpmo_reg_SOP_ERR_INT_REG_1_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                value << (CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001644 bits 31:0) bits 64:95 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_2 */
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
        /* (0x00001644 bits 31:0) bits 64:95 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_2 */
        return mpmo_reg_SOP_ERR_INT_REG_2_poll( b_ptr,
                                                h_ptr,
                                                subfield_mask << (CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                value << (CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF + subfield_offset),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_SOP_ERR_I_poll( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_SOP_ERR_I_poll( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpmo_lfield_SOP_ERR_I_poll", value[2] , value[1] , value[0] );

    /* (0x0000163c bits 0:31) bits 0:11 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_0 */
    return mpmo_reg_SOP_ERR_INT_REG_0_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_MSK,
                                            (value[0]<<CORE_REG_SOP_ERR_INT_REG_0_BIT_SOP_ERR_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);

    /* (0x00001640 bits 31:0) bits 32:63 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_1 */
    return mpmo_reg_SOP_ERR_INT_REG_1_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_MSK,
                                            (value[1]<<CORE_REG_SOP_ERR_INT_REG_1_BIT_SOP_ERR_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);

    /* (0x00001644 bits 31:0) bits 64:95 use field SOP_ERR_I of register PMC_CORE_REG_SOP_ERR_INT_REG_2 */
    return mpmo_reg_SOP_ERR_INT_REG_2_poll( b_ptr,
                                            h_ptr,
                                            CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_MSK,
                                            (value[2]<<CORE_REG_SOP_ERR_INT_REG_2_BIT_SOP_ERR_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void mpmo_lfield_BWR_RAMP_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_BWR_RAMP_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_BWR_RAMP_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x00001650 bits 0:31) bits 0:11 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_0 */
    mpmo_reg_BWR_RAMP_INT_REG_0_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_MSK,
                                                           CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF,
                                                           value[0]);

    /* (0x00001654 bits 31:0) bits 32:63 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_1 */
    mpmo_reg_BWR_RAMP_INT_REG_1_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_MSK,
                                                           CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF,
                                                           value[1]);

    /* (0x00001658 bits 31:0) bits 64:95 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_2 */
    mpmo_reg_BWR_RAMP_INT_REG_2_action_on_write_field_set( b_ptr,
                                                           h_ptr,
                                                           CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_MSK,
                                                           CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF,
                                                           value[2]);
}

static INLINE void mpmo_lfield_BWR_RAMP_I_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_BWR_RAMP_I_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001650 bits 0:31) bits 0:11 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_0 */
    reg_value = mpmo_reg_BWR_RAMP_INT_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_MSK) >> CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF;
    value[0] |= field_value;

    /* (0x00001654 bits 31:0) bits 32:63 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_1 */
    reg_value = mpmo_reg_BWR_RAMP_INT_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_MSK) >> CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF;
    value[1] |= field_value;

    /* (0x00001658 bits 31:0) bits 64:95 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_2 */
    reg_value = mpmo_reg_BWR_RAMP_INT_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_MSK) >> CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_BWR_RAMP_I_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_BWR_RAMP_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_BWR_RAMP_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                              mpmo_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_BWR_RAMP_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_BWR_RAMP_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_BWR_RAMP_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001650 bits 0:31) bits 0:11 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001650 bits 0:31) bits 0:11 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_0 */
        mpmo_reg_BWR_RAMP_INT_REG_0_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                               CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001654 bits 31:0) bits 32:63 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_1 */
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
        /* (0x00001654 bits 31:0) bits 32:63 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_1 */
        mpmo_reg_BWR_RAMP_INT_REG_1_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                               CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001658 bits 31:0) bits 64:95 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_2 */
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
        /* (0x00001658 bits 31:0) bits 64:95 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_2 */
        mpmo_reg_BWR_RAMP_INT_REG_2_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               subfield_mask << (CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                               CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF + subfield_offset,
                                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_BWR_RAMP_I_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_BWR_RAMP_I_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_BWR_RAMP_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_BWR_RAMP_I_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001650 bits 0:31) bits 0:11 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_0 */
        reg_value = mpmo_reg_BWR_RAMP_INT_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_MSK)
                      >> CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_MSK, CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF, field_value );
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
    /* (0x00001654 bits 31:0) bits 32:63 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_1 */
        reg_value = mpmo_reg_BWR_RAMP_INT_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_MSK)
                      >> CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_MSK, CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF, field_value );
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
    /* (0x00001658 bits 31:0) bits 64:95 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_2 */
        reg_value = mpmo_reg_BWR_RAMP_INT_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_MSK)
                      >> CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_MSK, CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_BWR_RAMP_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_BWR_RAMP_I_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_BWR_RAMP_I_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_BWR_RAMP_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_BWR_RAMP_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_BWR_RAMP_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001650 bits 0:31) bits 0:11 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001650 bits 0:31) bits 0:11 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_0 */
        return mpmo_reg_BWR_RAMP_INT_REG_0_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001654 bits 31:0) bits 32:63 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_1 */
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
        /* (0x00001654 bits 31:0) bits 32:63 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_1 */
        return mpmo_reg_BWR_RAMP_INT_REG_1_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001658 bits 31:0) bits 64:95 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_2 */
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
        /* (0x00001658 bits 31:0) bits 64:95 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_2 */
        return mpmo_reg_BWR_RAMP_INT_REG_2_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                 value << (CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_BWR_RAMP_I_poll( mpmo_buffer_t *b_ptr,
                                                                mpmo_handle_t *h_ptr,
                                                                UINT32 value[3],
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_BWR_RAMP_I_poll( mpmo_buffer_t *b_ptr,
                                                                mpmo_handle_t *h_ptr,
                                                                UINT32 value[3],
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpmo_lfield_BWR_RAMP_I_poll", value[2] , value[1] , value[0] );

    /* (0x00001650 bits 0:31) bits 0:11 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_0 */
    return mpmo_reg_BWR_RAMP_INT_REG_0_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_MSK,
                                             (value[0]<<CORE_REG_BWR_RAMP_INT_REG_0_BIT_BWR_RAMP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);

    /* (0x00001654 bits 31:0) bits 32:63 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_1 */
    return mpmo_reg_BWR_RAMP_INT_REG_1_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_MSK,
                                             (value[1]<<CORE_REG_BWR_RAMP_INT_REG_1_BIT_BWR_RAMP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);

    /* (0x00001658 bits 31:0) bits 64:95 use field BWR_RAMP_I of register PMC_CORE_REG_BWR_RAMP_INT_REG_2 */
    return mpmo_reg_BWR_RAMP_INT_REG_2_poll( b_ptr,
                                             h_ptr,
                                             CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_MSK,
                                             (value[2]<<CORE_REG_BWR_RAMP_INT_REG_2_BIT_BWR_RAMP_I_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void mpmo_lfield_SSF_STATE_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_STATE_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_STATE_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x00001730 bits 31:0) bits 0:31 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_0 */
    mpmo_reg_SSF_STATE_INT_REG_0_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_MSK,
                                                            CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF,
                                                            value[0]);

    /* (0x00001734 bits 31:0) bits 32:63 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_1 */
    mpmo_reg_SSF_STATE_INT_REG_1_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_MSK,
                                                            CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF,
                                                            value[1]);

    /* (0x00001738 bits 0:31) bits 64:79 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_2 */
    mpmo_reg_SSF_STATE_INT_REG_2_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_MSK,
                                                            CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF,
                                                            value[2]);
}

static INLINE void mpmo_lfield_SSF_STATE_I_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_STATE_I_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001730 bits 31:0) bits 0:31 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_0 */
    reg_value = mpmo_reg_SSF_STATE_INT_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_MSK) >> CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF;
    value[0] |= field_value;

    /* (0x00001734 bits 31:0) bits 32:63 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_1 */
    reg_value = mpmo_reg_SSF_STATE_INT_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_MSK) >> CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF;
    value[1] |= field_value;

    /* (0x00001738 bits 0:31) bits 64:79 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_2 */
    reg_value = mpmo_reg_SSF_STATE_INT_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_MSK) >> CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_STATE_I_get", value[2] , value[1] , value[0] );

}
static INLINE void mpmo_lfield_range_SSF_STATE_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_range_SSF_STATE_I_set_to_clear( mpmo_buffer_t *b_ptr,
                                                               mpmo_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_STATE_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_STATE_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_STATE_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001730 bits 31:0) bits 0:31 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_0 */
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
        /* (0x00001730 bits 31:0) bits 0:31 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_0 */
        mpmo_reg_SSF_STATE_INT_REG_0_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                                CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001734 bits 31:0) bits 32:63 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_1 */
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
        /* (0x00001734 bits 31:0) bits 32:63 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_1 */
        mpmo_reg_SSF_STATE_INT_REG_1_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                                CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001738 bits 0:31) bits 64:79 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_2 */
    if ((start_bit <= 79) && (stop_bit >= 64)) {
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
        if (stop_bit < 79) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 79;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001738 bits 0:31) bits 64:79 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_2 */
        mpmo_reg_SSF_STATE_INT_REG_2_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                                CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_lfield_range_SSF_STATE_I_get( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_SSF_STATE_I_get( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_STATE_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_STATE_I_get", stop_bit, 95 );
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
    /* (0x00001730 bits 31:0) bits 0:31 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_0 */
        reg_value = mpmo_reg_SSF_STATE_INT_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_MSK)
                      >> CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_MSK, CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF, field_value );
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
    /* (0x00001734 bits 31:0) bits 32:63 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_1 */
        reg_value = mpmo_reg_SSF_STATE_INT_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_MSK)
                      >> CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_MSK, CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 79) && (stop_bit >= 64)) {
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
        if (stop_bit < 79) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 79;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001738 bits 0:31) bits 64:79 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_2 */
        reg_value = mpmo_reg_SSF_STATE_INT_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_MSK)
                      >> CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_MSK, CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_STATE_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_STATE_I_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_STATE_I_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_STATE_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_STATE_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_STATE_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001730 bits 31:0) bits 0:31 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_0 */
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
        /* (0x00001730 bits 31:0) bits 0:31 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_0 */
        return mpmo_reg_SSF_STATE_INT_REG_0_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                  value << (CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001734 bits 31:0) bits 32:63 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_1 */
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
        /* (0x00001734 bits 31:0) bits 32:63 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_1 */
        return mpmo_reg_SSF_STATE_INT_REG_1_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                  value << (CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001738 bits 0:31) bits 64:79 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_2 */
    if ((start_bit <= 79) && (stop_bit >= 64)) {
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
        if (stop_bit < 79) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 79;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001738 bits 0:31) bits 64:79 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_2 */
        return mpmo_reg_SSF_STATE_INT_REG_2_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                  value << (CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_STATE_I_poll( mpmo_buffer_t *b_ptr,
                                                                 mpmo_handle_t *h_ptr,
                                                                 UINT32 value[3],
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_STATE_I_poll( mpmo_buffer_t *b_ptr,
                                                                 mpmo_handle_t *h_ptr,
                                                                 UINT32 value[3],
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_STATE_I_poll", value[2] , value[1] , value[0] );

    /* (0x00001730 bits 31:0) bits 0:31 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_0 */
    return mpmo_reg_SSF_STATE_INT_REG_0_poll( b_ptr,
                                              h_ptr,
                                              CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_MSK,
                                              (value[0]<<CORE_REG_SSF_STATE_INT_REG_0_BIT_SSF_STATE_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);

    /* (0x00001734 bits 31:0) bits 32:63 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_1 */
    return mpmo_reg_SSF_STATE_INT_REG_1_poll( b_ptr,
                                              h_ptr,
                                              CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_MSK,
                                              (value[1]<<CORE_REG_SSF_STATE_INT_REG_1_BIT_SSF_STATE_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);

    /* (0x00001738 bits 0:31) bits 64:79 use field SSF_STATE_I of register PMC_CORE_REG_SSF_STATE_INT_REG_2 */
    return mpmo_reg_SSF_STATE_INT_REG_2_poll( b_ptr,
                                              h_ptr,
                                              CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_MSK,
                                              (value[2]<<CORE_REG_SSF_STATE_INT_REG_2_BIT_SSF_STATE_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE void mpmo_lfield_BWR_RAMP_V_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_BWR_RAMP_V_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001720 bits 0:31) bits 0:11 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_0 */
    reg_value = mpmo_reg_BWR_RAMP_INT_V_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_MSK) >> CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_OFF;
    value[0] |= field_value;

    /* (0x00001724 bits 31:0) bits 32:63 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_1 */
    reg_value = mpmo_reg_BWR_RAMP_INT_V_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_MSK) >> CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_OFF;
    value[1] |= field_value;

    /* (0x00001728 bits 31:0) bits 64:95 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_2 */
    reg_value = mpmo_reg_BWR_RAMP_INT_V_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_MSK) >> CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_BWR_RAMP_V_get", value[2] , value[1] , value[0] );

}
static INLINE UINT32 mpmo_lfield_range_BWR_RAMP_V_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_BWR_RAMP_V_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_BWR_RAMP_V_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_BWR_RAMP_V_get", stop_bit, 95 );
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001720 bits 0:31) bits 0:11 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_0 */
        reg_value = mpmo_reg_BWR_RAMP_INT_V_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_MSK)
                      >> CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_MSK, CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_OFF, field_value );
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
    /* (0x00001724 bits 31:0) bits 32:63 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_1 */
        reg_value = mpmo_reg_BWR_RAMP_INT_V_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_MSK)
                      >> CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_MSK, CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_OFF, field_value );
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
    /* (0x00001728 bits 31:0) bits 64:95 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_2 */
        reg_value = mpmo_reg_BWR_RAMP_INT_V_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_MSK)
                      >> CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_MSK, CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_BWR_RAMP_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_BWR_RAMP_V_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_BWR_RAMP_V_poll( mpmo_buffer_t *b_ptr,
                                                                      mpmo_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_BWR_RAMP_V_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_BWR_RAMP_V_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_BWR_RAMP_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001720 bits 0:31) bits 0:11 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_0 */
    if (start_bit <= 11) {
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
        if (stop_bit < 11) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 11;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001720 bits 0:31) bits 0:11 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_0 */
        return mpmo_reg_BWR_RAMP_INT_V_REG_0_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_OFF + subfield_offset),
                                                   value << (CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001724 bits 31:0) bits 32:63 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_1 */
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
        /* (0x00001724 bits 31:0) bits 32:63 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_1 */
        return mpmo_reg_BWR_RAMP_INT_V_REG_1_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_OFF + subfield_offset),
                                                   value << (CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001728 bits 31:0) bits 64:95 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_2 */
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
        /* (0x00001728 bits 31:0) bits 64:95 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_2 */
        return mpmo_reg_BWR_RAMP_INT_V_REG_2_poll( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_OFF + subfield_offset),
                                                   value << (CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_BWR_RAMP_V_poll( mpmo_buffer_t *b_ptr,
                                                                mpmo_handle_t *h_ptr,
                                                                UINT32 value[3],
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_BWR_RAMP_V_poll( mpmo_buffer_t *b_ptr,
                                                                mpmo_handle_t *h_ptr,
                                                                UINT32 value[3],
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpmo_lfield_BWR_RAMP_V_poll", value[2] , value[1] , value[0] );

    /* (0x00001720 bits 0:31) bits 0:11 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_0 */
    return mpmo_reg_BWR_RAMP_INT_V_REG_0_poll( b_ptr,
                                               h_ptr,
                                               CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_MSK,
                                               (value[0]<<CORE_REG_BWR_RAMP_INT_V_REG_0_BIT_BWR_RAMP_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);

    /* (0x00001724 bits 31:0) bits 32:63 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_1 */
    return mpmo_reg_BWR_RAMP_INT_V_REG_1_poll( b_ptr,
                                               h_ptr,
                                               CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_MSK,
                                               (value[1]<<CORE_REG_BWR_RAMP_INT_V_REG_1_BIT_BWR_RAMP_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);

    /* (0x00001728 bits 31:0) bits 64:95 use field BWR_RAMP_V of register PMC_CORE_REG_BWR_RAMP_INT_V_REG_2 */
    return mpmo_reg_BWR_RAMP_INT_V_REG_2_poll( b_ptr,
                                               h_ptr,
                                               CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_MSK,
                                               (value[2]<<CORE_REG_BWR_RAMP_INT_V_REG_2_BIT_BWR_RAMP_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE void mpmo_lfield_SSF_STATE_V_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void mpmo_lfield_SSF_STATE_V_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00001748 bits 31:0) bits 0:31 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_0 */
    reg_value = mpmo_reg_SSF_STATE_INT_V_REG_0_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_MSK) >> CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_OFF;
    value[0] |= field_value;

    /* (0x0000174c bits 31:0) bits 32:63 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_1 */
    reg_value = mpmo_reg_SSF_STATE_INT_V_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_MSK) >> CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_OFF;
    value[1] |= field_value;

    /* (0x00001750 bits 0:31) bits 64:79 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_2 */
    reg_value = mpmo_reg_SSF_STATE_INT_V_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_MSK) >> CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_STATE_V_get", value[2] , value[1] , value[0] );

}
static INLINE UINT32 mpmo_lfield_range_SSF_STATE_V_get( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_lfield_range_SSF_STATE_V_get( mpmo_buffer_t *b_ptr,
                                                        mpmo_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_STATE_V_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_STATE_V_get", stop_bit, 95 );
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
    /* (0x00001748 bits 31:0) bits 0:31 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_0 */
        reg_value = mpmo_reg_SSF_STATE_INT_V_REG_0_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_MSK)
                      >> CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_MSK, CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_OFF, field_value );
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
    /* (0x0000174c bits 31:0) bits 32:63 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_1 */
        reg_value = mpmo_reg_SSF_STATE_INT_V_REG_1_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_MSK)
                      >> CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_MSK, CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 79) && (stop_bit >= 64)) {
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
        if (stop_bit < 79) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 79;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00001750 bits 0:31) bits 64:79 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_2 */
        reg_value = mpmo_reg_SSF_STATE_INT_V_REG_2_read(  b_ptr, h_ptr);
        field_value = (reg_value & CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_MSK)
                      >> CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_MSK, CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_STATE_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_STATE_V_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_STATE_V_poll( mpmo_buffer_t *b_ptr,
                                                                       mpmo_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_lfield_range_SSF_STATE_V_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_lfield_range_SSF_STATE_V_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mpmo_lfield_range_SSF_STATE_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001748 bits 31:0) bits 0:31 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_0 */
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
        /* (0x00001748 bits 31:0) bits 0:31 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_0 */
        return mpmo_reg_SSF_STATE_INT_V_REG_0_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_OFF + subfield_offset),
                                                    value << (CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000174c bits 31:0) bits 32:63 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_1 */
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
        /* (0x0000174c bits 31:0) bits 32:63 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_1 */
        return mpmo_reg_SSF_STATE_INT_V_REG_1_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_OFF + subfield_offset),
                                                    value << (CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001750 bits 0:31) bits 64:79 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_2 */
    if ((start_bit <= 79) && (stop_bit >= 64)) {
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
        if (stop_bit < 79) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 79;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001750 bits 0:31) bits 64:79 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_2 */
        return mpmo_reg_SSF_STATE_INT_V_REG_2_poll( b_ptr,
                                                    h_ptr,
                                                    subfield_mask << (CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_OFF + subfield_offset),
                                                    value << (CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_OFF + subfield_offset),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_STATE_V_poll( mpmo_buffer_t *b_ptr,
                                                                 mpmo_handle_t *h_ptr,
                                                                 UINT32 value[3],
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_STATE_V_poll( mpmo_buffer_t *b_ptr,
                                                                 mpmo_handle_t *h_ptr,
                                                                 UINT32 value[3],
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "mpmo_lfield_SSF_STATE_V_poll", value[2] , value[1] , value[0] );

    /* (0x00001748 bits 31:0) bits 0:31 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_0 */
    return mpmo_reg_SSF_STATE_INT_V_REG_0_poll( b_ptr,
                                                h_ptr,
                                                CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_MSK,
                                                (value[0]<<CORE_REG_SSF_STATE_INT_V_REG_0_BIT_SSF_STATE_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);

    /* (0x0000174c bits 31:0) bits 32:63 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_1 */
    return mpmo_reg_SSF_STATE_INT_V_REG_1_poll( b_ptr,
                                                h_ptr,
                                                CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_MSK,
                                                (value[1]<<CORE_REG_SSF_STATE_INT_V_REG_1_BIT_SSF_STATE_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);

    /* (0x00001750 bits 0:31) bits 64:79 use field SSF_STATE_V of register PMC_CORE_REG_SSF_STATE_INT_V_REG_2 */
    return mpmo_reg_SSF_STATE_INT_V_REG_2_poll( b_ptr,
                                                h_ptr,
                                                CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_MSK,
                                                (value[2]<<CORE_REG_SSF_STATE_INT_V_REG_2_BIT_SSF_STATE_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void mpmo_field_AGE_IDEAL_set( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_AGE_IDEAL_set( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_AGE_IDEAL_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_AGE_IDEAL_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_AGE_IDEAL_set", N, value );

    /* ((0x00001000 + (N) * 0x10) bits 31:16) field AGE_IDEAL of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
    mpmo_reg_CFG_RAM_1_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_MSK,
                                        CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_AGE_IDEAL_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_AGE_IDEAL_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_AGE_IDEAL_get", N, 95);
    /* ((0x00001000 + (N) * 0x10) bits 31:16) field AGE_IDEAL of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_1_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_MSK) >> CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_AGE_IDEAL_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_AGE_IDEAL_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_AGE_IDEAL_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_AGE_IDEAL_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_IDEAL_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_IDEAL_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_IDEAL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x10) bits 31:16) field AGE_IDEAL of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
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
        /* ((0x00001000 + (N) * 0x10) bits 31:16) field AGE_IDEAL of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
        mpmo_reg_CFG_RAM_1_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_AGE_IDEAL_get( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_AGE_IDEAL_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_AGE_IDEAL_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_IDEAL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_IDEAL_get", stop_bit, 15 );
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
    /* ((0x00001000 + (N) * 0x10) bits 31:16) field AGE_IDEAL of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_1_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_MSK)
                  >> CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_MSK, CORE_REG_CFG_RAM_1_BIT_AGE_IDEAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_IDEAL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_AGE_OLD_set( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_AGE_OLD_set( mpmo_buffer_t *b_ptr,
                                           mpmo_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_AGE_OLD_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_AGE_OLD_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_AGE_OLD_set", N, value );

    /* ((0x00001000 + (N) * 0x10) bits 15:0) field AGE_OLD of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
    mpmo_reg_CFG_RAM_1_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_1_BIT_AGE_OLD_MSK,
                                        CORE_REG_CFG_RAM_1_BIT_AGE_OLD_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_AGE_OLD_get( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_AGE_OLD_get( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_AGE_OLD_get", N, 95);
    /* ((0x00001000 + (N) * 0x10) bits 15:0) field AGE_OLD of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_1_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_1_BIT_AGE_OLD_MSK) >> CORE_REG_CFG_RAM_1_BIT_AGE_OLD_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_AGE_OLD_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_AGE_OLD_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_AGE_OLD_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_AGE_OLD_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_OLD_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_OLD_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_OLD_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001000 + (N) * 0x10) bits 15:0) field AGE_OLD of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
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
        /* ((0x00001000 + (N) * 0x10) bits 15:0) field AGE_OLD of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
        mpmo_reg_CFG_RAM_1_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_1_BIT_AGE_OLD_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_1_BIT_AGE_OLD_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_AGE_OLD_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_AGE_OLD_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_AGE_OLD_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_OLD_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_OLD_get", stop_bit, 15 );
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
    /* ((0x00001000 + (N) * 0x10) bits 15:0) field AGE_OLD of register PMC_CORE_REG_CFG_RAM_1 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_1_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_1_BIT_AGE_OLD_MSK)
                  >> CORE_REG_CFG_RAM_1_BIT_AGE_OLD_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_1_BIT_AGE_OLD_MSK, CORE_REG_CFG_RAM_1_BIT_AGE_OLD_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_OLD_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_AGE_THRES_3_2_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_AGE_THRES_3_2_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_AGE_THRES_3_2_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_AGE_THRES_3_2_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_AGE_THRES_3_2_set", N, value );

    /* ((0x00001004 + (N) * 0x10) bits 31:16) field AGE_THRES_3_2 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
    mpmo_reg_CFG_RAM_2_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_MSK,
                                        CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_AGE_THRES_3_2_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_AGE_THRES_3_2_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_AGE_THRES_3_2_get", N, 95);
    /* ((0x00001004 + (N) * 0x10) bits 31:16) field AGE_THRES_3_2 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_2_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_MSK) >> CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_AGE_THRES_3_2_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_AGE_THRES_3_2_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_AGE_THRES_3_2_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_AGE_THRES_3_2_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_THRES_3_2_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_THRES_3_2_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_THRES_3_2_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001004 + (N) * 0x10) bits 31:16) field AGE_THRES_3_2 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
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
        /* ((0x00001004 + (N) * 0x10) bits 31:16) field AGE_THRES_3_2 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
        mpmo_reg_CFG_RAM_2_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_AGE_THRES_3_2_get( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_AGE_THRES_3_2_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_AGE_THRES_3_2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_THRES_3_2_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_THRES_3_2_get", stop_bit, 15 );
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
    /* ((0x00001004 + (N) * 0x10) bits 31:16) field AGE_THRES_3_2 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_2_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_MSK)
                  >> CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_MSK, CORE_REG_CFG_RAM_2_BIT_AGE_THRES_3_2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_THRES_3_2_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_AGE_THRES_1_0_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_AGE_THRES_1_0_set( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_AGE_THRES_1_0_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_AGE_THRES_1_0_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_AGE_THRES_1_0_set", N, value );

    /* ((0x00001004 + (N) * 0x10) bits 15:0) field AGE_THRES_1_0 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
    mpmo_reg_CFG_RAM_2_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_MSK,
                                        CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_AGE_THRES_1_0_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_AGE_THRES_1_0_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_AGE_THRES_1_0_get", N, 95);
    /* ((0x00001004 + (N) * 0x10) bits 15:0) field AGE_THRES_1_0 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_2_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_MSK) >> CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_AGE_THRES_1_0_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_AGE_THRES_1_0_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_AGE_THRES_1_0_set( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_AGE_THRES_1_0_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_THRES_1_0_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_THRES_1_0_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_THRES_1_0_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001004 + (N) * 0x10) bits 15:0) field AGE_THRES_1_0 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
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
        /* ((0x00001004 + (N) * 0x10) bits 15:0) field AGE_THRES_1_0 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
        mpmo_reg_CFG_RAM_2_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_AGE_THRES_1_0_get( mpmo_buffer_t *b_ptr,
                                                         mpmo_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_AGE_THRES_1_0_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_AGE_THRES_1_0_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_AGE_THRES_1_0_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_AGE_THRES_1_0_get", stop_bit, 15 );
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
    /* ((0x00001004 + (N) * 0x10) bits 15:0) field AGE_THRES_1_0 of register PMC_CORE_REG_CFG_RAM_2 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_2_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_MSK)
                  >> CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_MSK, CORE_REG_CFG_RAM_2_BIT_AGE_THRES_1_0_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_AGE_THRES_1_0_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_RAMP_UP_DWN_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_RAMP_UP_DWN_set( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_RAMP_UP_DWN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_RAMP_UP_DWN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_RAMP_UP_DWN_set", N, value );

    /* ((0x00001008 + (N) * 0x10) bits 24:24) bits 0:0 use field RAMP_UP_DWN of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    mpmo_reg_CFG_RAM_3_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_3_BIT_RAMP_UP_DWN_MSK,
                                        CORE_REG_CFG_RAM_3_BIT_RAMP_UP_DWN_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_RAMP_UP_DWN_get( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_RAMP_UP_DWN_get( mpmo_buffer_t *b_ptr,
                                                 mpmo_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_RAMP_UP_DWN_get", N, 95);
    /* ((0x00001008 + (N) * 0x10) bits 24:24) bits 0:0 use field RAMP_UP_DWN of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_3_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_3_BIT_RAMP_UP_DWN_MSK) >> CORE_REG_CFG_RAM_3_BIT_RAMP_UP_DWN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_RAMP_UP_DWN_get", N, value );

    return value;
}
static INLINE void mpmo_field_CN_TARGET_set( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CN_TARGET_set( mpmo_buffer_t *b_ptr,
                                             mpmo_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CN_TARGET_set", N, 95);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CN_TARGET_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_CN_TARGET_set", N, value );

    /* ((0x00001008 + (N) * 0x10) bits 0:23) bits 0:23 use field CN_TARGET of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    mpmo_reg_CFG_RAM_3_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_3_BIT_CN_TARGET_MSK,
                                        CORE_REG_CFG_RAM_3_BIT_CN_TARGET_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_CN_TARGET_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CN_TARGET_get( mpmo_buffer_t *b_ptr,
                                               mpmo_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CN_TARGET_get", N, 95);
    /* ((0x00001008 + (N) * 0x10) bits 0:23) bits 0:23 use field CN_TARGET of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_3_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_3_BIT_CN_TARGET_MSK) >> CORE_REG_CFG_RAM_3_BIT_CN_TARGET_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_CN_TARGET_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_CN_TARGET_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_CN_TARGET_set( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_CN_TARGET_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CN_TARGET_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CN_TARGET_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CN_TARGET_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001008 + (N) * 0x10) bits 0:23) bits 0:23 use field CN_TARGET of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
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
        /* ((0x00001008 + (N) * 0x10) bits 0:23) bits 0:23 use field CN_TARGET of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
        mpmo_reg_CFG_RAM_3_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_3_BIT_CN_TARGET_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_3_BIT_CN_TARGET_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_CN_TARGET_get( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_CN_TARGET_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_CN_TARGET_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CN_TARGET_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CN_TARGET_get", stop_bit, 23 );
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
    /* ((0x00001008 + (N) * 0x10) bits 0:23) bits 0:23 use field CN_TARGET of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_3_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_3_BIT_CN_TARGET_MSK)
                  >> CORE_REG_CFG_RAM_3_BIT_CN_TARGET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_3_BIT_CN_TARGET_MSK, CORE_REG_CFG_RAM_3_BIT_CN_TARGET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CN_TARGET_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_CN_TARGET2_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CN_TARGET2_set( mpmo_buffer_t *b_ptr,
                                              mpmo_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CN_TARGET2_set", N, 95);
    if (value > 16777215)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CN_TARGET2_set", value, 16777215);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_CN_TARGET2_set", N, value );

    /* ((0x00001008 + (N) * 0x10) bits 31:8) field CN_TARGET2 of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    mpmo_reg_CFG_RAM_3_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_MSK,
                                        CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_CN_TARGET2_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CN_TARGET2_get( mpmo_buffer_t *b_ptr,
                                                mpmo_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CN_TARGET2_get", N, 95);
    /* ((0x00001008 + (N) * 0x10) bits 31:8) field CN_TARGET2 of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_3_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_MSK) >> CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_CN_TARGET2_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_CN_TARGET2_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_CN_TARGET2_set( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_CN_TARGET2_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CN_TARGET2_set", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CN_TARGET2_set", stop_bit, 23 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CN_TARGET2_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001008 + (N) * 0x10) bits 31:8) field CN_TARGET2 of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
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
        /* ((0x00001008 + (N) * 0x10) bits 31:8) field CN_TARGET2 of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
        mpmo_reg_CFG_RAM_3_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_CN_TARGET2_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_CN_TARGET2_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_CN_TARGET2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CN_TARGET2_get", stop_bit, start_bit );
    if (stop_bit > 23) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CN_TARGET2_get", stop_bit, 23 );
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
    /* ((0x00001008 + (N) * 0x10) bits 31:8) field CN_TARGET2 of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_3_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_MSK)
                  >> CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_MSK, CORE_REG_CFG_RAM_3_BIT_CN_TARGET2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CN_TARGET2_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void mpmo_field_CN_TARGET_DECIMAL_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_CN_TARGET_DECIMAL_set( mpmo_buffer_t *b_ptr,
                                                     mpmo_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CN_TARGET_DECIMAL_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mpmo_field_CN_TARGET_DECIMAL_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "mpmo_field_CN_TARGET_DECIMAL_set", N, value );

    /* ((0x00001008 + (N) * 0x10) bits 7:0) field CN_TARGET_DECIMAL of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    mpmo_reg_CFG_RAM_3_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_MSK,
                                        CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_OFF,
                                        value);
}

static INLINE UINT32 mpmo_field_CN_TARGET_DECIMAL_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CN_TARGET_DECIMAL_get( mpmo_buffer_t *b_ptr,
                                                       mpmo_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_CN_TARGET_DECIMAL_get", N, 95);
    /* ((0x00001008 + (N) * 0x10) bits 7:0) field CN_TARGET_DECIMAL of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_3_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_MSK) >> CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mpmo_field_CN_TARGET_DECIMAL_get", N, value );

    return value;
}
static INLINE void mpmo_field_range_CN_TARGET_DECIMAL_set( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mpmo_field_range_CN_TARGET_DECIMAL_set( mpmo_buffer_t *b_ptr,
                                                           mpmo_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_CN_TARGET_DECIMAL_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CN_TARGET_DECIMAL_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CN_TARGET_DECIMAL_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CN_TARGET_DECIMAL_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00001008 + (N) * 0x10) bits 7:0) field CN_TARGET_DECIMAL of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
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
        /* ((0x00001008 + (N) * 0x10) bits 7:0) field CN_TARGET_DECIMAL of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
        mpmo_reg_CFG_RAM_3_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_OFF + subfield_offset),
                                            CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mpmo_field_range_CN_TARGET_DECIMAL_get( mpmo_buffer_t *b_ptr,
                                                             mpmo_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_CN_TARGET_DECIMAL_get( mpmo_buffer_t *b_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "mpmo_field_range_CN_TARGET_DECIMAL_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CN_TARGET_DECIMAL_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CN_TARGET_DECIMAL_get", stop_bit, 7 );
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
    /* ((0x00001008 + (N) * 0x10) bits 7:0) field CN_TARGET_DECIMAL of register PMC_CORE_REG_CFG_RAM_3 index N=0..95 */
    reg_value = mpmo_reg_CFG_RAM_3_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_MSK)
                  >> CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_MSK, CORE_REG_CFG_RAM_3_BIT_CN_TARGET_DECIMAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CN_TARGET_DECIMAL_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 mpmo_field_RAMP_STATE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_RAMP_STATE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 16:15) field RAMP_STATE of register PMC_CORE_REG_DEBUG_DATA */
    reg_value = mpmo_reg_DEBUG_DATA_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DEBUG_DATA_BIT_RAMP_STATE_MSK) >> CORE_REG_DEBUG_DATA_BIT_RAMP_STATE_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_RAMP_STATE_get", value );

    return value;
}
static INLINE UINT32 mpmo_field_range_RAMP_STATE_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_RAMP_STATE_get( mpmo_buffer_t *b_ptr,
                                                      mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_RAMP_STATE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_RAMP_STATE_get", stop_bit, 1 );
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
    /* (0x0000002c bits 16:15) field RAMP_STATE of register PMC_CORE_REG_DEBUG_DATA */
    reg_value = mpmo_reg_DEBUG_DATA_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_DEBUG_DATA_BIT_RAMP_STATE_MSK)
                  >> CORE_REG_DEBUG_DATA_BIT_RAMP_STATE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_DEBUG_DATA_BIT_RAMP_STATE_MSK, CORE_REG_DEBUG_DATA_BIT_RAMP_STATE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_RAMP_STATE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mpmo_field_CURR_CSI_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_CURR_CSI_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 4:0) field CURR_CSI of register PMC_CORE_REG_DEBUG_DATA */
    reg_value = mpmo_reg_DEBUG_DATA_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DEBUG_DATA_BIT_CURR_CSI_MSK) >> CORE_REG_DEBUG_DATA_BIT_CURR_CSI_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_CURR_CSI_get", value );

    return value;
}
static INLINE UINT32 mpmo_field_range_CURR_CSI_get( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_CURR_CSI_get( mpmo_buffer_t *b_ptr,
                                                    mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_CURR_CSI_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_CURR_CSI_get", stop_bit, 4 );
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
    /* (0x0000002c bits 4:0) field CURR_CSI of register PMC_CORE_REG_DEBUG_DATA */
    reg_value = mpmo_reg_DEBUG_DATA_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_DEBUG_DATA_BIT_CURR_CSI_MSK)
                  >> CORE_REG_DEBUG_DATA_BIT_CURR_CSI_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_DEBUG_DATA_BIT_CURR_CSI_MSK, CORE_REG_DEBUG_DATA_BIT_CURR_CSI_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_CURR_CSI_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mpmo_field_PKT_AGE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_PKT_AGE_get( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 15:0) field PKT_AGE of register PMC_CORE_REG_DEBUG_DATA_1 */
    reg_value = mpmo_reg_DEBUG_DATA_1_read(  b_ptr, h_ptr);
    value = (reg_value & CORE_REG_DEBUG_DATA_1_BIT_PKT_AGE_MSK) >> CORE_REG_DEBUG_DATA_1_BIT_PKT_AGE_OFF;
    IOLOG( "%s -> 0x%08x", "mpmo_field_PKT_AGE_get", value );

    return value;
}
static INLINE UINT32 mpmo_field_range_PKT_AGE_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mpmo_field_range_PKT_AGE_get( mpmo_buffer_t *b_ptr,
                                                   mpmo_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mpmo_field_range_PKT_AGE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mpmo_field_range_PKT_AGE_get", stop_bit, 15 );
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
    /* (0x00000030 bits 15:0) field PKT_AGE of register PMC_CORE_REG_DEBUG_DATA_1 */
    reg_value = mpmo_reg_DEBUG_DATA_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & CORE_REG_DEBUG_DATA_1_BIT_PKT_AGE_MSK)
                  >> CORE_REG_DEBUG_DATA_1_BIT_PKT_AGE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CORE_REG_DEBUG_DATA_1_BIT_PKT_AGE_MSK, CORE_REG_DEBUG_DATA_1_BIT_PKT_AGE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mpmo_field_range_PKT_AGE_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MPMO_IO_INLINE_H */
