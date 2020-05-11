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
 *     and register accessor functions for the mfec block
 *****************************************************************************/
#ifndef _MFEC_IO_INLINE_H
#define _MFEC_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "mfec_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MFEC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mfec
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
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} mfec_buffer_t;
static INLINE void mfec_buffer_init( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mfec_buffer_init( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mfec_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mfec_buffer_flush( mfec_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mfec_buffer_flush( mfec_buffer_t *b_ptr )
{
    IOLOG( "mfec_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mfec_reg_read( mfec_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_read( mfec_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
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
static INLINE void mfec_reg_write( mfec_buffer_t *b_ptr,
                                   lineotn_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_write( mfec_buffer_t *b_ptr,
                                   lineotn_handle_t *h_ptr,
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

static INLINE void mfec_field_set( mfec_buffer_t *b_ptr,
                                   lineotn_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_set( mfec_buffer_t *b_ptr,
                                   lineotn_handle_t *h_ptr,
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

static INLINE void mfec_action_on_write_field_set( mfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_action_on_write_field_set( mfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
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

static INLINE void mfec_burst_read( mfec_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void mfec_burst_read( mfec_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
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

static INLINE void mfec_burst_write( mfec_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void mfec_burst_write( mfec_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mfec_poll( mfec_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mfec_poll( mfec_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
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
 *  register access functions for mfec
 * ==================================================================================
 */

static INLINE void mfec_reg_MASTER_CONFIG_write( mfec_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_MASTER_CONFIG_write( mfec_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mfec_reg_MASTER_CONFIG_write", value );
    mfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_MASTER_CONFIG,
                    value);
}

static INLINE void mfec_reg_MASTER_CONFIG_field_set( mfec_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_MASTER_CONFIG_field_set( mfec_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mfec_reg_MASTER_CONFIG_field_set", mask, ofs, value );
    mfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_MASTER_CONFIG,
                    mask,
                    PMC_MFEC_REG_MASTER_CONFIG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mfec_reg_MASTER_CONFIG_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_MASTER_CONFIG_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_MFEC_REG_MASTER_CONFIG);

    IOLOG( "%s -> 0x%08x;", "mfec_reg_MASTER_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void mfec_reg_SWIZZLE100G_CONFIG_write( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_SWIZZLE100G_CONFIG_write( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mfec_reg_SWIZZLE100G_CONFIG_write", value );
    mfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_SWIZZLE100G_CONFIG,
                    value);
}

static INLINE void mfec_reg_SWIZZLE100G_CONFIG_field_set( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_SWIZZLE100G_CONFIG_field_set( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mfec_reg_SWIZZLE100G_CONFIG_field_set", mask, ofs, value );
    mfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_SWIZZLE100G_CONFIG,
                    mask,
                    PMC_MFEC_REG_SWIZZLE100G_CONFIG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mfec_reg_SWIZZLE100G_CONFIG_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_SWIZZLE100G_CONFIG_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_MFEC_REG_SWIZZLE100G_CONFIG);

    IOLOG( "%s -> 0x%08x;", "mfec_reg_SWIZZLE100G_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void mfec_reg_DECODER_CONFIG_write( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_DECODER_CONFIG_write( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mfec_reg_DECODER_CONFIG_write", value );
    mfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_DECODER_CONFIG,
                    value);
}

static INLINE void mfec_reg_DECODER_CONFIG_field_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_DECODER_CONFIG_field_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mfec_reg_DECODER_CONFIG_field_set", mask, ofs, value );
    mfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_DECODER_CONFIG,
                    mask,
                    PMC_MFEC_REG_DECODER_CONFIG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mfec_reg_DECODER_CONFIG_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_DECODER_CONFIG_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_MFEC_REG_DECODER_CONFIG);

    IOLOG( "%s -> 0x%08x;", "mfec_reg_DECODER_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void mfec_reg_DECODER_BYPASS_write( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_DECODER_BYPASS_write( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mfec_reg_DECODER_BYPASS_write", value );
    mfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_DECODER_BYPASS,
                    value);
}

static INLINE void mfec_reg_DECODER_BYPASS_field_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_DECODER_BYPASS_field_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mfec_reg_DECODER_BYPASS_field_set", mask, ofs, value );
    mfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_DECODER_BYPASS,
                    mask,
                    PMC_MFEC_REG_DECODER_BYPASS_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mfec_reg_DECODER_BYPASS_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_DECODER_BYPASS_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_MFEC_REG_DECODER_BYPASS);

    IOLOG( "%s -> 0x%08x;", "mfec_reg_DECODER_BYPASS_read", reg_value);
    return reg_value;
}

static INLINE void mfec_reg_ENCODER_CONFIG_write( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_ENCODER_CONFIG_write( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mfec_reg_ENCODER_CONFIG_write", value );
    mfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_ENCODER_CONFIG,
                    value);
}

static INLINE void mfec_reg_ENCODER_CONFIG_field_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_ENCODER_CONFIG_field_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mfec_reg_ENCODER_CONFIG_field_set", mask, ofs, value );
    mfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_ENCODER_CONFIG,
                    mask,
                    PMC_MFEC_REG_ENCODER_CONFIG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mfec_reg_ENCODER_CONFIG_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_ENCODER_CONFIG_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_MFEC_REG_ENCODER_CONFIG);

    IOLOG( "%s -> 0x%08x;", "mfec_reg_ENCODER_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void mfec_reg_ENCODER_BYPASS_write( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_ENCODER_BYPASS_write( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mfec_reg_ENCODER_BYPASS_write", value );
    mfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_ENCODER_BYPASS,
                    value);
}

static INLINE void mfec_reg_ENCODER_BYPASS_field_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_ENCODER_BYPASS_field_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mfec_reg_ENCODER_BYPASS_field_set", mask, ofs, value );
    mfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_ENCODER_BYPASS,
                    mask,
                    PMC_MFEC_REG_ENCODER_BYPASS_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mfec_reg_ENCODER_BYPASS_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_ENCODER_BYPASS_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_MFEC_REG_ENCODER_BYPASS);

    IOLOG( "%s -> 0x%08x;", "mfec_reg_ENCODER_BYPASS_read", reg_value);
    return reg_value;
}

static INLINE void mfec_reg_TOTAL_FEC_STATS_array_burst_write( mfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void mfec_reg_TOTAL_FEC_STATS_array_burst_write( mfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value )
{
    if (len + ofs > 12)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "mfec_reg_TOTAL_FEC_STATS_array_burst_write", ofs, len, 12 );
    IOLOG( "%s of %d words ofs=%d len=%d", "mfec_reg_TOTAL_FEC_STATS_array_burst_write", 12, ofs, len);
    mfec_burst_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MFEC_REG_TOTAL_FEC_STATS(ofs),
                      len,
                      value);

}

static INLINE void mfec_reg_TOTAL_FEC_STATS_array_field_set( mfec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_TOTAL_FEC_STATS_array_field_set( mfec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mfec_reg_TOTAL_FEC_STATS_array_field_set", mask, ofs, value );
    mfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_MFEC_REG_TOTAL_FEC_STATS(N),
                    mask,
                    PMC_MFEC_REG_TOTAL_FEC_STATS_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE void mfec_reg_TOTAL_FEC_STATS_array_burst_read( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void mfec_reg_TOTAL_FEC_STATS_array_burst_read( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value )
{
    if (len + ofs > 12)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "mfec_reg_TOTAL_FEC_STATS_array_burst_read", ofs, len, 12 );
    mfec_burst_read( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_MFEC_REG_TOTAL_FEC_STATS(ofs),
                     len,
                     value);

    IOLOG( "%s of %d words ofs=%d len=%d", "mfec_reg_TOTAL_FEC_STATS_array_burst_read", 12, ofs, len);
}

static INLINE UINT32 mfec_reg_TOTAL_FEC_STATS_array_read( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_TOTAL_FEC_STATS_array_read( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 N )
{
    UINT32 reg_value;

    reg_value = mfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_MFEC_REG_TOTAL_FEC_STATS(N));

    IOLOG( "%s -> 0x%08x;", "mfec_reg_TOTAL_FEC_STATS_array_read", reg_value);
    return reg_value;
}

static INLINE void mfec_reg_FEC_STAT_UPDATE_write( mfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_FEC_STAT_UPDATE_write( mfec_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mfec_reg_FEC_STAT_UPDATE_write", value );
    mfec_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_MFEC_REG_FEC_STAT_UPDATE,
                    value);
}

static INLINE void mfec_reg_FEC_STAT_UPDATE_field_set( mfec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_reg_FEC_STAT_UPDATE_field_set( mfec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mfec_reg_FEC_STAT_UPDATE_field_set", mask, ofs, value );
    mfec_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_STATUS,
                    PMC_MFEC_REG_FEC_STAT_UPDATE,
                    mask,
                    PMC_MFEC_REG_FEC_STAT_UPDATE_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 mfec_reg_FEC_STAT_UPDATE_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_reg_FEC_STAT_UPDATE_read( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mfec_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_STATUS,
                               PMC_MFEC_REG_FEC_STAT_UPDATE);

    IOLOG( "%s -> 0x%08x;", "mfec_reg_FEC_STAT_UPDATE_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mfec_field_MFEC_GLUE_LOWPWR_set( mfec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_MFEC_GLUE_LOWPWR_set( mfec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_MFEC_GLUE_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mfec_field_MFEC_GLUE_LOWPWR_set", value );

    /* (0x00004000 bits 1) field MFEC_GLUE_LOWPWR of register PMC_MFEC_REG_MASTER_CONFIG */
    mfec_reg_MASTER_CONFIG_field_set( b_ptr,
                                      h_ptr,
                                      MFEC_REG_MASTER_CONFIG_BIT_MFEC_GLUE_LOWPWR_MSK,
                                      MFEC_REG_MASTER_CONFIG_BIT_MFEC_GLUE_LOWPWR_OFF,
                                      value);
}

static INLINE UINT32 mfec_field_MFEC_GLUE_LOWPWR_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_MFEC_GLUE_LOWPWR_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004000 bits 1) field MFEC_GLUE_LOWPWR of register PMC_MFEC_REG_MASTER_CONFIG */
    reg_value = mfec_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_MASTER_CONFIG_BIT_MFEC_GLUE_LOWPWR_MSK) >> MFEC_REG_MASTER_CONFIG_BIT_MFEC_GLUE_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_MFEC_GLUE_LOWPWR_get", value );

    return value;
}
static INLINE void mfec_field_RESET_set( mfec_buffer_t *b_ptr,
                                         lineotn_handle_t *h_ptr,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_RESET_set( mfec_buffer_t *b_ptr,
                                         lineotn_handle_t *h_ptr,
                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mfec_field_RESET_set", value );

    /* (0x00004000 bits 0) field RESET of register PMC_MFEC_REG_MASTER_CONFIG */
    mfec_reg_MASTER_CONFIG_field_set( b_ptr,
                                      h_ptr,
                                      MFEC_REG_MASTER_CONFIG_BIT_RESET_MSK,
                                      MFEC_REG_MASTER_CONFIG_BIT_RESET_OFF,
                                      value);
}

static INLINE UINT32 mfec_field_RESET_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_RESET_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004000 bits 0) field RESET of register PMC_MFEC_REG_MASTER_CONFIG */
    reg_value = mfec_reg_MASTER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_MASTER_CONFIG_BIT_RESET_MSK) >> MFEC_REG_MASTER_CONFIG_BIT_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_RESET_get", value );

    return value;
}
static INLINE void mfec_field_SWIZZLE_SEL_100G_40G_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_SWIZZLE_SEL_100G_40G_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_SWIZZLE_SEL_100G_40G_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mfec_field_SWIZZLE_SEL_100G_40G_set", value );

    /* (0x0000400c bits 4:2) bits 0:2 use field SWIZZLE_SEL_100G_40G of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
    mfec_reg_SWIZZLE100G_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_MSK,
                                           MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_OFF,
                                           value);
}

static INLINE UINT32 mfec_field_SWIZZLE_SEL_100G_40G_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_SWIZZLE_SEL_100G_40G_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 4:2) bits 0:2 use field SWIZZLE_SEL_100G_40G of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
    reg_value = mfec_reg_SWIZZLE100G_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_MSK) >> MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_SWIZZLE_SEL_100G_40G_get", value );

    return value;
}
static INLINE void mfec_field_range_SWIZZLE_SEL_100G_40G_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_SWIZZLE_SEL_100G_40G_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_SWIZZLE_SEL_100G_40G_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_SWIZZLE_SEL_100G_40G_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_SWIZZLE_SEL_100G_40G_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000400c bits 4:2) bits 0:2 use field SWIZZLE_SEL_100G_40G of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
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
        /* (0x0000400c bits 4:2) bits 0:2 use field SWIZZLE_SEL_100G_40G of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
        mfec_reg_SWIZZLE100G_CONFIG_field_set( b_ptr,
                                               h_ptr,
                                               subfield_mask << (MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_OFF + subfield_offset),
                                               MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_SWIZZLE_SEL_100G_40G_get( mfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_SWIZZLE_SEL_100G_40G_get( mfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_SWIZZLE_SEL_100G_40G_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_SWIZZLE_SEL_100G_40G_get", stop_bit, 2 );
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
    /* (0x0000400c bits 4:2) bits 0:2 use field SWIZZLE_SEL_100G_40G of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
    reg_value = mfec_reg_SWIZZLE100G_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_MSK)
                  >> MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_MSK, MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_SEL_100G_40G_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_SWIZZLE_SEL_100G_40G_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_SWIZZLE100G_RAM_LOWPWR_set( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_SWIZZLE100G_RAM_LOWPWR_set( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_SWIZZLE100G_RAM_LOWPWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mfec_field_SWIZZLE100G_RAM_LOWPWR_set", value );

    /* (0x0000400c bits 0) field SWIZZLE100G_RAM_LOWPWR of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
    mfec_reg_SWIZZLE100G_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE100G_RAM_LOWPWR_MSK,
                                           MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE100G_RAM_LOWPWR_OFF,
                                           value);
}

static INLINE UINT32 mfec_field_SWIZZLE100G_RAM_LOWPWR_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_SWIZZLE100G_RAM_LOWPWR_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 0) field SWIZZLE100G_RAM_LOWPWR of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
    reg_value = mfec_reg_SWIZZLE100G_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE100G_RAM_LOWPWR_MSK) >> MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE100G_RAM_LOWPWR_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_SWIZZLE100G_RAM_LOWPWR_get", value );

    return value;
}
static INLINE void mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set", value );

    /* (0x00004010 bits 30:27) bits 0:3 use field DECODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_MSK,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 30:27) bits 0:3 use field DECODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_MSK) >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 30:27) bits 0:3 use field DECODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
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
        /* (0x00004010 bits 30:27) bits 0:3 use field DECODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
        mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_OFF + subfield_offset),
                                           MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_get", stop_bit, 3 );
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
    /* (0x00004010 bits 30:27) bits 0:3 use field DECODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_MSK)
                  >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_MSK, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_RSFEC_10G_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set", value );

    /* (0x00004010 bits 26:23) bits 0:3 use field DECODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_MSK,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 26:23) bits 0:3 use field DECODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_MSK) >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 26:23) bits 0:3 use field DECODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
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
        /* (0x00004010 bits 26:23) bits 0:3 use field DECODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
        mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_OFF + subfield_offset),
                                           MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_get", stop_bit, 3 );
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
    /* (0x00004010 bits 26:23) bits 0:3 use field DECODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_MSK)
                  >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_MSK, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_RSFEC_10G_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set", value );

    /* (0x00004010 bits 22:19) bits 0:3 use field DECODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_MSK,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 22:19) bits 0:3 use field DECODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_MSK) >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 22:19) bits 0:3 use field DECODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
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
        /* (0x00004010 bits 22:19) bits 0:3 use field DECODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
        mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_OFF + subfield_offset),
                                           MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_get", stop_bit, 3 );
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
    /* (0x00004010 bits 22:19) bits 0:3 use field DECODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_MSK)
                  >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_MSK, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_RSFEC_10G_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_DECODER_40G_SLICE2_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_40G_SLICE2_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_40G_SLICE2_FEC_SEL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_40G_SLICE2_FEC_SEL_set", value );

    /* (0x00004010 bits 18:14) bits 0:4 use field DECODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_MSK,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_40G_SLICE2_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_40G_SLICE2_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 18:14) bits 0:4 use field DECODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_MSK) >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_40G_SLICE2_FEC_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 18:14) bits 0:4 use field DECODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
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
        /* (0x00004010 bits 18:14) bits 0:4 use field DECODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
        mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_OFF + subfield_offset),
                                           MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_get", stop_bit, 4 );
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
    /* (0x00004010 bits 18:14) bits 0:4 use field DECODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_MSK)
                  >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_MSK, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE2_FEC_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_DECODER_40G_SLICE1_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_40G_SLICE1_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_40G_SLICE1_FEC_SEL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_40G_SLICE1_FEC_SEL_set", value );

    /* (0x00004010 bits 13:9) bits 0:4 use field DECODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_MSK,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_40G_SLICE1_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_40G_SLICE1_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 13:9) bits 0:4 use field DECODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_MSK) >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_40G_SLICE1_FEC_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 13:9) bits 0:4 use field DECODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
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
        /* (0x00004010 bits 13:9) bits 0:4 use field DECODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
        mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_OFF + subfield_offset),
                                           MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_get", stop_bit, 4 );
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
    /* (0x00004010 bits 13:9) bits 0:4 use field DECODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_MSK)
                  >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_MSK, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE1_FEC_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_DECODER_40G_SLICE0_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_40G_SLICE0_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_40G_SLICE0_FEC_SEL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_40G_SLICE0_FEC_SEL_set", value );

    /* (0x00004010 bits 8:4) bits 0:4 use field DECODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_MSK,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_40G_SLICE0_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_40G_SLICE0_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 8:4) bits 0:4 use field DECODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_MSK) >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_40G_SLICE0_FEC_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 8:4) bits 0:4 use field DECODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
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
        /* (0x00004010 bits 8:4) bits 0:4 use field DECODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
        mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_OFF + subfield_offset),
                                           MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_get", stop_bit, 4 );
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
    /* (0x00004010 bits 8:4) bits 0:4 use field DECODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_MSK)
                  >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_MSK, MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_SLICE0_FEC_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_DECODER_100G_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_100G_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_100G_FEC_SEL_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_100G_FEC_SEL_set", value );

    /* (0x00004010 bits 3:1) bits 0:2 use field DECODER_100G_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_MSK,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_100G_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_100G_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 3:1) bits 0:2 use field DECODER_100G_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_MSK) >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_100G_FEC_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_DECODER_100G_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_DECODER_100G_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_100G_FEC_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_100G_FEC_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_100G_FEC_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004010 bits 3:1) bits 0:2 use field DECODER_100G_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
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
        /* (0x00004010 bits 3:1) bits 0:2 use field DECODER_100G_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
        mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_OFF + subfield_offset),
                                           MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_DECODER_100G_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_DECODER_100G_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_100G_FEC_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_100G_FEC_SEL_get", stop_bit, 2 );
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
    /* (0x00004010 bits 3:1) bits 0:2 use field DECODER_100G_FEC_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_MSK)
                  >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_MSK, MFEC_REG_DECODER_CONFIG_BIT_DECODER_100G_FEC_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_100G_FEC_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_DECODER_40G_100G_SEL_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_40G_100G_SEL_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_40G_100G_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_40G_100G_SEL_set", value );

    /* (0x00004010 bits 0) field DECODER_40G_100G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    mfec_reg_DECODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_100G_SEL_MSK,
                                       MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_100G_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_40G_100G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_40G_100G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004010 bits 0) field DECODER_40G_100G_SEL of register PMC_MFEC_REG_DECODER_CONFIG */
    reg_value = mfec_reg_DECODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_100G_SEL_MSK) >> MFEC_REG_DECODER_CONFIG_BIT_DECODER_40G_100G_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_40G_100G_SEL_get", value );

    return value;
}
static INLINE void mfec_field_DECODER_BYPASS_set( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_DECODER_BYPASS_set( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_DECODER_BYPASS_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "mfec_field_DECODER_BYPASS_set", value );

    /* (0x00004014 bits 11:0) bits 0:11 use field DECODER_BYPASS of register PMC_MFEC_REG_DECODER_BYPASS */
    mfec_reg_DECODER_BYPASS_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_MSK,
                                       MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_DECODER_BYPASS_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_DECODER_BYPASS_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004014 bits 11:0) bits 0:11 use field DECODER_BYPASS of register PMC_MFEC_REG_DECODER_BYPASS */
    reg_value = mfec_reg_DECODER_BYPASS_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_MSK) >> MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_DECODER_BYPASS_get", value );

    return value;
}
static INLINE void mfec_field_range_DECODER_BYPASS_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_DECODER_BYPASS_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_BYPASS_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_BYPASS_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_BYPASS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004014 bits 11:0) bits 0:11 use field DECODER_BYPASS of register PMC_MFEC_REG_DECODER_BYPASS */
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
        /* (0x00004014 bits 11:0) bits 0:11 use field DECODER_BYPASS of register PMC_MFEC_REG_DECODER_BYPASS */
        mfec_reg_DECODER_BYPASS_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_OFF + subfield_offset),
                                           MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_DECODER_BYPASS_get( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_DECODER_BYPASS_get( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_DECODER_BYPASS_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_DECODER_BYPASS_get", stop_bit, 11 );
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
    /* (0x00004014 bits 11:0) bits 0:11 use field DECODER_BYPASS of register PMC_MFEC_REG_DECODER_BYPASS */
    reg_value = mfec_reg_DECODER_BYPASS_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_MSK)
                  >> MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_MSK, MFEC_REG_DECODER_BYPASS_BIT_DECODER_BYPASS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_DECODER_BYPASS_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set", value );

    /* (0x00004018 bits 30:27) bits 0:3 use field ENCODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_MSK,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 30:27) bits 0:3 use field ENCODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_MSK) >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 30:27) bits 0:3 use field ENCODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
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
        /* (0x00004018 bits 30:27) bits 0:3 use field ENCODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
        mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_OFF + subfield_offset),
                                           MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get", stop_bit, 3 );
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
    /* (0x00004018 bits 30:27) bits 0:3 use field ENCODER_40G_SLICE2_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_MSK)
                  >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_MSK, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_RSFEC_10G_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set", value );

    /* (0x00004018 bits 26:23) bits 0:3 use field ENCODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_MSK,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 26:23) bits 0:3 use field ENCODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_MSK) >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 26:23) bits 0:3 use field ENCODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
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
        /* (0x00004018 bits 26:23) bits 0:3 use field ENCODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
        mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_OFF + subfield_offset),
                                           MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get", stop_bit, 3 );
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
    /* (0x00004018 bits 26:23) bits 0:3 use field ENCODER_40G_SLICE1_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_MSK)
                  >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_MSK, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_RSFEC_10G_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set", value, 15);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set", value );

    /* (0x00004018 bits 22:19) bits 0:3 use field ENCODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_MSK,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 22:19) bits 0:3 use field ENCODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_MSK) >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set( mfec_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit,
                                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 22:19) bits 0:3 use field ENCODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
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
        /* (0x00004018 bits 22:19) bits 0:3 use field ENCODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
        mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_OFF + subfield_offset),
                                           MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get( mfec_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 start_bit,
                                                                            UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get", stop_bit, 3 );
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
    /* (0x00004018 bits 22:19) bits 0:3 use field ENCODER_40G_SLICE0_RSFEC_10G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_MSK)
                  >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_MSK, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_RSFEC_10G_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set", value );

    /* (0x00004018 bits 18:14) bits 0:4 use field ENCODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_MSK,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 18:14) bits 0:4 use field ENCODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_MSK) >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 18:14) bits 0:4 use field ENCODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
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
        /* (0x00004018 bits 18:14) bits 0:4 use field ENCODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
        mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_OFF + subfield_offset),
                                           MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get", stop_bit, 4 );
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
    /* (0x00004018 bits 18:14) bits 0:4 use field ENCODER_40G_SLICE2_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_MSK)
                  >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_MSK, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE2_FEC_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set", value );

    /* (0x00004018 bits 13:9) bits 0:4 use field ENCODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_MSK,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 13:9) bits 0:4 use field ENCODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_MSK) >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 13:9) bits 0:4 use field ENCODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
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
        /* (0x00004018 bits 13:9) bits 0:4 use field ENCODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
        mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_OFF + subfield_offset),
                                           MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get", stop_bit, 4 );
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
    /* (0x00004018 bits 13:9) bits 0:4 use field ENCODER_40G_SLICE1_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_MSK)
                  >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_MSK, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE1_FEC_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set", value );

    /* (0x00004018 bits 8:4) bits 0:4 use field ENCODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_MSK,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 8:4) bits 0:4 use field ENCODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_MSK) >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 8:4) bits 0:4 use field ENCODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
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
        /* (0x00004018 bits 8:4) bits 0:4 use field ENCODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
        mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_OFF + subfield_offset),
                                           MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get", stop_bit, 4 );
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
    /* (0x00004018 bits 8:4) bits 0:4 use field ENCODER_40G_SLICE0_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_MSK)
                  >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_MSK, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_SLICE0_FEC_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ENCODER_100G_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_100G_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_100G_FEC_SEL_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_100G_FEC_SEL_set", value );

    /* (0x00004018 bits 3:1) bits 0:2 use field ENCODER_100G_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_MSK,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_100G_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_100G_FEC_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 3:1) bits 0:2 use field ENCODER_100G_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_MSK) >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_100G_FEC_SEL_get", value );

    return value;
}
static INLINE void mfec_field_range_ENCODER_100G_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_ENCODER_100G_FEC_SEL_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_100G_FEC_SEL_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_100G_FEC_SEL_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_100G_FEC_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004018 bits 3:1) bits 0:2 use field ENCODER_100G_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
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
        /* (0x00004018 bits 3:1) bits 0:2 use field ENCODER_100G_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
        mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_OFF + subfield_offset),
                                           MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_ENCODER_100G_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_ENCODER_100G_FEC_SEL_get( mfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_100G_FEC_SEL_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_100G_FEC_SEL_get", stop_bit, 2 );
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
    /* (0x00004018 bits 3:1) bits 0:2 use field ENCODER_100G_FEC_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_MSK)
                  >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_MSK, MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_100G_FEC_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_100G_FEC_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ENCODER_40G_100G_SEL_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_40G_100G_SEL_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_40G_100G_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_40G_100G_SEL_set", value );

    /* (0x00004018 bits 0) field ENCODER_40G_100G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    mfec_reg_ENCODER_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_100G_SEL_MSK,
                                       MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_100G_SEL_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_40G_100G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_40G_100G_SEL_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004018 bits 0) field ENCODER_40G_100G_SEL of register PMC_MFEC_REG_ENCODER_CONFIG */
    reg_value = mfec_reg_ENCODER_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_100G_SEL_MSK) >> MFEC_REG_ENCODER_CONFIG_BIT_ENCODER_40G_100G_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_40G_100G_SEL_get", value );

    return value;
}
static INLINE void mfec_field_ENCODER_BYPASS_set( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ENCODER_BYPASS_set( mfec_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 4095)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ENCODER_BYPASS_set", value, 4095);
    IOLOG( "%s <= 0x%08x", "mfec_field_ENCODER_BYPASS_set", value );

    /* (0x0000401c bits 11:0) bits 0:11 use field ENCODER_BYPASS of register PMC_MFEC_REG_ENCODER_BYPASS */
    mfec_reg_ENCODER_BYPASS_field_set( b_ptr,
                                       h_ptr,
                                       MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_MSK,
                                       MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_OFF,
                                       value);
}

