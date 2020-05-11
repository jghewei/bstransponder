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
 *     and register accessor functions for the rgmp block
 *****************************************************************************/
#ifndef _RGMP_IO_INLINE_H
#define _RGMP_IO_INLINE_H

#include "rgmp_loc.h"
#include "rgmp_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RGMP_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for rgmp
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
    rgmp_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} rgmp_buffer_t;
static INLINE void rgmp_buffer_init( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void rgmp_buffer_init( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "rgmp_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void rgmp_buffer_flush( rgmp_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rgmp_buffer_flush( rgmp_buffer_t *b_ptr )
{
    IOLOG( "rgmp_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 rgmp_reg_read( rgmp_buffer_t *b_ptr,
                                    rgmp_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_read( rgmp_buffer_t *b_ptr,
                                    rgmp_handle_t *h_ptr,
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
static INLINE void rgmp_reg_write( rgmp_buffer_t *b_ptr,
                                   rgmp_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_write( rgmp_buffer_t *b_ptr,
                                   rgmp_handle_t *h_ptr,
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

static INLINE void rgmp_field_set( rgmp_buffer_t *b_ptr,
                                   rgmp_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_field_set( rgmp_buffer_t *b_ptr,
                                   rgmp_handle_t *h_ptr,
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

static INLINE void rgmp_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
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

static INLINE void rgmp_burst_read( rgmp_buffer_t *b_ptr,
                                    rgmp_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_burst_read( rgmp_buffer_t *b_ptr,
                                    rgmp_handle_t *h_ptr,
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

static INLINE void rgmp_burst_write( rgmp_buffer_t *b_ptr,
                                     rgmp_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_burst_write( rgmp_buffer_t *b_ptr,
                                     rgmp_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE rgmp_poll( rgmp_buffer_t *b_ptr,
                                              rgmp_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_poll( rgmp_buffer_t *b_ptr,
                                              rgmp_handle_t *h_ptr,
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
 *  register access functions for rgmp
 * ==================================================================================
 */

static INLINE void rgmp_reg_CHANNEL_RESET_array_write( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CHANNEL_RESET_array_write( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rgmp_reg_CHANNEL_RESET_array_write", value );
    rgmp_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RGMP96_CORE_REG_CHANNEL_RESET(N),
                    value);
}

static INLINE void rgmp_reg_CHANNEL_RESET_array_field_set( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CHANNEL_RESET_array_field_set( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CHANNEL_RESET_array_field_set", N, mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RGMP96_CORE_REG_CHANNEL_RESET(N),
                    mask,
                    PMC_RGMP96_CORE_REG_CHANNEL_RESET_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rgmp_reg_CHANNEL_RESET_array_read( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CHANNEL_RESET_array_read( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_RGMP96_CORE_REG_CHANNEL_RESET(N));

    IOLOG( "%s -> 0x%08x; N=%d", "rgmp_reg_CHANNEL_RESET_array_read", reg_value, N);
    return reg_value;
}

static INLINE void rgmp_reg_DPLM_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_DPLM_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_DPLM_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_DPLM_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_DPLM_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_DPLM_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_DPLM_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_DPLM_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_DPLM_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_DPLM_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_DPLM_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_DPLM_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_DPLM_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_DPLM_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_DPLM_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_DPLM_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_DPLM_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_DPLM_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_DPLM_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_CSF_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CSF_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CSF_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CSF_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CSF_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_CSF_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32 N,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CSF_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32 N,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CSF_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_CSF_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_CSF_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_CSF_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 ofs,
                                                         UINT32 len,
                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CSF_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 ofs,
                                                         UINT32 len,
                                                         UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CSF_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CSF_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CSF_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CSF_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                     rgmp_handle_t *h_ptr,
                                                     UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CSF_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                     rgmp_handle_t *h_ptr,
                                                     UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CSF_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CSF_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                   rgmp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                   rgmp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_CM_OUT_LIMIT_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_OUT_LIMIT_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CM_OUT_LIMIT_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CM_OUT_LIMIT_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CM_OUT_LIMIT_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CM_OUT_LIMIT_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_CND_MAX_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CND_MAX_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CND_MAX_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CND_MAX_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CND_MAX_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_CND_MAX_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                            rgmp_handle_t *h_ptr,
                                                            UINT32 N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CND_MAX_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                            rgmp_handle_t *h_ptr,
                                                            UINT32 N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CND_MAX_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_CND_MAX_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_CND_MAX_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_CND_MAX_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CND_MAX_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CND_MAX_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CND_MAX_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CND_MAX_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CND_MAX_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CND_MAX_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CND_MAX_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CND_MAX_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                      rgmp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                      rgmp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                   rgmp_handle_t *h_ptr,
                                                                   UINT32 N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                   rgmp_handle_t *h_ptr,
                                                                   UINT32 N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                      rgmp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                      rgmp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_PSERV_LATE_EXP_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                   rgmp_handle_t *h_ptr,
                                                                   UINT32 N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_LATE_EXP_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                   rgmp_handle_t *h_ptr,
                                                                   UINT32 N,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_PSERV_LATE_EXP_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_PSERV_LATE_EXP_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_PSERV_LATE_EXP_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_PSERV_LATE_EXP_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_SSF_SF_RCVD_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_SSF_SF_RCVD_INT_EN_array_field_set( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_SSF_SF_RCVD_INT_EN_array_field_set", mask, ofs, value );
    rgmp_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN(N),
                    mask,
                    PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_SSF_SF_RCVD_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_SSF_SF_RCVD_INT_EN_array_read( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_SSF_SF_RCVD_INT_EN_array_read", reg_value);
    return reg_value;
}

static INLINE void rgmp_reg_DPLM_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32 ofs,
                                                        UINT32 len,
                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_DPLM_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32 ofs,
                                                        UINT32 len,
                                                        UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_DPLM_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_DPLM_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_DPLM_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_DPLM_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                      rgmp_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_DPLM_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                      rgmp_handle_t *h_ptr,
                                                                      UINT32 N,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_DPLM_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_DPLM_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_DPLM_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_DPLM_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_DPLM_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_DPLM_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_DPLM_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_DPLM_INT_array_read( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_DPLM_INT_array_read( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_DPLM_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_DPLM_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_DPLM_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_DPLM_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_DPLM_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_DPLM_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_CSF_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CSF_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CSF_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CSF_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CSF_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_CSF_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CSF_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CSF_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_CSF_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_CSF_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 ofs,
                                                      UINT32 len,
                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CSF_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 ofs,
                                                      UINT32 len,
                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CSF_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CSF_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CSF_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CSF_INT_array_read( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CSF_INT_array_read( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CSF_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CSF_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CSF_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CSF_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N,
                                                                UINT32 mask,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_CSF_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CSF_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                             rgmp_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                             rgmp_handle_t *h_ptr,
                                                                             UINT32 ofs,
                                                                             UINT32 len,
                                                                             UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                           rgmp_handle_t *h_ptr,
                                                                                           UINT32 N,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                           rgmp_handle_t *h_ptr,
                                                                                           UINT32 N,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                            rgmp_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                            rgmp_handle_t *h_ptr,
                                                                            UINT32 ofs,
                                                                            UINT32 len,
                                                                            UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_read( rgmp_buffer_t *b_ptr,
                                                                        rgmp_handle_t *h_ptr,
                                                                        UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_read( rgmp_buffer_t *b_ptr,
                                                                        rgmp_handle_t *h_ptr,
                                                                        UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                                      rgmp_handle_t *h_ptr,
                                                                                      UINT32 N,
                                                                                      UINT32 mask,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                                      rgmp_handle_t *h_ptr,
                                                                                      UINT32 N,
                                                                                      UINT32 mask,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_CND_MAX_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CND_MAX_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CND_MAX_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CND_MAX_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CND_MAX_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_CND_MAX_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                         rgmp_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CND_MAX_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                         rgmp_handle_t *h_ptr,
                                                                         UINT32 N,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CND_MAX_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_CND_MAX_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_CND_MAX_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CND_MAX_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CND_MAX_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CND_MAX_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CND_MAX_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CND_MAX_INT_array_read( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CND_MAX_INT_array_read( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CND_MAX_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CND_MAX_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CND_MAX_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CND_MAX_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_CND_MAX_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CND_MAX_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                   rgmp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                   rgmp_handle_t *h_ptr,
                                                                   UINT32 ofs,
                                                                   UINT32 len,
                                                                   UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                 rgmp_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                 rgmp_handle_t *h_ptr,
                                                                                 UINT32 N,
                                                                                 UINT32 mask,
                                                                                 UINT32 ofs,
                                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_JC_ERR_SYNCLOSS_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_JC_ERR_SYNCLOSS_INT_array_read( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_JC_ERR_SYNCLOSS_INT_array_read( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_JC_ERR_SYNCLOSS_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_JC_ERR_SYNCLOSS_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                            rgmp_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_JC_ERR_SYNCLOSS_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                            rgmp_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_JC_ERR_SYNCLOSS_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                rgmp_handle_t *h_ptr,
                                                                                UINT32 N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                rgmp_handle_t *h_ptr,
                                                                                UINT32 N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 ofs,
                                                                 UINT32 len,
                                                                 UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CM_CND_CRC_ERR_INT_array_read( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CM_CND_CRC_ERR_INT_array_read( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CM_CND_CRC_ERR_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CM_CND_CRC_ERR_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                           rgmp_handle_t *h_ptr,
                                                                           UINT32 N,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CM_CND_CRC_ERR_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                           rgmp_handle_t *h_ptr,
                                                                           UINT32 N,
                                                                           UINT32 mask,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_CM_CND_CRC_ERR_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                      rgmp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                      rgmp_handle_t *h_ptr,
                                                                      UINT32 ofs,
                                                                      UINT32 len,
                                                                      UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                    rgmp_handle_t *h_ptr,
                                                                                    UINT32 N,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                    rgmp_handle_t *h_ptr,
                                                                                    UINT32 N,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_read( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                               rgmp_handle_t *h_ptr,
                                                                               UINT32 N,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                               rgmp_handle_t *h_ptr,
                                                                               UINT32 N,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 ofs,
                                                                     UINT32 len,
                                                                     UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_PSERV_LATE_EXPIRY_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                   rgmp_handle_t *h_ptr,
                                                                                   UINT32 N,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_LATE_EXPIRY_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                                   rgmp_handle_t *h_ptr,
                                                                                   UINT32 N,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_PSERV_LATE_EXPIRY_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 ofs,
                                                                    UINT32 len,
                                                                    UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_PSERV_LATE_EXPIRY_INT_array_read( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_PSERV_LATE_EXPIRY_INT_array_read( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_PSERV_LATE_EXPIRY_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_PSERV_LATE_EXPIRY_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                              rgmp_handle_t *h_ptr,
                                                                              UINT32 N,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_PSERV_LATE_EXPIRY_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                              rgmp_handle_t *h_ptr,
                                                                              UINT32 N,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_PSERV_LATE_EXPIRY_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_SSF_SF_RCVD_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                               rgmp_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_SSF_SF_RCVD_INT_array_burst_write( rgmp_buffer_t *b_ptr,
                                                               rgmp_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_SSF_SF_RCVD_INT_array_burst_write", ofs, len, 3 );
    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_SSF_SF_RCVD_INT_array_burst_write", 3, ofs, len);
    rgmp_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT(ofs),
                      len,
                      value);

}

static INLINE void rgmp_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                             rgmp_handle_t *h_ptr,
                                                                             UINT32 N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set( rgmp_buffer_t *b_ptr,
                                                                             rgmp_handle_t *h_ptr,
                                                                             UINT32 N,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rgmp_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set", mask, ofs, value );
    rgmp_action_on_write_field_set( b_ptr,
                                    h_ptr,
                                    MEM_TYPE_STATUS,
                                    PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT(N),
                                    mask,
                                    ofs,
                                    value);

}

static INLINE void rgmp_reg_SSF_SF_RCVD_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_SSF_SF_RCVD_INT_array_burst_read( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_SSF_SF_RCVD_INT_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_SSF_SF_RCVD_INT_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_SSF_SF_RCVD_INT_array_read( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_SSF_SF_RCVD_INT_array_read( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_SSF_SF_RCVD_INT_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_SSF_SF_RCVD_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                        rgmp_handle_t *h_ptr,
                                                                        UINT32 N,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_SSF_SF_RCVD_INT_array_poll( rgmp_buffer_t *b_ptr,
                                                                        rgmp_handle_t *h_ptr,
                                                                        UINT32 N,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_SSF_SF_RCVD_INT_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_DPLM_INT_VAL_array_burst_read( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_DPLM_INT_VAL_array_burst_read( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_DPLM_INT_VAL_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_DPLM_INT_VAL(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_DPLM_INT_VAL_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_DPLM_INT_VAL_array_read( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_DPLM_INT_VAL_array_read( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_DPLM_INT_VAL(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_DPLM_INT_VAL_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_DPLM_INT_VAL_array_poll( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_DPLM_INT_VAL_array_poll( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 N,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_DPLM_INT_VAL_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_DPLM_INT_VAL(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_CSF_INT_VAL_array_burst_read( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_CSF_INT_VAL_array_burst_read( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_CSF_INT_VAL_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_CSF_INT_VAL(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_CSF_INT_VAL_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_CSF_INT_VAL_array_read( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_CSF_INT_VAL_array_read( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_CSF_INT_VAL(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_CSF_INT_VAL_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CSF_INT_VAL_array_poll( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_CSF_INT_VAL_array_poll( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 N,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_CSF_INT_VAL_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_CSF_INT_VAL(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


static INLINE void rgmp_reg_SSF_SF_RCVD_INT_VAL_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value ) ALWAYS_INLINE;
static INLINE void rgmp_reg_SSF_SF_RCVD_INT_VAL_array_burst_read( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 ofs,
                                                                  UINT32 len,
                                                                  UINT32 *value )
{
    if (len + ofs > 3)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "rgmp_reg_SSF_SF_RCVD_INT_VAL_array_burst_read", ofs, len, 3 );
    rgmp_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "rgmp_reg_SSF_SF_RCVD_INT_VAL_array_burst_read", 3, ofs, len);
}

static INLINE UINT32 rgmp_reg_SSF_SF_RCVD_INT_VAL_array_read( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_reg_SSF_SF_RCVD_INT_VAL_array_read( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 N )
{
    UINT32 reg_value;

    reg_value = rgmp_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL(N));

    IOLOG( "%s -> 0x%08x;", "rgmp_reg_SSF_SF_RCVD_INT_VAL_array_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_SSF_SF_RCVD_INT_VAL_array_poll( rgmp_buffer_t *b_ptr,
                                                                            rgmp_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_reg_SSF_SF_RCVD_INT_VAL_array_poll( rgmp_buffer_t *b_ptr,
                                                                            rgmp_handle_t *h_ptr,
                                                                            UINT32 N,
                                                                            UINT32 mask,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "rgmp_reg_SSF_SF_RCVD_INT_VAL_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return rgmp_poll( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL(N),
                      mask,
                      value,
                      cmp,
                      max_count,
                      num_failed_polls,
                      delay_between_polls_in_microseconds);

}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void rgmp_field_CHAN_RSTB_set( rgmp_buffer_t *b_ptr,
                                             rgmp_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_field_CHAN_RSTB_set( rgmp_buffer_t *b_ptr,
                                             rgmp_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rgmp_field_CHAN_RSTB_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rgmp_field_CHAN_RSTB_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "rgmp_field_CHAN_RSTB_set", N, value );

    /* ((0x00000000 + (N) * 0x4) bits 0) field CHAN_RSTB of register PMC_RGMP96_CORE_REG_CHANNEL_RESET index N=0..95 */
    rgmp_reg_CHANNEL_RESET_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            RGMP96_CORE_REG_CHANNEL_RESET_BIT_CHAN_RSTB_MSK,
                                            RGMP96_CORE_REG_CHANNEL_RESET_BIT_CHAN_RSTB_OFF,
                                            value);
}

static INLINE UINT32 rgmp_field_CHAN_RSTB_get( rgmp_buffer_t *b_ptr,
                                               rgmp_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rgmp_field_CHAN_RSTB_get( rgmp_buffer_t *b_ptr,
                                               rgmp_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "rgmp_field_CHAN_RSTB_get", N, 95);
    /* ((0x00000000 + (N) * 0x4) bits 0) field CHAN_RSTB of register PMC_RGMP96_CORE_REG_CHANNEL_RESET index N=0..95 */
    reg_value = rgmp_reg_CHANNEL_RESET_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & RGMP96_CORE_REG_CHANNEL_RESET_BIT_CHAN_RSTB_MSK) >> RGMP96_CORE_REG_CHANNEL_RESET_BIT_CHAN_RSTB_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "rgmp_field_CHAN_RSTB_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_N_burstsize3
 * ==================================================================================
 */
static INLINE void rgmp_lfield_DPLM_E_set( rgmp_buffer_t *b_ptr,
                                           rgmp_handle_t *h_ptr,
                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_DPLM_E_set( rgmp_buffer_t *b_ptr,
                                           rgmp_handle_t *h_ptr,
                                           UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_DPLM_E_set");
    rgmp_reg_DPLM_INT_EN_array_burst_write( b_ptr,
                                            h_ptr,
                                            0,
                                            3,
                                            value);
}

static INLINE void rgmp_lfield_DPLM_E_get( rgmp_buffer_t *b_ptr,
                                           rgmp_handle_t *h_ptr,
                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_DPLM_E_get( rgmp_buffer_t *b_ptr,
                                           rgmp_handle_t *h_ptr,
                                           UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_DPLM_E_get");
    rgmp_reg_DPLM_INT_EN_array_burst_read( b_ptr,
                                           h_ptr,
                                           0,
                                           3,
                                           (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_DPLM_E_set( rgmp_buffer_t *b_ptr,
                                                 rgmp_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_DPLM_E_set( rgmp_buffer_t *b_ptr,
                                                 rgmp_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_DPLM_E_set", start_bit, stop_bit, value);
    rgmp_reg_DPLM_INT_EN_array_field_set( b_ptr,
                                          h_ptr,
                                          word_number,
                                          mask,
                                          field_ofs,
                                          value);
}

static INLINE UINT32 rgmp_lfield_range_DPLM_E_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_DPLM_E_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_DPLM_INT_EN_array_read( b_ptr,
                                                 h_ptr,
                                                 word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_DPLM_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void rgmp_lfield_CSF_E_set( rgmp_buffer_t *b_ptr,
                                          rgmp_handle_t *h_ptr,
                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CSF_E_set( rgmp_buffer_t *b_ptr,
                                          rgmp_handle_t *h_ptr,
                                          UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CSF_E_set");
    rgmp_reg_CSF_INT_EN_array_burst_write( b_ptr,
                                           h_ptr,
                                           0,
                                           3,
                                           value);
}

static INLINE void rgmp_lfield_CSF_E_get( rgmp_buffer_t *b_ptr,
                                          rgmp_handle_t *h_ptr,
                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CSF_E_get( rgmp_buffer_t *b_ptr,
                                          rgmp_handle_t *h_ptr,
                                          UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CSF_E_get");
    rgmp_reg_CSF_INT_EN_array_burst_read( b_ptr,
                                          h_ptr,
                                          0,
                                          3,
                                          (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_CSF_E_set( rgmp_buffer_t *b_ptr,
                                                rgmp_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_CSF_E_set( rgmp_buffer_t *b_ptr,
                                                rgmp_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit,
                                                UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CSF_E_set", start_bit, stop_bit, value);
    rgmp_reg_CSF_INT_EN_array_field_set( b_ptr,
                                         h_ptr,
                                         word_number,
                                         mask,
                                         field_ofs,
                                         value);
}

static INLINE UINT32 rgmp_lfield_range_CSF_E_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CSF_E_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CSF_INT_EN_array_read( b_ptr,
                                                h_ptr,
                                                word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CSF_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void rgmp_lfield_CM_OUT_LIMIT_E_set( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CM_OUT_LIMIT_E_set( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CM_OUT_LIMIT_E_set");
    rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    0,
                                                    3,
                                                    value);
}

static INLINE void rgmp_lfield_CM_OUT_LIMIT_E_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CM_OUT_LIMIT_E_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CM_OUT_LIMIT_E_get");
    rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_CM_OUT_LIMIT_E_set( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_CM_OUT_LIMIT_E_set( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_OUT_LIMIT_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CM_OUT_LIMIT_E_set", start_bit, stop_bit, value);
    rgmp_reg_CM_OUT_LIMIT_INT_EN_array_field_set( b_ptr,
                                                  h_ptr,
                                                  word_number,
                                                  mask,
                                                  field_ofs,
                                                  value);
}

static INLINE UINT32 rgmp_lfield_range_CM_OUT_LIMIT_E_get( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CM_OUT_LIMIT_E_get( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_OUT_LIMIT_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CM_OUT_LIMIT_INT_EN_array_read( b_ptr,
                                                         h_ptr,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CM_OUT_LIMIT_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void rgmp_lfield_CND_MAX_E_set( rgmp_buffer_t *b_ptr,
                                              rgmp_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CND_MAX_E_set( rgmp_buffer_t *b_ptr,
                                              rgmp_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CND_MAX_E_set");
    rgmp_reg_CND_MAX_INT_EN_array_burst_write( b_ptr,
                                               h_ptr,
                                               0,
                                               3,
                                               value);
}

static INLINE void rgmp_lfield_CND_MAX_E_get( rgmp_buffer_t *b_ptr,
                                              rgmp_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CND_MAX_E_get( rgmp_buffer_t *b_ptr,
                                              rgmp_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CND_MAX_E_get");
    rgmp_reg_CND_MAX_INT_EN_array_burst_read( b_ptr,
                                              h_ptr,
                                              0,
                                              3,
                                              (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_CND_MAX_E_set( rgmp_buffer_t *b_ptr,
                                                    rgmp_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_CND_MAX_E_set( rgmp_buffer_t *b_ptr,
                                                    rgmp_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CND_MAX_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CND_MAX_E_set", start_bit, stop_bit, value);
    rgmp_reg_CND_MAX_INT_EN_array_field_set( b_ptr,
                                             h_ptr,
                                             word_number,
                                             mask,
                                             field_ofs,
                                             value);
}

static INLINE UINT32 rgmp_lfield_range_CND_MAX_E_get( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CND_MAX_E_get( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CND_MAX_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CND_MAX_INT_EN_array_read( b_ptr,
                                                    h_ptr,
                                                    word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CND_MAX_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void rgmp_lfield_JC_ERR_SYNCLOSS_E_set( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_JC_ERR_SYNCLOSS_E_set( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_JC_ERR_SYNCLOSS_E_set");
    rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void rgmp_lfield_JC_ERR_SYNCLOSS_E_get( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_JC_ERR_SYNCLOSS_E_get( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_JC_ERR_SYNCLOSS_E_get");
    rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_JC_ERR_SYNCLOSS_E_set( rgmp_buffer_t *b_ptr,
                                                            rgmp_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_JC_ERR_SYNCLOSS_E_set( rgmp_buffer_t *b_ptr,
                                                            rgmp_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_JC_ERR_SYNCLOSS_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_JC_ERR_SYNCLOSS_E_set", start_bit, stop_bit, value);
    rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 rgmp_lfield_range_JC_ERR_SYNCLOSS_E_get( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_JC_ERR_SYNCLOSS_E_get( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_JC_ERR_SYNCLOSS_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_JC_ERR_SYNCLOSS_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void rgmp_lfield_CM_CND_CRC_ERR_E_set( rgmp_buffer_t *b_ptr,
                                                     rgmp_handle_t *h_ptr,
                                                     UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CM_CND_CRC_ERR_E_set( rgmp_buffer_t *b_ptr,
                                                     rgmp_handle_t *h_ptr,
                                                     UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CM_CND_CRC_ERR_E_set");
    rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_write( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      value);
}

static INLINE void rgmp_lfield_CM_CND_CRC_ERR_E_get( rgmp_buffer_t *b_ptr,
                                                     rgmp_handle_t *h_ptr,
                                                     UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CM_CND_CRC_ERR_E_get( rgmp_buffer_t *b_ptr,
                                                     rgmp_handle_t *h_ptr,
                                                     UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CM_CND_CRC_ERR_E_get");
    rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_read( b_ptr,
                                                     h_ptr,
                                                     0,
                                                     3,
                                                     (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_CM_CND_CRC_ERR_E_set( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_CM_CND_CRC_ERR_E_set( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_CND_CRC_ERR_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CM_CND_CRC_ERR_E_set", start_bit, stop_bit, value);
    rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_field_set( b_ptr,
                                                    h_ptr,
                                                    word_number,
                                                    mask,
                                                    field_ofs,
                                                    value);
}

static INLINE UINT32 rgmp_lfield_range_CM_CND_CRC_ERR_E_get( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CM_CND_CRC_ERR_E_get( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_CND_CRC_ERR_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_read( b_ptr,
                                                           h_ptr,
                                                           word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CM_CND_CRC_ERR_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void rgmp_lfield_PSERV_EARLY_E_set( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_PSERV_EARLY_E_set( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_PSERV_EARLY_E_set");
    rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void rgmp_lfield_PSERV_EARLY_E_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_PSERV_EARLY_E_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_PSERV_EARLY_E_get");
    rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_PSERV_EARLY_E_set( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_PSERV_EARLY_E_set( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_EARLY_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_PSERV_EARLY_E_set", start_bit, stop_bit, value);
    rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_field_set( b_ptr,
                                                     h_ptr,
                                                     word_number,
                                                     mask,
                                                     field_ofs,
                                                     value);
}

static INLINE UINT32 rgmp_lfield_range_PSERV_EARLY_E_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_PSERV_EARLY_E_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_EARLY_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_PSERV_EARLY_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void rgmp_lfield_PSERV_LATE_E_set( rgmp_buffer_t *b_ptr,
                                                 rgmp_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_PSERV_LATE_E_set( rgmp_buffer_t *b_ptr,
                                                 rgmp_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_PSERV_LATE_E_set");
    rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_write( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      value);
}

static INLINE void rgmp_lfield_PSERV_LATE_E_get( rgmp_buffer_t *b_ptr,
                                                 rgmp_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_PSERV_LATE_E_get( rgmp_buffer_t *b_ptr,
                                                 rgmp_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_PSERV_LATE_E_get");
    rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_read( b_ptr,
                                                     h_ptr,
                                                     0,
                                                     3,
                                                     (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_PSERV_LATE_E_set( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_PSERV_LATE_E_set( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_LATE_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_PSERV_LATE_E_set", start_bit, stop_bit, value);
    rgmp_reg_PSERV_LATE_EXP_INT_EN_array_field_set( b_ptr,
                                                    h_ptr,
                                                    word_number,
                                                    mask,
                                                    field_ofs,
                                                    value);
}

static INLINE UINT32 rgmp_lfield_range_PSERV_LATE_E_get( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_PSERV_LATE_E_get( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_LATE_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_PSERV_LATE_EXP_INT_EN_array_read( b_ptr,
                                                           h_ptr,
                                                           word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_PSERV_LATE_E_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void rgmp_lfield_SSF_SF_RCVD_E_set( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_SSF_SF_RCVD_E_set( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_SSF_SF_RCVD_E_set");
    rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   value);
}

static INLINE void rgmp_lfield_SSF_SF_RCVD_E_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_SSF_SF_RCVD_E_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_SSF_SF_RCVD_E_get");
    rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  3,
                                                  (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_SSF_SF_RCVD_E_set( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_SSF_SF_RCVD_E_set( rgmp_buffer_t *b_ptr,
                                                        rgmp_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_E_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_SSF_SF_RCVD_E_set", start_bit, stop_bit, value);
    rgmp_reg_SSF_SF_RCVD_INT_EN_array_field_set( b_ptr,
                                                 h_ptr,
                                                 word_number,
                                                 mask,
                                                 field_ofs,
                                                 value);
}

static INLINE UINT32 rgmp_lfield_range_SSF_SF_RCVD_E_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_SSF_SF_RCVD_E_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_E_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_SSF_SF_RCVD_INT_EN_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_SSF_SF_RCVD_E_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_burstsize3
 * ==================================================================================
 */
static INLINE void rgmp_lfield_DPLM_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                    rgmp_handle_t *h_ptr,
                                                    UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_DPLM_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                    rgmp_handle_t *h_ptr,
                                                    UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_DPLM_I_set_to_clear");
    rgmp_reg_DPLM_INT_array_burst_write( b_ptr,
                                         h_ptr,
                                         0,
                                         3,
                                         value);
}

static INLINE void rgmp_lfield_DPLM_I_get( rgmp_buffer_t *b_ptr,
                                           rgmp_handle_t *h_ptr,
                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_DPLM_I_get( rgmp_buffer_t *b_ptr,
                                           rgmp_handle_t *h_ptr,
                                           UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_DPLM_I_get");
    rgmp_reg_DPLM_INT_array_burst_read( b_ptr,
                                        h_ptr,
                                        0,
                                        3,
                                        (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_DPLM_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_DPLM_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_DPLM_INT_array_action_on_write_field_set( b_ptr,
                                                       h_ptr,
                                                       word_number,
                                                       mask,
                                                       field_ofs,
                                                       value);
}

static INLINE UINT32 rgmp_lfield_range_DPLM_I_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_DPLM_I_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_DPLM_INT_array_read( b_ptr,
                                              h_ptr,
                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_DPLM_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_DPLM_I_poll( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_DPLM_I_poll( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_DPLM_INT_array_poll( b_ptr,
                                         h_ptr,
                                         word_number,
                                         mask,
                                         ( value << field_ofs),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_CSF_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CSF_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CSF_I_set_to_clear");
    rgmp_reg_CSF_INT_array_burst_write(  b_ptr, h_ptr, 0, 3, value);
}

static INLINE void rgmp_lfield_CSF_I_get( rgmp_buffer_t *b_ptr,
                                          rgmp_handle_t *h_ptr,
                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CSF_I_get( rgmp_buffer_t *b_ptr,
                                          rgmp_handle_t *h_ptr,
                                          UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CSF_I_get");
    rgmp_reg_CSF_INT_array_burst_read( b_ptr,
                                       h_ptr,
                                       0,
                                       3,
                                       (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_CSF_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_CSF_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CSF_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_CSF_INT_array_action_on_write_field_set( b_ptr,
                                                      h_ptr,
                                                      word_number,
                                                      mask,
                                                      field_ofs,
                                                      value);
}

static INLINE UINT32 rgmp_lfield_range_CSF_I_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CSF_I_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CSF_INT_array_read( b_ptr,
                                             h_ptr,
                                             word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CSF_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CSF_I_poll( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CSF_I_poll( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CSF_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_CSF_INT_array_poll( b_ptr,
                                        h_ptr,
                                        word_number,
                                        mask,
                                        ( value << field_ofs),
                                        cmp,
                                        max_count,
                                        num_failed_polls,
                                        delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_CM_OUT_LIMIT_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                            rgmp_handle_t *h_ptr,
                                                            UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CM_OUT_LIMIT_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                            rgmp_handle_t *h_ptr,
                                                            UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CM_OUT_LIMIT_I_set_to_clear");
    rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_write( b_ptr,
                                                              h_ptr,
                                                              0,
                                                              3,
                                                              value);
}

static INLINE void rgmp_lfield_CM_OUT_LIMIT_I_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CM_OUT_LIMIT_I_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CM_OUT_LIMIT_I_get");
    rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_read( b_ptr,
                                                             h_ptr,
                                                             0,
                                                             3,
                                                             (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_CM_OUT_LIMIT_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_CM_OUT_LIMIT_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_OUT_LIMIT_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CM_OUT_LIMIT_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_action_on_write_field_set( b_ptr,
                                                                            h_ptr,
                                                                            word_number,
                                                                            mask,
                                                                            field_ofs,
                                                                            value);
}

static INLINE UINT32 rgmp_lfield_range_CM_OUT_LIMIT_I_get( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CM_OUT_LIMIT_I_get( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_OUT_LIMIT_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_read( b_ptr,
                                                                   h_ptr,
                                                                   word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CM_OUT_LIMIT_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CM_OUT_LIMIT_I_poll( rgmp_buffer_t *b_ptr,
                                                                          rgmp_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CM_OUT_LIMIT_I_poll( rgmp_buffer_t *b_ptr,
                                                                          rgmp_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_OUT_LIMIT_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CM_OUT_LIMIT_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_poll( b_ptr,
                                                              h_ptr,
                                                              word_number,
                                                              mask,
                                                              ( value << field_ofs),
                                                              cmp,
                                                              max_count,
                                                              num_failed_polls,
                                                              delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_CND_MAX_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CND_MAX_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                       rgmp_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CND_MAX_I_set_to_clear");
    rgmp_reg_CND_MAX_INT_array_burst_write( b_ptr,
                                            h_ptr,
                                            0,
                                            3,
                                            value);
}

static INLINE void rgmp_lfield_CND_MAX_I_get( rgmp_buffer_t *b_ptr,
                                              rgmp_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CND_MAX_I_get( rgmp_buffer_t *b_ptr,
                                              rgmp_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CND_MAX_I_get");
    rgmp_reg_CND_MAX_INT_array_burst_read( b_ptr,
                                           h_ptr,
                                           0,
                                           3,
                                           (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_CND_MAX_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_CND_MAX_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CND_MAX_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CND_MAX_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_CND_MAX_INT_array_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          word_number,
                                                          mask,
                                                          field_ofs,
                                                          value);
}

static INLINE UINT32 rgmp_lfield_range_CND_MAX_I_get( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CND_MAX_I_get( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CND_MAX_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CND_MAX_INT_array_read( b_ptr,
                                                 h_ptr,
                                                 word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CND_MAX_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CND_MAX_I_poll( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CND_MAX_I_poll( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CND_MAX_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CND_MAX_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_CND_MAX_INT_array_poll( b_ptr,
                                            h_ptr,
                                            word_number,
                                            mask,
                                            ( value << field_ofs),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_JC_ERR_SYNCLOSS_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                               rgmp_handle_t *h_ptr,
                                                               UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_JC_ERR_SYNCLOSS_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                               rgmp_handle_t *h_ptr,
                                                               UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_JC_ERR_SYNCLOSS_I_set_to_clear");
    rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_write( b_ptr,
                                                    h_ptr,
                                                    0,
                                                    3,
                                                    value);
}

static INLINE void rgmp_lfield_JC_ERR_SYNCLOSS_I_get( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_JC_ERR_SYNCLOSS_I_get( rgmp_buffer_t *b_ptr,
                                                      rgmp_handle_t *h_ptr,
                                                      UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_JC_ERR_SYNCLOSS_I_get");
    rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_JC_ERR_SYNCLOSS_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_JC_ERR_SYNCLOSS_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                     rgmp_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_JC_ERR_SYNCLOSS_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_JC_ERR_SYNCLOSS_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_JC_ERR_SYNCLOSS_INT_array_action_on_write_field_set( b_ptr,
                                                                  h_ptr,
                                                                  word_number,
                                                                  mask,
                                                                  field_ofs,
                                                                  value);
}

static INLINE UINT32 rgmp_lfield_range_JC_ERR_SYNCLOSS_I_get( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_JC_ERR_SYNCLOSS_I_get( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_JC_ERR_SYNCLOSS_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_JC_ERR_SYNCLOSS_INT_array_read( b_ptr,
                                                         h_ptr,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_JC_ERR_SYNCLOSS_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_JC_ERR_SYNCLOSS_I_poll( rgmp_buffer_t *b_ptr,
                                                                             rgmp_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_JC_ERR_SYNCLOSS_I_poll( rgmp_buffer_t *b_ptr,
                                                                             rgmp_handle_t *h_ptr,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_JC_ERR_SYNCLOSS_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_JC_ERR_SYNCLOSS_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_JC_ERR_SYNCLOSS_INT_array_poll( b_ptr,
                                                    h_ptr,
                                                    word_number,
                                                    mask,
                                                    ( value << field_ofs),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_CM_CND_CRC_ERR_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CM_CND_CRC_ERR_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                              rgmp_handle_t *h_ptr,
                                                              UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CM_CND_CRC_ERR_I_set_to_clear");
    rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_write( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   value);
}

static INLINE void rgmp_lfield_CM_CND_CRC_ERR_I_get( rgmp_buffer_t *b_ptr,
                                                     rgmp_handle_t *h_ptr,
                                                     UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CM_CND_CRC_ERR_I_get( rgmp_buffer_t *b_ptr,
                                                     rgmp_handle_t *h_ptr,
                                                     UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CM_CND_CRC_ERR_I_get");
    rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_read( b_ptr,
                                                  h_ptr,
                                                  0,
                                                  3,
                                                  (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_CM_CND_CRC_ERR_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_CM_CND_CRC_ERR_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                    rgmp_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_CND_CRC_ERR_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CM_CND_CRC_ERR_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set( b_ptr,
                                                                 h_ptr,
                                                                 word_number,
                                                                 mask,
                                                                 field_ofs,
                                                                 value);
}

static INLINE UINT32 rgmp_lfield_range_CM_CND_CRC_ERR_I_get( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CM_CND_CRC_ERR_I_get( rgmp_buffer_t *b_ptr,
                                                             rgmp_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_CND_CRC_ERR_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CM_CND_CRC_ERR_INT_array_read( b_ptr,
                                                        h_ptr,
                                                        word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CM_CND_CRC_ERR_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CM_CND_CRC_ERR_I_poll( rgmp_buffer_t *b_ptr,
                                                                            rgmp_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CM_CND_CRC_ERR_I_poll( rgmp_buffer_t *b_ptr,
                                                                            rgmp_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_CND_CRC_ERR_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CM_CND_CRC_ERR_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_CM_CND_CRC_ERR_INT_array_poll( b_ptr,
                                                   h_ptr,
                                                   word_number,
                                                   mask,
                                                   ( value << field_ofs),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_PSERV_EARLY_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_PSERV_EARLY_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_PSERV_EARLY_I_set_to_clear");
    rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_write( b_ptr,
                                                       h_ptr,
                                                       0,
                                                       3,
                                                       value);
}

static INLINE void rgmp_lfield_PSERV_EARLY_I_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_PSERV_EARLY_I_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_PSERV_EARLY_I_get");
    rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_read( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_PSERV_EARLY_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_PSERV_EARLY_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_EARLY_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_PSERV_EARLY_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     word_number,
                                                                     mask,
                                                                     field_ofs,
                                                                     value);
}

static INLINE UINT32 rgmp_lfield_range_PSERV_EARLY_I_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_PSERV_EARLY_I_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_EARLY_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_read( b_ptr,
                                                            h_ptr,
                                                            word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_PSERV_EARLY_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_PSERV_EARLY_I_poll( rgmp_buffer_t *b_ptr,
                                                                         rgmp_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_PSERV_EARLY_I_poll( rgmp_buffer_t *b_ptr,
                                                                         rgmp_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_EARLY_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_PSERV_EARLY_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_poll( b_ptr,
                                                       h_ptr,
                                                       word_number,
                                                       mask,
                                                       ( value << field_ofs),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_PSERV_LATE_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_PSERV_LATE_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_PSERV_LATE_I_set_to_clear");
    rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_write( b_ptr,
                                                      h_ptr,
                                                      0,
                                                      3,
                                                      value);
}

static INLINE void rgmp_lfield_PSERV_LATE_I_get( rgmp_buffer_t *b_ptr,
                                                 rgmp_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_PSERV_LATE_I_get( rgmp_buffer_t *b_ptr,
                                                 rgmp_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_PSERV_LATE_I_get");
    rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_read( b_ptr,
                                                     h_ptr,
                                                     0,
                                                     3,
                                                     (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_PSERV_LATE_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_PSERV_LATE_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                rgmp_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_LATE_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_PSERV_LATE_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_PSERV_LATE_EXPIRY_INT_array_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    word_number,
                                                                    mask,
                                                                    field_ofs,
                                                                    value);
}

static INLINE UINT32 rgmp_lfield_range_PSERV_LATE_I_get( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_PSERV_LATE_I_get( rgmp_buffer_t *b_ptr,
                                                         rgmp_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_LATE_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_PSERV_LATE_EXPIRY_INT_array_read( b_ptr,
                                                           h_ptr,
                                                           word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_PSERV_LATE_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_PSERV_LATE_I_poll( rgmp_buffer_t *b_ptr,
                                                                        rgmp_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_PSERV_LATE_I_poll( rgmp_buffer_t *b_ptr,
                                                                        rgmp_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_LATE_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_PSERV_LATE_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_PSERV_LATE_EXPIRY_INT_array_poll( b_ptr,
                                                      h_ptr,
                                                      word_number,
                                                      mask,
                                                      ( value << field_ofs),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_SSF_SF_RCVD_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_SSF_SF_RCVD_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                           rgmp_handle_t *h_ptr,
                                                           UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_SSF_SF_RCVD_I_set_to_clear");
    rgmp_reg_SSF_SF_RCVD_INT_array_burst_write( b_ptr,
                                                h_ptr,
                                                0,
                                                3,
                                                value);
}

static INLINE void rgmp_lfield_SSF_SF_RCVD_I_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_SSF_SF_RCVD_I_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_SSF_SF_RCVD_I_get");
    rgmp_reg_SSF_SF_RCVD_INT_array_burst_read( b_ptr,
                                               h_ptr,
                                               0,
                                               3,
                                               (UINT32 *)value);
}

static INLINE void rgmp_lfield_range_SSF_SF_RCVD_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_range_SSF_SF_RCVD_I_set_to_clear( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit, value);
    rgmp_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set( b_ptr,
                                                              h_ptr,
                                                              word_number,
                                                              mask,
                                                              field_ofs,
                                                              value);
}

static INLINE UINT32 rgmp_lfield_range_SSF_SF_RCVD_I_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_SSF_SF_RCVD_I_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_I_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_SSF_SF_RCVD_INT_array_read( b_ptr,
                                                     h_ptr,
                                                     word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_SSF_SF_RCVD_I_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_SSF_SF_RCVD_I_poll( rgmp_buffer_t *b_ptr,
                                                                         rgmp_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_SSF_SF_RCVD_I_poll( rgmp_buffer_t *b_ptr,
                                                                         rgmp_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_I_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_SSF_SF_RCVD_INT_array_poll( b_ptr,
                                                h_ptr,
                                                word_number,
                                                mask,
                                                ( value << field_ofs),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value_N_burstsize3
 * ==================================================================================
 */
static INLINE void rgmp_lfield_DPLM_V_get( rgmp_buffer_t *b_ptr,
                                           rgmp_handle_t *h_ptr,
                                           UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_DPLM_V_get( rgmp_buffer_t *b_ptr,
                                           rgmp_handle_t *h_ptr,
                                           UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_DPLM_V_get");
    rgmp_reg_DPLM_INT_VAL_array_burst_read( b_ptr,
                                            h_ptr,
                                            0,
                                            3,
                                            (UINT32 *)value);
}

static INLINE UINT32 rgmp_lfield_range_DPLM_V_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_DPLM_V_get( rgmp_buffer_t *b_ptr,
                                                   rgmp_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_V_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_DPLM_INT_VAL_array_read( b_ptr,
                                                  h_ptr,
                                                  word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_DPLM_V_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_DPLM_V_poll( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_DPLM_V_poll( rgmp_buffer_t *b_ptr,
                                                                  rgmp_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_V_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_DPLM_V_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_DPLM_INT_VAL_array_poll( b_ptr,
                                             h_ptr,
                                             word_number,
                                             mask,
                                             ( value << field_ofs),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_CSF_V_get( rgmp_buffer_t *b_ptr,
                                          rgmp_handle_t *h_ptr,
                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_CSF_V_get( rgmp_buffer_t *b_ptr,
                                          rgmp_handle_t *h_ptr,
                                          UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_CSF_V_get");
    rgmp_reg_CSF_INT_VAL_array_burst_read( b_ptr,
                                           h_ptr,
                                           0,
                                           3,
                                           (UINT32 *)value);
}

static INLINE UINT32 rgmp_lfield_range_CSF_V_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_CSF_V_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_V_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_CSF_INT_VAL_array_read( b_ptr,
                                                 h_ptr,
                                                 word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_CSF_V_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CSF_V_poll( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CSF_V_poll( rgmp_buffer_t *b_ptr,
                                                                 rgmp_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_V_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CSF_V_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_CSF_INT_VAL_array_poll( b_ptr,
                                            h_ptr,
                                            word_number,
                                            mask,
                                            ( value << field_ofs),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void rgmp_lfield_SSF_SF_RCVD_V_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void rgmp_lfield_SSF_SF_RCVD_V_get( rgmp_buffer_t *b_ptr,
                                                  rgmp_handle_t *h_ptr,
                                                  UINT32 value[3] )
{
    IOLOG( "%s ", "rgmp_lfield_SSF_SF_RCVD_V_get");
    rgmp_reg_SSF_SF_RCVD_INT_VAL_array_burst_read( b_ptr,
                                                   h_ptr,
                                                   0,
                                                   3,
                                                   (UINT32 *)value);
}

static INLINE UINT32 rgmp_lfield_range_SSF_SF_RCVD_V_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 rgmp_lfield_range_SSF_SF_RCVD_V_get( rgmp_buffer_t *b_ptr,
                                                          rgmp_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value;
    UINT32 word_number;
    UINT32 reg_value;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits));
    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_V_get", start_bit, stop_bit );
    }
    reg_value = rgmp_reg_SSF_SF_RCVD_INT_VAL_array_read( b_ptr,
                                                         h_ptr,
                                                         word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "rgmp_lfield_range_SSF_SF_RCVD_V_get", start_bit, stop_bit, value );
    return value;
}

static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_SSF_SF_RCVD_V_poll( rgmp_buffer_t *b_ptr,
                                                                         rgmp_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_SSF_SF_RCVD_V_poll( rgmp_buffer_t *b_ptr,
                                                                         rgmp_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    UINT32 word_number;
    UINT32 field_ofs;
    UINT32 num_bits;
    UINT32 mask;

    word_number = start_bit / 32;
    field_ofs = start_bit % 32;
    num_bits = stop_bit - start_bit + 1;
    mask = (0xffffffff >> (32-num_bits)) << field_ofs;
    if (stop_bit / 32 != word_number) {
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_V_poll", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_V_set_to_clear", start_bit, stop_bit);
    return rgmp_reg_SSF_SF_RCVD_INT_VAL_array_poll( b_ptr,
                                                    h_ptr,
                                                    word_number,
                                                    mask,
                                                    ( value << field_ofs),
                                                    cmp,
                                                    max_count,
                                                    num_failed_polls,
                                                    delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RGMP_IO_INLINE_H */