static INLINE UINT32 mfec_field_ENCODER_BYPASS_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ENCODER_BYPASS_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000401c bits 11:0) bits 0:11 use field ENCODER_BYPASS of register PMC_MFEC_REG_ENCODER_BYPASS */
    reg_value = mfec_reg_ENCODER_BYPASS_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_MSK) >> MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ENCODER_BYPASS_get", value );

    return value;
}
static INLINE void mfec_field_range_ENCODER_BYPASS_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_ENCODER_BYPASS_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_BYPASS_set", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_BYPASS_set", stop_bit, 11 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_BYPASS_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000401c bits 11:0) bits 0:11 use field ENCODER_BYPASS of register PMC_MFEC_REG_ENCODER_BYPASS */
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
        /* (0x0000401c bits 11:0) bits 0:11 use field ENCODER_BYPASS of register PMC_MFEC_REG_ENCODER_BYPASS */
        mfec_reg_ENCODER_BYPASS_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_OFF + subfield_offset),
                                           MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_ENCODER_BYPASS_get( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_ENCODER_BYPASS_get( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_ENCODER_BYPASS_get", stop_bit, start_bit );
    if (stop_bit > 11) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_ENCODER_BYPASS_get", stop_bit, 11 );
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
    /* (0x0000401c bits 11:0) bits 0:11 use field ENCODER_BYPASS of register PMC_MFEC_REG_ENCODER_BYPASS */
    reg_value = mfec_reg_ENCODER_BYPASS_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_MSK)
                  >> MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_MSK, MFEC_REG_ENCODER_BYPASS_BIT_ENCODER_BYPASS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_ENCODER_BYPASS_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_burstsize12
 * ==================================================================================
 */
static INLINE void mfec_lfield_TOTAL_CORR_ERRS_set( mfec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value[12] ) ALWAYS_INLINE;
static INLINE void mfec_lfield_TOTAL_CORR_ERRS_set( mfec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value[12] )
{
    IOLOG( "%s ", "mfec_lfield_TOTAL_CORR_ERRS_set");
    mfec_reg_TOTAL_FEC_STATS_array_burst_write( b_ptr,
                                                h_ptr,
                                                0,
                                                12,
                                                value);
}

static INLINE void mfec_lfield_TOTAL_CORR_ERRS_get( mfec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value[12] ) ALWAYS_INLINE;
static INLINE void mfec_lfield_TOTAL_CORR_ERRS_get( mfec_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value[12] )
{
    IOLOG( "%s ", "mfec_lfield_TOTAL_CORR_ERRS_get");
    mfec_reg_TOTAL_FEC_STATS_array_burst_read( b_ptr,
                                               h_ptr,
                                               0,
                                               12,
                                               (UINT32 *)value);
}

static INLINE void mfec_lfield_range_TOTAL_CORR_ERRS_set( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_lfield_range_TOTAL_CORR_ERRS_set( mfec_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mfec_lfield_range_TOTAL_CORR_ERRS_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mfec_lfield_range_TOTAL_CORR_ERRS_set", start_bit, stop_bit, value);
    mfec_reg_TOTAL_FEC_STATS_array_field_set( b_ptr,
                                              h_ptr,
                                              word_number,
                                              mask,
                                              field_ofs,
                                              value);
}

static INLINE UINT32 mfec_lfield_range_TOTAL_CORR_ERRS_get( mfec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 mfec_lfield_range_TOTAL_CORR_ERRS_get( mfec_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mfec_lfield_range_TOTAL_CORR_ERRS_get", start_bit, stop_bit );
    }
    reg_value = mfec_reg_TOTAL_FEC_STATS_array_read( b_ptr,
                                                     h_ptr,
                                                     word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "mfec_lfield_range_TOTAL_CORR_ERRS_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void mfec_field_SWIZZLE_ENC_REINIT_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_SWIZZLE_ENC_REINIT_set( mfec_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_SWIZZLE_ENC_REINIT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mfec_field_SWIZZLE_ENC_REINIT_set", value );

    /* (0x0000400c bits 5) field SWIZZLE_ENC_REINIT of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
    mfec_reg_SWIZZLE100G_CONFIG_field_set( b_ptr,
                                           h_ptr,
                                           MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_ENC_REINIT_MSK,
                                           MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_ENC_REINIT_OFF,
                                           value);
}

static INLINE UINT32 mfec_field_SWIZZLE_ENC_REINIT_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_SWIZZLE_ENC_REINIT_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000400c bits 5) field SWIZZLE_ENC_REINIT of register PMC_MFEC_REG_SWIZZLE100G_CONFIG */
    reg_value = mfec_reg_SWIZZLE100G_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_ENC_REINIT_MSK) >> MFEC_REG_SWIZZLE100G_CONFIG_BIT_SWIZZLE_ENC_REINIT_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_SWIZZLE_ENC_REINIT_get", value );

    return value;
}

static INLINE void mfec_field_SWZ_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_SWZ_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_SWZ_FEC_STATS_UPDATE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mfec_field_SWZ_FEC_STATS_UPDATE_set", value );

    /* (0x00004020 bits 12:10) bits 0:2 use field SWZ_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                        h_ptr,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_MSK,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_OFF,
                                        value);
}

static INLINE UINT32 mfec_field_SWZ_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_SWZ_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004020 bits 12:10) bits 0:2 use field SWZ_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_MSK) >> MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_SWZ_FEC_STATS_UPDATE_get", value );

    return value;
}
static INLINE void mfec_field_range_SWZ_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_SWZ_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_SWZ_FEC_STATS_UPDATE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_SWZ_FEC_STATS_UPDATE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_SWZ_FEC_STATS_UPDATE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004020 bits 12:10) bits 0:2 use field SWZ_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
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
        /* (0x00004020 bits 12:10) bits 0:2 use field SWZ_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
        mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_OFF + subfield_offset),
                                            MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_SWZ_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_SWZ_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_SWZ_FEC_STATS_UPDATE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_SWZ_FEC_STATS_UPDATE_get", stop_bit, 2 );
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
    /* (0x00004020 bits 12:10) bits 0:2 use field SWZ_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_MSK)
                  >> MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_MSK, MFEC_REG_FEC_STAT_UPDATE_BIT_SWZ_FEC_STATS_UPDATE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_SWZ_FEC_STATS_UPDATE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_RS_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_RS_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_RS_FEC_STATS_UPDATE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mfec_field_RS_FEC_STATS_UPDATE_set", value );

    /* (0x00004020 bits 9:7) bits 0:2 use field RS_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                        h_ptr,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_MSK,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF,
                                        value);
}

static INLINE UINT32 mfec_field_RS_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_RS_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004020 bits 9:7) bits 0:2 use field RS_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_MSK) >> MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_RS_FEC_STATS_UPDATE_get", value );

    return value;
}
static INLINE void mfec_field_range_RS_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_RS_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_RS_FEC_STATS_UPDATE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_RS_FEC_STATS_UPDATE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_RS_FEC_STATS_UPDATE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004020 bits 9:7) bits 0:2 use field RS_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
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
        /* (0x00004020 bits 9:7) bits 0:2 use field RS_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
        mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF + subfield_offset),
                                            MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_RS_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_RS_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_RS_FEC_STATS_UPDATE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_RS_FEC_STATS_UPDATE_get", stop_bit, 2 );
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
    /* (0x00004020 bits 9:7) bits 0:2 use field RS_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_MSK)
                  >> MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_MSK, MFEC_REG_FEC_STAT_UPDATE_BIT_RS_FEC_STATS_UPDATE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_RS_FEC_STATS_UPDATE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_I7_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_I7_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_I7_FEC_STATS_UPDATE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mfec_field_I7_FEC_STATS_UPDATE_set", value );

    /* (0x00004020 bits 6:4) bits 0:2 use field I7_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                        h_ptr,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_MSK,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_OFF,
                                        value);
}

static INLINE UINT32 mfec_field_I7_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_I7_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004020 bits 6:4) bits 0:2 use field I7_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_MSK) >> MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_I7_FEC_STATS_UPDATE_get", value );

    return value;
}
static INLINE void mfec_field_range_I7_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_I7_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_I7_FEC_STATS_UPDATE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_I7_FEC_STATS_UPDATE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_I7_FEC_STATS_UPDATE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004020 bits 6:4) bits 0:2 use field I7_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
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
        /* (0x00004020 bits 6:4) bits 0:2 use field I7_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
        mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_OFF + subfield_offset),
                                            MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_I7_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_I7_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_I7_FEC_STATS_UPDATE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_I7_FEC_STATS_UPDATE_get", stop_bit, 2 );
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
    /* (0x00004020 bits 6:4) bits 0:2 use field I7_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_MSK)
                  >> MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_MSK, MFEC_REG_FEC_STAT_UPDATE_BIT_I7_FEC_STATS_UPDATE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_I7_FEC_STATS_UPDATE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_I4_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_I4_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_I4_FEC_STATS_UPDATE_set", value, 7);
    IOLOG( "%s <= 0x%08x", "mfec_field_I4_FEC_STATS_UPDATE_set", value );

    /* (0x00004020 bits 3:1) bits 0:2 use field I4_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                        h_ptr,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_MSK,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_OFF,
                                        value);
}

static INLINE UINT32 mfec_field_I4_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_I4_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004020 bits 3:1) bits 0:2 use field I4_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_MSK) >> MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_I4_FEC_STATS_UPDATE_get", value );

    return value;
}
static INLINE void mfec_field_range_I4_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_range_I4_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_I4_FEC_STATS_UPDATE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_I4_FEC_STATS_UPDATE_set", stop_bit, 2 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_I4_FEC_STATS_UPDATE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00004020 bits 3:1) bits 0:2 use field I4_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
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
        /* (0x00004020 bits 3:1) bits 0:2 use field I4_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
        mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                            h_ptr,
                                            subfield_mask << (MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_OFF + subfield_offset),
                                            MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 mfec_field_range_I4_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_range_I4_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mfec_field_range_I4_FEC_STATS_UPDATE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mfec_field_range_I4_FEC_STATS_UPDATE_get", stop_bit, 2 );
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
    /* (0x00004020 bits 3:1) bits 0:2 use field I4_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    field_value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_MSK)
                  >> MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_MSK, MFEC_REG_FEC_STAT_UPDATE_BIT_I4_FEC_STATS_UPDATE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mfec_field_range_I4_FEC_STATS_UPDATE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mfec_field_ALL_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void mfec_field_ALL_FEC_STATS_UPDATE_set( mfec_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mfec_field_ALL_FEC_STATS_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mfec_field_ALL_FEC_STATS_UPDATE_set", value );

    /* (0x00004020 bits 0) field ALL_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    mfec_reg_FEC_STAT_UPDATE_field_set( b_ptr,
                                        h_ptr,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_ALL_FEC_STATS_UPDATE_MSK,
                                        MFEC_REG_FEC_STAT_UPDATE_BIT_ALL_FEC_STATS_UPDATE_OFF,
                                        value);
}

static INLINE UINT32 mfec_field_ALL_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mfec_field_ALL_FEC_STATS_UPDATE_get( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00004020 bits 0) field ALL_FEC_STATS_UPDATE of register PMC_MFEC_REG_FEC_STAT_UPDATE */
    reg_value = mfec_reg_FEC_STAT_UPDATE_read(  b_ptr, h_ptr);
    value = (reg_value & MFEC_REG_FEC_STAT_UPDATE_BIT_ALL_FEC_STATS_UPDATE_MSK) >> MFEC_REG_FEC_STAT_UPDATE_BIT_ALL_FEC_STATS_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "mfec_field_ALL_FEC_STATS_UPDATE_get", value );

    return value;
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MFEC_IO_INLINE_H */
