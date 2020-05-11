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
 *     and register accessor functions for the mstatx block
 *****************************************************************************/
#ifndef _MSTATX_IO_INLINE_H
#define _MSTATX_IO_INLINE_H

#include "enet_fege_loc.h"
#include "mstatx_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define MSTATX_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for mstatx
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
    enet_fege_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} mstatx_buffer_t;
static INLINE void mstatx_buffer_init( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void mstatx_buffer_init( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "mstatx_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void mstatx_buffer_flush( mstatx_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void mstatx_buffer_flush( mstatx_buffer_t *b_ptr )
{
    IOLOG( "mstatx_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 mstatx_reg_read( mstatx_buffer_t *b_ptr,
                                      enet_fege_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_read( mstatx_buffer_t *b_ptr,
                                      enet_fege_handle_t *h_ptr,
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
static INLINE void mstatx_reg_write( mstatx_buffer_t *b_ptr,
                                     enet_fege_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_write( mstatx_buffer_t *b_ptr,
                                     enet_fege_handle_t *h_ptr,
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

static INLINE void mstatx_field_set( mstatx_buffer_t *b_ptr,
                                     enet_fege_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_set( mstatx_buffer_t *b_ptr,
                                     enet_fege_handle_t *h_ptr,
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

static INLINE void mstatx_action_on_write_field_set( mstatx_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_action_on_write_field_set( mstatx_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
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

static INLINE void mstatx_burst_read( mstatx_buffer_t *b_ptr,
                                      enet_fege_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void mstatx_burst_read( mstatx_buffer_t *b_ptr,
                                      enet_fege_handle_t *h_ptr,
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

static INLINE void mstatx_burst_write( mstatx_buffer_t *b_ptr,
                                       enet_fege_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void mstatx_burst_write( mstatx_buffer_t *b_ptr,
                                       enet_fege_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE mstatx_poll( mstatx_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE mstatx_poll( mstatx_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
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
 *  register access functions for mstatx
 * ==================================================================================
 */

static INLINE void mstatx_reg_INT_1_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_1_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_1_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_1,
                      value);
}

static INLINE void mstatx_reg_INT_1_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_1_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_1_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_1,
                      mask,
                      PMC_MSTATX_REG_INT_1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_1_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_1_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_1);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_1_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_2_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_2_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_2_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_2,
                      value);
}

static INLINE void mstatx_reg_INT_2_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_2_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_2_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_2,
                      mask,
                      PMC_MSTATX_REG_INT_2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_2_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_2_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_2);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_2_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_3_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_3_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_3_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_3,
                      value);
}

static INLINE void mstatx_reg_INT_3_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_3_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_3_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_3,
                      mask,
                      PMC_MSTATX_REG_INT_3_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_3_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_3_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_3);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_3_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_4_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_4_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_4_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_4,
                      value);
}

static INLINE void mstatx_reg_INT_4_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_4_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_4_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_4,
                      mask,
                      PMC_MSTATX_REG_INT_4_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_4_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_4_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_4);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_4_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_5_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_5_write( mstatx_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_5_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_5,
                      value);
}

static INLINE void mstatx_reg_INT_5_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_5_field_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 mask,
                                               UINT32 ofs,
                                               UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_5_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_5,
                      mask,
                      PMC_MSTATX_REG_INT_5_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_5_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_5_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_5);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_5_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_EN_1_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_1_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_EN_1_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_1,
                      value);
}

static INLINE void mstatx_reg_INT_EN_1_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_1_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_EN_1_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_1,
                      mask,
                      PMC_MSTATX_REG_INT_EN_1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_EN_1_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_EN_1_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_EN_1);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_EN_1_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_EN_2_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_2_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_EN_2_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_2,
                      value);
}

static INLINE void mstatx_reg_INT_EN_2_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_2_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_EN_2_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_2,
                      mask,
                      PMC_MSTATX_REG_INT_EN_2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_EN_2_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_EN_2_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_EN_2);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_EN_2_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_EN_3_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_3_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_EN_3_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_3,
                      value);
}

static INLINE void mstatx_reg_INT_EN_3_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_3_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_EN_3_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_3,
                      mask,
                      PMC_MSTATX_REG_INT_EN_3_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_EN_3_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_EN_3_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_EN_3);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_EN_3_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_EN_4_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_4_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_EN_4_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_4,
                      value);
}

static INLINE void mstatx_reg_INT_EN_4_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_4_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_EN_4_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_4,
                      mask,
                      PMC_MSTATX_REG_INT_EN_4_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_EN_4_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_EN_4_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_EN_4);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_EN_4_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_INT_EN_5_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_5_write( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_INT_EN_5_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_5,
                      value);
}

static INLINE void mstatx_reg_INT_EN_5_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_INT_EN_5_field_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_INT_EN_5_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_INT_EN_5,
                      mask,
                      PMC_MSTATX_REG_INT_EN_5_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_INT_EN_5_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_INT_EN_5_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_INT_EN_5);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_INT_EN_5_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_CTRL_write( mstatx_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_CTRL_write( mstatx_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_CTRL_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_CTRL,
                      value);
}

static INLINE void mstatx_reg_CTRL_field_set( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_CTRL_field_set( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 mask,
                                              UINT32 ofs,
                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_CTRL_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_CTRL,
                      mask,
                      PMC_MSTATX_REG_CTRL_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_CTRL_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_CTRL_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_CTRL);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void mstatx_reg_VARIABLE_SELECT_write( mstatx_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_VARIABLE_SELECT_write( mstatx_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "mstatx_reg_VARIABLE_SELECT_write", value );
    mstatx_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_VARIABLE_SELECT,
                      value);
}

static INLINE void mstatx_reg_VARIABLE_SELECT_field_set( mstatx_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_reg_VARIABLE_SELECT_field_set( mstatx_buffer_t *b_ptr,
                                                         enet_fege_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "mstatx_reg_VARIABLE_SELECT_field_set", mask, ofs, value );
    mstatx_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_MSTATX_REG_VARIABLE_SELECT,
                      mask,
                      PMC_MSTATX_REG_VARIABLE_SELECT_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 mstatx_reg_VARIABLE_SELECT_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_VARIABLE_SELECT_read( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_MSTATX_REG_VARIABLE_SELECT);

    IOLOG( "%s -> 0x%08x;", "mstatx_reg_VARIABLE_SELECT_read", reg_value);
    return reg_value;
}

static INLINE UINT32 mstatx_reg_STATISTICAL_CNT_LOW_array_read( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_STATISTICAL_CNT_LOW_array_read( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_MSTATX_REG_STATISTICAL_CNT_LOW(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mstatx_reg_STATISTICAL_CNT_LOW_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mstatx_reg_STATISTICAL_CNT_MID_array_read( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_STATISTICAL_CNT_MID_array_read( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_MSTATX_REG_STATISTICAL_CNT_MID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mstatx_reg_STATISTICAL_CNT_MID_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 mstatx_reg_STATISTICAL_CNT_HIGH_array_read( mstatx_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_reg_STATISTICAL_CNT_HIGH_array_read( mstatx_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = mstatx_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_MSTATX_REG_STATISTICAL_CNT_HIGH(N));

    IOLOG( "%s -> 0x%08x; N=%d", "mstatx_reg_STATISTICAL_CNT_HIGH_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void mstatx_field_RX_LONG_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_LONG_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_LONG_LENGTH_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_LONG_LENGTH_ERROR_I_set", value );

    /* (0x00001000 bits 15) field RX_LONG_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_LONG_LENGTH_ERROR_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_LONG_LENGTH_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_LONG_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_LONG_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 15) field RX_LONG_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_LONG_LENGTH_ERROR_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_LONG_LENGTH_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_LONG_LENGTH_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_set", value );

    /* (0x00001000 bits 14) field RX_IN_RANGE_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_IN_RANGE_LENGTH_ERROR_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_IN_RANGE_LENGTH_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 14) field RX_IN_RANGE_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_IN_RANGE_LENGTH_ERROR_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_IN_RANGE_LENGTH_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_LAYER1_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_LAYER1_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_LAYER1_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_LAYER1_ERROR_I_set", value );

    /* (0x00001000 bits 13) field RX_LAYER1_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_LAYER1_ERROR_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_LAYER1_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_LAYER1_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_LAYER1_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 13) field RX_LAYER1_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_LAYER1_ERROR_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_LAYER1_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_LAYER1_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_SYMBOL_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_SYMBOL_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_SYMBOL_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_SYMBOL_ERROR_I_set", value );

    /* (0x00001000 bits 12) field RX_SYMBOL_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_SYMBOL_ERROR_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_SYMBOL_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_SYMBOL_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_SYMBOL_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 12) field RX_SYMBOL_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_SYMBOL_ERROR_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_SYMBOL_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_SYMBOL_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FCS_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FCS_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FCS_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FCS_ERROR_I_set", value );

    /* (0x00001000 bits 10) field RX_FCS_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_FCS_ERROR_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_FCS_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FCS_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FCS_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 10) field RX_FCS_ERROR_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_FCS_ERROR_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_FCS_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FCS_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_set", value );

    /* (0x00001000 bits 9) field RX_MAC_CONTROL_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_MAC_CONTROL_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_MAC_CONTROL_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 9) field RX_MAC_CONTROL_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_MAC_CONTROL_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_MAC_CONTROL_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set", value );

    /* (0x00001000 bits 8) field RX_MAC_PAUSE_CONTROL_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 8) field RX_MAC_PAUSE_CONTROL_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_TAGGED_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_TAGGED_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_TAGGED_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_TAGGED_FRAMES_OK_I_set", value );

    /* (0x00001000 bits 7) field RX_TAGGED_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_TAGGED_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_TAGGED_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_TAGGED_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_TAGGED_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 7) field RX_TAGGED_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_TAGGED_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_TAGGED_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_TAGGED_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_MULTICAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_MULTICAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_MULTICAST_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_MULTICAST_FRAMES_OK_I_set", value );

    /* (0x00001000 bits 6) field RX_MULTICAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_MULTICAST_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_MULTICAST_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_MULTICAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_MULTICAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 6) field RX_MULTICAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_MULTICAST_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_MULTICAST_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_MULTICAST_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_BROADCAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_BROADCAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_BROADCAST_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_BROADCAST_FRAMES_OK_I_set", value );

    /* (0x00001000 bits 5) field RX_BROADCAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_BROADCAST_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_BROADCAST_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_BROADCAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_BROADCAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 5) field RX_BROADCAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_BROADCAST_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_BROADCAST_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_BROADCAST_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_UNICAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_UNICAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_UNICAST_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_UNICAST_FRAMES_OK_I_set", value );

    /* (0x00001000 bits 4) field RX_UNICAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_UNICAST_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_UNICAST_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_UNICAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_UNICAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 4) field RX_UNICAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_UNICAST_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_UNICAST_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_UNICAST_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_OCTETS_I_set", value );

    /* (0x00001000 bits 3) field RX_OCTETS_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_OCTETS_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 3) field RX_OCTETS_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_OCTETS_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_I_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_I_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_I_set", value );

    /* (0x00001000 bits 2) field RX_FRAMES_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_FRAMES_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_FRAMES_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 2) field RX_FRAMES_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_FRAMES_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_FRAMES_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_OCTETS_OK_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_OCTETS_OK_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_OCTETS_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_OCTETS_OK_I_set", value );

    /* (0x00001000 bits 1) field RX_OCTETS_OK_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_OCTETS_OK_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_OCTETS_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_OCTETS_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_OCTETS_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 1) field RX_OCTETS_OK_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_OCTETS_OK_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_OCTETS_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_OCTETS_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_OK_I_set", value );

    /* (0x00001000 bits 0) field RX_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    mstatx_reg_INT_1_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_1_BIT_RX_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_1_BIT_RX_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 0) field RX_FRAMES_OK_I of register PMC_MSTATX_REG_INT_1 */
    reg_value = mstatx_reg_INT_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_1_BIT_RX_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_1_BIT_RX_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_VARIABLE_I_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_VARIABLE_I_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_VARIABLE_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_VARIABLE_I_set", value );

    /* (0x00001004 bits 11) field RX_VARIABLE_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_VARIABLE_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_VARIABLE_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_VARIABLE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_VARIABLE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 11) field RX_VARIABLE_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_VARIABLE_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_VARIABLE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_VARIABLE_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_STATION_ADDRESS_FILTERED_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_STATION_ADDRESS_FILTERED_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_STATION_ADDRESS_FILTERED_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_STATION_ADDRESS_FILTERED_I_set", value );

    /* (0x00001004 bits 10) field RX_STATION_ADDRESS_FILTERED_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_STATION_ADDRESS_FILTERED_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_STATION_ADDRESS_FILTERED_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_STATION_ADDRESS_FILTERED_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_STATION_ADDRESS_FILTERED_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 10) field RX_STATION_ADDRESS_FILTERED_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_STATION_ADDRESS_FILTERED_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_STATION_ADDRESS_FILTERED_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_STATION_ADDRESS_FILTERED_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_set", value );

    /* (0x00001004 bits 9) field RX_FRAMES_1519_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_1519_TO_MAX_SIZE_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_1519_TO_MAX_SIZE_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 9) field RX_FRAMES_1519_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_FRAMES_1519_TO_MAX_SIZE_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_FRAMES_1519_TO_MAX_SIZE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_set", value );

    /* (0x00001004 bits 8) field RX_FRAMES_1024_TO_1518_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_1024_TO_1518_OCTETS_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_1024_TO_1518_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 8) field RX_FRAMES_1024_TO_1518_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_FRAMES_1024_TO_1518_OCTETS_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_FRAMES_1024_TO_1518_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_set", value );

    /* (0x00001004 bits 7) field RX_FRAMES_512_TO_1023_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_512_TO_1023_OCTETS_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_512_TO_1023_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 7) field RX_FRAMES_512_TO_1023_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_FRAMES_512_TO_1023_OCTETS_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_FRAMES_512_TO_1023_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_set", value );

    /* (0x00001004 bits 6) field RX_FRAMES_256_TO_511_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_256_TO_511_OCTETS_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_256_TO_511_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 6) field RX_FRAMES_256_TO_511_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_FRAMES_256_TO_511_OCTETS_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_FRAMES_256_TO_511_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_set", value );

    /* (0x00001004 bits 5) field RX_FRAMES_128_TO_255_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_128_TO_255_OCTETS_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_128_TO_255_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 5) field RX_FRAMES_128_TO_255_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_FRAMES_128_TO_255_OCTETS_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_FRAMES_128_TO_255_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_set", value );

    /* (0x00001004 bits 4) field RX_FRAMES_65_TO_127_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_65_TO_127_OCTETS_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_65_TO_127_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 4) field RX_FRAMES_65_TO_127_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_FRAMES_65_TO_127_OCTETS_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_FRAMES_65_TO_127_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_64_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_64_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_64_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_64_OCTETS_I_set", value );

    /* (0x00001004 bits 3) field RX_FRAMES_64_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_64_OCTETS_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_FRAMES_64_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_64_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_64_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 3) field RX_FRAMES_64_OCTETS_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_FRAMES_64_OCTETS_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_FRAMES_64_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_64_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_set", value );

    /* (0x00001004 bits 2) field RX_SHORT_LENGTH_CRC_ERROR_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_SHORT_LENGTH_CRC_ERROR_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_SHORT_LENGTH_CRC_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 2) field RX_SHORT_LENGTH_CRC_ERROR_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_SHORT_LENGTH_CRC_ERROR_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_SHORT_LENGTH_CRC_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_SHORT_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_SHORT_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_SHORT_LENGTH_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_SHORT_LENGTH_ERROR_I_set", value );

    /* (0x00001004 bits 1) field RX_SHORT_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_SHORT_LENGTH_ERROR_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_SHORT_LENGTH_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_SHORT_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_SHORT_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 1) field RX_SHORT_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_SHORT_LENGTH_ERROR_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_SHORT_LENGTH_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_SHORT_LENGTH_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_set", value );

    /* (0x00001004 bits 0) field RX_LONG_LENGTH_CRC_ERROR_I of register PMC_MSTATX_REG_INT_2 */
    mstatx_reg_INT_2_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_2_BIT_RX_LONG_LENGTH_CRC_ERROR_I_MSK,
                                MSTATX_REG_INT_2_BIT_RX_LONG_LENGTH_CRC_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 0) field RX_LONG_LENGTH_CRC_ERROR_I of register PMC_MSTATX_REG_INT_2 */
    reg_value = mstatx_reg_INT_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_2_BIT_RX_LONG_LENGTH_CRC_ERROR_I_MSK) >> MSTATX_REG_INT_2_BIT_RX_LONG_LENGTH_CRC_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_set", value );

    /* (0x00001008 bits 4) field RX_FRAMES_2501_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_3 */
    mstatx_reg_INT_3_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_2501_TO_MAX_SIZE_I_MSK,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_2501_TO_MAX_SIZE_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001008 bits 4) field RX_FRAMES_2501_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_3 */
    reg_value = mstatx_reg_INT_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_3_BIT_RX_FRAMES_2501_TO_MAX_SIZE_I_MSK) >> MSTATX_REG_INT_3_BIT_RX_FRAMES_2501_TO_MAX_SIZE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_set", value );

    /* (0x00001008 bits 3) field RX_FRAMES_2001_TO_2500_OCTETS_I of register PMC_MSTATX_REG_INT_3 */
    mstatx_reg_INT_3_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_2001_TO_2500_OCTETS_I_MSK,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_2001_TO_2500_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001008 bits 3) field RX_FRAMES_2001_TO_2500_OCTETS_I of register PMC_MSTATX_REG_INT_3 */
    reg_value = mstatx_reg_INT_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_3_BIT_RX_FRAMES_2001_TO_2500_OCTETS_I_MSK) >> MSTATX_REG_INT_3_BIT_RX_FRAMES_2001_TO_2500_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_set", value );

    /* (0x00001008 bits 2) field RX_FRAMES_1549_TO_2000_OCTETS_I of register PMC_MSTATX_REG_INT_3 */
    mstatx_reg_INT_3_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_1549_TO_2000_OCTETS_I_MSK,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_1549_TO_2000_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001008 bits 2) field RX_FRAMES_1549_TO_2000_OCTETS_I of register PMC_MSTATX_REG_INT_3 */
    reg_value = mstatx_reg_INT_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_3_BIT_RX_FRAMES_1549_TO_2000_OCTETS_I_MSK) >> MSTATX_REG_INT_3_BIT_RX_FRAMES_1549_TO_2000_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_set", value );

    /* (0x00001008 bits 1) field RX_FRAMES_1523_TO_1548_OCTETS_I of register PMC_MSTATX_REG_INT_3 */
    mstatx_reg_INT_3_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_1523_TO_1548_OCTETS_I_MSK,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_1523_TO_1548_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001008 bits 1) field RX_FRAMES_1523_TO_1548_OCTETS_I of register PMC_MSTATX_REG_INT_3 */
    reg_value = mstatx_reg_INT_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_3_BIT_RX_FRAMES_1523_TO_1548_OCTETS_I_MSK) >> MSTATX_REG_INT_3_BIT_RX_FRAMES_1523_TO_1548_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_set", value );

    /* (0x00001008 bits 0) field RX_FRAMES_1519_TO_1522_OCTETS_I of register PMC_MSTATX_REG_INT_3 */
    mstatx_reg_INT_3_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_1519_TO_1522_OCTETS_I_MSK,
                                MSTATX_REG_INT_3_BIT_RX_FRAMES_1519_TO_1522_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001008 bits 0) field RX_FRAMES_1519_TO_1522_OCTETS_I of register PMC_MSTATX_REG_INT_3 */
    reg_value = mstatx_reg_INT_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_3_BIT_RX_FRAMES_1519_TO_1522_OCTETS_I_MSK) >> MSTATX_REG_INT_3_BIT_RX_FRAMES_1519_TO_1522_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_set", value );

    /* (0x0000100c bits 15) field TX_UNICAST_FRAMES_ATTEMPTED_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_UNICAST_FRAMES_ATTEMPTED_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_UNICAST_FRAMES_ATTEMPTED_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 15) field TX_UNICAST_FRAMES_ATTEMPTED_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_UNICAST_FRAMES_ATTEMPTED_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_UNICAST_FRAMES_ATTEMPTED_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MULTICAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MULTICAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_OK_I_set", value );

    /* (0x0000100c bits 14) field TX_MULTICAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_MULTICAST_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_MULTICAST_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_MULTICAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MULTICAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 14) field TX_MULTICAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_MULTICAST_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_MULTICAST_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_BROADCAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_BROADCAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_OK_I_set", value );

    /* (0x0000100c bits 13) field TX_BROADCAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_BROADCAST_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_BROADCAST_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_BROADCAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_BROADCAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 13) field TX_BROADCAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_BROADCAST_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_BROADCAST_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_UNICAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_UNICAST_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_OK_I_set", value );

    /* (0x0000100c bits 12) field TX_UNICAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_UNICAST_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_UNICAST_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_UNICAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_UNICAST_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 12) field TX_UNICAST_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_UNICAST_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_UNICAST_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_set", value );

    /* (0x0000100c bits 11) field TX_MAC_CONTROL_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_MAC_CONTROL_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_MAC_CONTROL_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 11) field TX_MAC_CONTROL_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_MAC_CONTROL_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_MAC_CONTROL_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_set", value );

    /* (0x0000100c bits 10) field TX_FRAMES_1523_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_FRAMES_1523_TO_MAX_SIZE_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_FRAMES_1523_TO_MAX_SIZE_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 10) field TX_FRAMES_1523_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_FRAMES_1523_TO_MAX_SIZE_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_FRAMES_1523_TO_MAX_SIZE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MAC_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MAC_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MAC_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MAC_ERROR_I_set", value );

    /* (0x0000100c bits 9) field TX_MAC_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_MAC_ERROR_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_MAC_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_MAC_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MAC_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 9) field TX_MAC_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_MAC_ERROR_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_MAC_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MAC_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_SYSTEM_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_SYSTEM_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_SYSTEM_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_SYSTEM_ERROR_I_set", value );

    /* (0x0000100c bits 8) field TX_SYSTEM_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_SYSTEM_ERROR_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_SYSTEM_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_SYSTEM_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_SYSTEM_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 8) field TX_SYSTEM_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_SYSTEM_ERROR_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_SYSTEM_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_SYSTEM_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_LONG_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_LONG_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_LONG_LENGTH_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_LONG_LENGTH_ERROR_I_set", value );

    /* (0x0000100c bits 7) field TX_LONG_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_LONG_LENGTH_ERROR_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_LONG_LENGTH_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_LONG_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_LONG_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 7) field TX_LONG_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_LONG_LENGTH_ERROR_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_LONG_LENGTH_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_LONG_LENGTH_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_SHORT_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_SHORT_LENGTH_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_SHORT_LENGTH_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_SHORT_LENGTH_ERROR_I_set", value );

    /* (0x0000100c bits 6) field TX_SHORT_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_SHORT_LENGTH_ERROR_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_SHORT_LENGTH_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_SHORT_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_SHORT_LENGTH_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 6) field TX_SHORT_LENGTH_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_SHORT_LENGTH_ERROR_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_SHORT_LENGTH_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_SHORT_LENGTH_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FCS_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FCS_ERROR_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FCS_ERROR_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FCS_ERROR_I_set", value );

    /* (0x0000100c bits 5) field TX_FCS_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_FCS_ERROR_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_FCS_ERROR_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FCS_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FCS_ERROR_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 5) field TX_FCS_ERROR_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_FCS_ERROR_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_FCS_ERROR_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FCS_ERROR_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set", value );

    /* (0x0000100c bits 4) field TX_MAC_PAUSE_CONTROL_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 4) field TX_MAC_PAUSE_CONTROL_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_TAGGED_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_TAGGED_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_TAGGED_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_TAGGED_FRAMES_OK_I_set", value );

    /* (0x0000100c bits 3) field TX_TAGGED_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_TAGGED_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_TAGGED_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_TAGGED_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_TAGGED_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 3) field TX_TAGGED_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_TAGGED_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_TAGGED_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_TAGGED_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_OCTETS_I_set", value );

    /* (0x0000100c bits 2) field TX_OCTETS_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_OCTETS_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 2) field TX_OCTETS_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_OCTETS_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_OCTETS_OK_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_OCTETS_OK_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_OCTETS_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_OCTETS_OK_I_set", value );

    /* (0x0000100c bits 1) field TX_OCTETS_OK_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_OCTETS_OK_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_OCTETS_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_OCTETS_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_OCTETS_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 1) field TX_OCTETS_OK_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_OCTETS_OK_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_OCTETS_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_OCTETS_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_OK_I_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_OK_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_OK_I_set", value );

    /* (0x0000100c bits 0) field TX_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    mstatx_reg_INT_4_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_4_BIT_TX_FRAMES_OK_I_MSK,
                                MSTATX_REG_INT_4_BIT_TX_FRAMES_OK_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_OK_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 0) field TX_FRAMES_OK_I of register PMC_MSTATX_REG_INT_4 */
    reg_value = mstatx_reg_INT_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_4_BIT_TX_FRAMES_OK_I_MSK) >> MSTATX_REG_INT_4_BIT_TX_FRAMES_OK_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_OK_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_set", value );

    /* (0x00001010 bits 15) field TX_FRAMES_2501_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_2501_TO_MAX_SIZE_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_2501_TO_MAX_SIZE_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 15) field TX_FRAMES_2501_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_2501_TO_MAX_SIZE_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_2501_TO_MAX_SIZE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_set", value );

    /* (0x00001010 bits 14) field TX_FRAMES_2001_TO_2500_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_2001_TO_2500_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_2001_TO_2500_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 14) field TX_FRAMES_2001_TO_2500_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_2001_TO_2500_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_2001_TO_2500_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_set", value );

    /* (0x00001010 bits 13) field TX_FRAMES_1549_TO_2000_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1549_TO_2000_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1549_TO_2000_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 13) field TX_FRAMES_1549_TO_2000_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_1549_TO_2000_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_1549_TO_2000_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_set", value );

    /* (0x00001010 bits 12) field TX_FRAMES_1523_TO_1548_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1523_TO_1548_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1523_TO_1548_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 12) field TX_FRAMES_1523_TO_1548_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_1523_TO_1548_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_1523_TO_1548_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_set", value );

    /* (0x00001010 bits 11) field TX_FRAMES_1519_TO_1522_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1519_TO_1522_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1519_TO_1522_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 11) field TX_FRAMES_1519_TO_1522_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_1519_TO_1522_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_1519_TO_1522_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_set", value );

    /* (0x00001010 bits 10) field RX_FRAMES_1523_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_RX_FRAMES_1523_TO_MAX_SIZE_I_MSK,
                                MSTATX_REG_INT_5_BIT_RX_FRAMES_1523_TO_MAX_SIZE_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 10) field RX_FRAMES_1523_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_RX_FRAMES_1523_TO_MAX_SIZE_I_MSK) >> MSTATX_REG_INT_5_BIT_RX_FRAMES_1523_TO_MAX_SIZE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_VARIABLE_I_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_VARIABLE_I_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_VARIABLE_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_VARIABLE_I_set", value );

    /* (0x00001010 bits 9) field TX_VARIABLE_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_VARIABLE_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_VARIABLE_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_VARIABLE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_VARIABLE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 9) field TX_VARIABLE_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_VARIABLE_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_VARIABLE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_VARIABLE_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_set", value );

    /* (0x00001010 bits 8) field TX_FRAMES_1519_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1519_TO_MAX_SIZE_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1519_TO_MAX_SIZE_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 8) field TX_FRAMES_1519_TO_MAX_SIZE_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_1519_TO_MAX_SIZE_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_1519_TO_MAX_SIZE_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_set", value );

    /* (0x00001010 bits 7) field TX_FRAMES_1024_TO_1518_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1024_TO_1518_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_1024_TO_1518_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 7) field TX_FRAMES_1024_TO_1518_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_1024_TO_1518_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_1024_TO_1518_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_set", value );

    /* (0x00001010 bits 6) field TX_FRAMES_512_TO_1023_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_512_TO_1023_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_512_TO_1023_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 6) field TX_FRAMES_512_TO_1023_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_512_TO_1023_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_512_TO_1023_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_set", value );

    /* (0x00001010 bits 5) field TX_FRAMES_256_TO_511_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_256_TO_511_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_256_TO_511_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 5) field TX_FRAMES_256_TO_511_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_256_TO_511_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_256_TO_511_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_set", value );

    /* (0x00001010 bits 4) field TX_FRAMES_128_TO_255_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_128_TO_255_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_128_TO_255_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 4) field TX_FRAMES_128_TO_255_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_128_TO_255_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_128_TO_255_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_set", value );

    /* (0x00001010 bits 3) field TX_FRAMES_65_TO_127_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_65_TO_127_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_65_TO_127_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 3) field TX_FRAMES_65_TO_127_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_65_TO_127_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_65_TO_127_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_64_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_64_OCTETS_I_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_64_OCTETS_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_64_OCTETS_I_set", value );

    /* (0x00001010 bits 2) field TX_FRAMES_64_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_64_OCTETS_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_FRAMES_64_OCTETS_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_64_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_64_OCTETS_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 2) field TX_FRAMES_64_OCTETS_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_FRAMES_64_OCTETS_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_FRAMES_64_OCTETS_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_64_OCTETS_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_set", value );

    /* (0x00001010 bits 1) field TX_MULTICAST_FRAMES_ATTEMPTED_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_MULTICAST_FRAMES_ATTEMPTED_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_MULTICAST_FRAMES_ATTEMPTED_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 1) field TX_MULTICAST_FRAMES_ATTEMPTED_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_MULTICAST_FRAMES_ATTEMPTED_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_MULTICAST_FRAMES_ATTEMPTED_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_get", value );

    return value;
}
static INLINE void mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_set", value );

    /* (0x00001010 bits 0) field TX_BROADCAST_FRAMES_ATTEMPTED_I of register PMC_MSTATX_REG_INT_5 */
    mstatx_reg_INT_5_field_set( b_ptr,
                                h_ptr,
                                MSTATX_REG_INT_5_BIT_TX_BROADCAST_FRAMES_ATTEMPTED_I_MSK,
                                MSTATX_REG_INT_5_BIT_TX_BROADCAST_FRAMES_ATTEMPTED_I_OFF,
                                value);
}

static INLINE UINT32 mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 0) field TX_BROADCAST_FRAMES_ATTEMPTED_I of register PMC_MSTATX_REG_INT_5 */
    reg_value = mstatx_reg_INT_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_5_BIT_TX_BROADCAST_FRAMES_ATTEMPTED_I_MSK) >> MSTATX_REG_INT_5_BIT_TX_BROADCAST_FRAMES_ATTEMPTED_I_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_get", value );

    return value;
}
static INLINE void mstatx_field_RX_LONG_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_LONG_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_LONG_LENGTH_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_LONG_LENGTH_ERROR_E_set", value );

    /* (0x00001014 bits 15) field RX_LONG_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_LONG_LENGTH_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_LONG_LENGTH_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_LONG_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_LONG_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 15) field RX_LONG_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_LONG_LENGTH_ERROR_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_LONG_LENGTH_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_LONG_LENGTH_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_set", value );

    /* (0x00001014 bits 14) field RX_IN_RANGE_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_IN_RANGE_LENGTH_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_IN_RANGE_LENGTH_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 14) field RX_IN_RANGE_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_IN_RANGE_LENGTH_ERROR_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_IN_RANGE_LENGTH_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_LAYER1_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_LAYER1_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_LAYER1_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_LAYER1_ERROR_E_set", value );

    /* (0x00001014 bits 13) field RX_LAYER1_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_LAYER1_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_LAYER1_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_LAYER1_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_LAYER1_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 13) field RX_LAYER1_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_LAYER1_ERROR_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_LAYER1_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_LAYER1_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_SYMBOL_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_SYMBOL_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_SYMBOL_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_SYMBOL_ERROR_E_set", value );

    /* (0x00001014 bits 12) field RX_SYMBOL_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_SYMBOL_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_SYMBOL_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_SYMBOL_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_SYMBOL_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 12) field RX_SYMBOL_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_SYMBOL_ERROR_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_SYMBOL_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_SYMBOL_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FCS_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FCS_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FCS_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FCS_ERROR_E_set", value );

    /* (0x00001014 bits 10) field RX_FCS_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_FCS_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_FCS_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FCS_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FCS_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 10) field RX_FCS_ERROR_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_FCS_ERROR_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_FCS_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FCS_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_set", value );

    /* (0x00001014 bits 9) field RX_MAC_CONTROL_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_MAC_CONTROL_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_MAC_CONTROL_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 9) field RX_MAC_CONTROL_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_MAC_CONTROL_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_MAC_CONTROL_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set", value );

    /* (0x00001014 bits 8) field RX_MAC_PAUSE_CONTROL_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 8) field RX_MAC_PAUSE_CONTROL_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_TAGGED_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_TAGGED_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_TAGGED_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_TAGGED_FRAMES_OK_E_set", value );

    /* (0x00001014 bits 7) field RX_TAGGED_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_TAGGED_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_TAGGED_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_TAGGED_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_TAGGED_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 7) field RX_TAGGED_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_TAGGED_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_TAGGED_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_TAGGED_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_MULTICAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_MULTICAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_MULTICAST_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_MULTICAST_FRAMES_OK_E_set", value );

    /* (0x00001014 bits 6) field RX_MULTICAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_MULTICAST_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_MULTICAST_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_MULTICAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_MULTICAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 6) field RX_MULTICAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_MULTICAST_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_MULTICAST_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_MULTICAST_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_BROADCAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_BROADCAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_BROADCAST_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_BROADCAST_FRAMES_OK_E_set", value );

    /* (0x00001014 bits 5) field RX_BROADCAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_BROADCAST_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_BROADCAST_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_BROADCAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_BROADCAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 5) field RX_BROADCAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_BROADCAST_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_BROADCAST_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_BROADCAST_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_UNICAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_UNICAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_UNICAST_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_UNICAST_FRAMES_OK_E_set", value );

    /* (0x00001014 bits 4) field RX_UNICAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_UNICAST_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_UNICAST_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_UNICAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_UNICAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 4) field RX_UNICAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_UNICAST_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_UNICAST_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_UNICAST_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_OCTETS_E_set", value );

    /* (0x00001014 bits 3) field RX_OCTETS_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 3) field RX_OCTETS_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_E_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_E_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_E_set", value );

    /* (0x00001014 bits 2) field RX_FRAMES_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_FRAMES_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_FRAMES_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 2) field RX_FRAMES_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_FRAMES_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_FRAMES_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_OCTETS_OK_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_OCTETS_OK_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_OCTETS_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_OCTETS_OK_E_set", value );

    /* (0x00001014 bits 1) field RX_OCTETS_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_OCTETS_OK_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_OCTETS_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_OCTETS_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_OCTETS_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 1) field RX_OCTETS_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_OCTETS_OK_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_OCTETS_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_OCTETS_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_OK_E_set", value );

    /* (0x00001014 bits 0) field RX_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    mstatx_reg_INT_EN_1_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_1_BIT_RX_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_1_BIT_RX_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 0) field RX_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_1 */
    reg_value = mstatx_reg_INT_EN_1_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_1_BIT_RX_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_1_BIT_RX_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_VARIABLE_E_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_VARIABLE_E_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_VARIABLE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_VARIABLE_E_set", value );

    /* (0x00001018 bits 11) field RX_VARIABLE_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_VARIABLE_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_VARIABLE_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_VARIABLE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_VARIABLE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 11) field RX_VARIABLE_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_VARIABLE_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_VARIABLE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_VARIABLE_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_STATION_ADDRESS_FILTERED_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_STATION_ADDRESS_FILTERED_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_STATION_ADDRESS_FILTERED_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_STATION_ADDRESS_FILTERED_E_set", value );

    /* (0x00001018 bits 10) field RX_STATION_ADDRESS_FILTERED_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_STATION_ADDRESS_FILTERED_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_STATION_ADDRESS_FILTERED_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_STATION_ADDRESS_FILTERED_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_STATION_ADDRESS_FILTERED_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 10) field RX_STATION_ADDRESS_FILTERED_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_STATION_ADDRESS_FILTERED_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_STATION_ADDRESS_FILTERED_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_STATION_ADDRESS_FILTERED_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_set", value );

    /* (0x00001018 bits 9) field RX_FRAMES_1519_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_1519_TO_MAX_SIZE_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_1519_TO_MAX_SIZE_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 9) field RX_FRAMES_1519_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_1519_TO_MAX_SIZE_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_1519_TO_MAX_SIZE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_set", value );

    /* (0x00001018 bits 8) field RX_FRAMES_1024_TO_1518_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_1024_TO_1518_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_1024_TO_1518_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 8) field RX_FRAMES_1024_TO_1518_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_1024_TO_1518_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_1024_TO_1518_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_set", value );

    /* (0x00001018 bits 7) field RX_FRAMES_512_TO_1023_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_512_TO_1023_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_512_TO_1023_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 7) field RX_FRAMES_512_TO_1023_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_512_TO_1023_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_512_TO_1023_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_set", value );

    /* (0x00001018 bits 6) field RX_FRAMES_256_TO_511_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_256_TO_511_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_256_TO_511_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 6) field RX_FRAMES_256_TO_511_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_256_TO_511_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_256_TO_511_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_set", value );

    /* (0x00001018 bits 5) field RX_FRAMES_128_TO_255_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_128_TO_255_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_128_TO_255_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 5) field RX_FRAMES_128_TO_255_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_128_TO_255_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_128_TO_255_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_set", value );

    /* (0x00001018 bits 4) field RX_FRAMES_65_TO_127_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_65_TO_127_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_65_TO_127_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 4) field RX_FRAMES_65_TO_127_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_65_TO_127_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_65_TO_127_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_64_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_64_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_64_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_64_OCTETS_E_set", value );

    /* (0x00001018 bits 3) field RX_FRAMES_64_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_64_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_64_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_64_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_64_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 3) field RX_FRAMES_64_OCTETS_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_64_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_FRAMES_64_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_64_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_set", value );

    /* (0x00001018 bits 2) field RX_SHORT_LENGTH_CRC_ERROR_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_SHORT_LENGTH_CRC_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_SHORT_LENGTH_CRC_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 2) field RX_SHORT_LENGTH_CRC_ERROR_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_SHORT_LENGTH_CRC_ERROR_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_SHORT_LENGTH_CRC_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_SHORT_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_SHORT_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_SHORT_LENGTH_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_SHORT_LENGTH_ERROR_E_set", value );

    /* (0x00001018 bits 1) field RX_SHORT_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_SHORT_LENGTH_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_SHORT_LENGTH_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_SHORT_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_SHORT_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 1) field RX_SHORT_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_SHORT_LENGTH_ERROR_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_SHORT_LENGTH_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_SHORT_LENGTH_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_set", value );

    /* (0x00001018 bits 0) field RX_LONG_LENGTH_CRC_ERROR_E of register PMC_MSTATX_REG_INT_EN_2 */
    mstatx_reg_INT_EN_2_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_2_BIT_RX_LONG_LENGTH_CRC_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_2_BIT_RX_LONG_LENGTH_CRC_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 0) field RX_LONG_LENGTH_CRC_ERROR_E of register PMC_MSTATX_REG_INT_EN_2 */
    reg_value = mstatx_reg_INT_EN_2_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_2_BIT_RX_LONG_LENGTH_CRC_ERROR_E_MSK) >> MSTATX_REG_INT_EN_2_BIT_RX_LONG_LENGTH_CRC_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_set", value );

    /* (0x0000101c bits 4) field RX_FRAMES_2501_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_3 */
    mstatx_reg_INT_EN_3_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_2501_TO_MAX_SIZE_E_MSK,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_2501_TO_MAX_SIZE_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000101c bits 4) field RX_FRAMES_2501_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_3 */
    reg_value = mstatx_reg_INT_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_2501_TO_MAX_SIZE_E_MSK) >> MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_2501_TO_MAX_SIZE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_set", value );

    /* (0x0000101c bits 3) field RX_FRAMES_2001_TO_2500_OCTETS_E of register PMC_MSTATX_REG_INT_EN_3 */
    mstatx_reg_INT_EN_3_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_2001_TO_2500_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_2001_TO_2500_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000101c bits 3) field RX_FRAMES_2001_TO_2500_OCTETS_E of register PMC_MSTATX_REG_INT_EN_3 */
    reg_value = mstatx_reg_INT_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_2001_TO_2500_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_2001_TO_2500_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_set", value );

    /* (0x0000101c bits 2) field RX_FRAMES_1549_TO_2000_OCTETS_E of register PMC_MSTATX_REG_INT_EN_3 */
    mstatx_reg_INT_EN_3_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1549_TO_2000_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1549_TO_2000_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000101c bits 2) field RX_FRAMES_1549_TO_2000_OCTETS_E of register PMC_MSTATX_REG_INT_EN_3 */
    reg_value = mstatx_reg_INT_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1549_TO_2000_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1549_TO_2000_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_set", value );

    /* (0x0000101c bits 1) field RX_FRAMES_1523_TO_1548_OCTETS_E of register PMC_MSTATX_REG_INT_EN_3 */
    mstatx_reg_INT_EN_3_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1523_TO_1548_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1523_TO_1548_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000101c bits 1) field RX_FRAMES_1523_TO_1548_OCTETS_E of register PMC_MSTATX_REG_INT_EN_3 */
    reg_value = mstatx_reg_INT_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1523_TO_1548_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1523_TO_1548_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_set", value );

    /* (0x0000101c bits 0) field RX_FRAMES_1519_TO_1522_OCTETS_E of register PMC_MSTATX_REG_INT_EN_3 */
    mstatx_reg_INT_EN_3_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1519_TO_1522_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1519_TO_1522_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000101c bits 0) field RX_FRAMES_1519_TO_1522_OCTETS_E of register PMC_MSTATX_REG_INT_EN_3 */
    reg_value = mstatx_reg_INT_EN_3_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1519_TO_1522_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_3_BIT_RX_FRAMES_1519_TO_1522_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_set", value );

    /* (0x00001020 bits 15) field TX_UNICAST_FRAMES_ATTEMPTED_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_UNICAST_FRAMES_ATTEMPTED_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_UNICAST_FRAMES_ATTEMPTED_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 15) field TX_UNICAST_FRAMES_ATTEMPTED_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_UNICAST_FRAMES_ATTEMPTED_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_UNICAST_FRAMES_ATTEMPTED_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MULTICAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MULTICAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_OK_E_set", value );

    /* (0x00001020 bits 14) field TX_MULTICAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_MULTICAST_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_MULTICAST_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_MULTICAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MULTICAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 14) field TX_MULTICAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_MULTICAST_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_MULTICAST_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_BROADCAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_BROADCAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_OK_E_set", value );

    /* (0x00001020 bits 13) field TX_BROADCAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_BROADCAST_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_BROADCAST_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_BROADCAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_BROADCAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 13) field TX_BROADCAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_BROADCAST_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_BROADCAST_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_UNICAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_UNICAST_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_OK_E_set", value );

    /* (0x00001020 bits 12) field TX_UNICAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_UNICAST_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_UNICAST_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_UNICAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_UNICAST_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 12) field TX_UNICAST_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_UNICAST_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_UNICAST_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_UNICAST_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_set", value );

    /* (0x00001020 bits 11) field TX_MAC_CONTROL_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_MAC_CONTROL_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_MAC_CONTROL_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 11) field TX_MAC_CONTROL_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_MAC_CONTROL_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_MAC_CONTROL_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_set", value );

    /* (0x00001020 bits 10) field TX_FRAMES_1523_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_FRAMES_1523_TO_MAX_SIZE_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_FRAMES_1523_TO_MAX_SIZE_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 10) field TX_FRAMES_1523_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_FRAMES_1523_TO_MAX_SIZE_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_FRAMES_1523_TO_MAX_SIZE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MAC_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MAC_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MAC_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MAC_ERROR_E_set", value );

    /* (0x00001020 bits 9) field TX_MAC_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_MAC_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_MAC_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_MAC_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MAC_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 9) field TX_MAC_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_MAC_ERROR_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_MAC_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MAC_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_SYSTEM_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_SYSTEM_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_SYSTEM_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_SYSTEM_ERROR_E_set", value );

    /* (0x00001020 bits 8) field TX_SYSTEM_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_SYSTEM_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_SYSTEM_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_SYSTEM_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_SYSTEM_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 8) field TX_SYSTEM_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_SYSTEM_ERROR_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_SYSTEM_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_SYSTEM_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_LONG_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_LONG_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_LONG_LENGTH_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_LONG_LENGTH_ERROR_E_set", value );

    /* (0x00001020 bits 7) field TX_LONG_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_LONG_LENGTH_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_LONG_LENGTH_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_LONG_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_LONG_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 7) field TX_LONG_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_LONG_LENGTH_ERROR_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_LONG_LENGTH_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_LONG_LENGTH_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_SHORT_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_SHORT_LENGTH_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_SHORT_LENGTH_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_SHORT_LENGTH_ERROR_E_set", value );

    /* (0x00001020 bits 6) field TX_SHORT_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_SHORT_LENGTH_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_SHORT_LENGTH_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_SHORT_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_SHORT_LENGTH_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 6) field TX_SHORT_LENGTH_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_SHORT_LENGTH_ERROR_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_SHORT_LENGTH_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_SHORT_LENGTH_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FCS_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FCS_ERROR_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FCS_ERROR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FCS_ERROR_E_set", value );

    /* (0x00001020 bits 5) field TX_FCS_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_FCS_ERROR_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_FCS_ERROR_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FCS_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FCS_ERROR_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 5) field TX_FCS_ERROR_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_FCS_ERROR_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_FCS_ERROR_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FCS_ERROR_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                                      enet_fege_handle_t *h_ptr,
                                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set", value );

    /* (0x00001020 bits 4) field TX_MAC_PAUSE_CONTROL_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 4) field TX_MAC_PAUSE_CONTROL_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_TAGGED_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_TAGGED_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_TAGGED_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_TAGGED_FRAMES_OK_E_set", value );

    /* (0x00001020 bits 3) field TX_TAGGED_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_TAGGED_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_TAGGED_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_TAGGED_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_TAGGED_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 3) field TX_TAGGED_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_TAGGED_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_TAGGED_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_TAGGED_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_OCTETS_E_set", value );

    /* (0x00001020 bits 2) field TX_OCTETS_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 2) field TX_OCTETS_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_OCTETS_OK_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_OCTETS_OK_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_OCTETS_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_OCTETS_OK_E_set", value );

    /* (0x00001020 bits 1) field TX_OCTETS_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_OCTETS_OK_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_OCTETS_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_OCTETS_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_OCTETS_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 1) field TX_OCTETS_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_OCTETS_OK_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_OCTETS_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_OCTETS_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_OK_E_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_OK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_OK_E_set", value );

    /* (0x00001020 bits 0) field TX_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    mstatx_reg_INT_EN_4_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_4_BIT_TX_FRAMES_OK_E_MSK,
                                   MSTATX_REG_INT_EN_4_BIT_TX_FRAMES_OK_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_OK_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001020 bits 0) field TX_FRAMES_OK_E of register PMC_MSTATX_REG_INT_EN_4 */
    reg_value = mstatx_reg_INT_EN_4_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_4_BIT_TX_FRAMES_OK_E_MSK) >> MSTATX_REG_INT_EN_4_BIT_TX_FRAMES_OK_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_OK_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_set", value );

    /* (0x00001024 bits 15) field TX_FRAMES_2501_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_2501_TO_MAX_SIZE_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_2501_TO_MAX_SIZE_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 15) field TX_FRAMES_2501_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_2501_TO_MAX_SIZE_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_2501_TO_MAX_SIZE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_set", value );

    /* (0x00001024 bits 14) field TX_FRAMES_2001_TO_2500_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_2001_TO_2500_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_2001_TO_2500_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 14) field TX_FRAMES_2001_TO_2500_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_2001_TO_2500_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_2001_TO_2500_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_set", value );

    /* (0x00001024 bits 13) field TX_FRAMES_1549_TO_2000_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1549_TO_2000_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1549_TO_2000_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 13) field TX_FRAMES_1549_TO_2000_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1549_TO_2000_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1549_TO_2000_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_set", value );

    /* (0x00001024 bits 12) field TX_FRAMES_1523_TO_1548_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1523_TO_1548_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1523_TO_1548_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 12) field TX_FRAMES_1523_TO_1548_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1523_TO_1548_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1523_TO_1548_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_set", value );

    /* (0x00001024 bits 11) field TX_FRAMES_1519_TO_1522_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1519_TO_1522_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1519_TO_1522_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 11) field TX_FRAMES_1519_TO_1522_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1519_TO_1522_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1519_TO_1522_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_set", value );

    /* (0x00001024 bits 10) field RX_FRAMES_1523_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_RX_FRAMES_1523_TO_MAX_SIZE_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_RX_FRAMES_1523_TO_MAX_SIZE_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 10) field RX_FRAMES_1523_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_RX_FRAMES_1523_TO_MAX_SIZE_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_RX_FRAMES_1523_TO_MAX_SIZE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_VARIABLE_E_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_VARIABLE_E_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_VARIABLE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_VARIABLE_E_set", value );

    /* (0x00001024 bits 9) field TX_VARIABLE_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_VARIABLE_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_VARIABLE_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_VARIABLE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_VARIABLE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 9) field TX_VARIABLE_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_VARIABLE_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_VARIABLE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_VARIABLE_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_set", value );

    /* (0x00001024 bits 8) field TX_FRAMES_1519_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1519_TO_MAX_SIZE_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1519_TO_MAX_SIZE_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 8) field TX_FRAMES_1519_TO_MAX_SIZE_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1519_TO_MAX_SIZE_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1519_TO_MAX_SIZE_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_set", value );

    /* (0x00001024 bits 7) field TX_FRAMES_1024_TO_1518_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1024_TO_1518_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1024_TO_1518_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 7) field TX_FRAMES_1024_TO_1518_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1024_TO_1518_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_1024_TO_1518_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                    enet_fege_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_set", value );

    /* (0x00001024 bits 6) field TX_FRAMES_512_TO_1023_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_512_TO_1023_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_512_TO_1023_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 6) field TX_FRAMES_512_TO_1023_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_512_TO_1023_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_512_TO_1023_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_set", value );

    /* (0x00001024 bits 5) field TX_FRAMES_256_TO_511_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_256_TO_511_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_256_TO_511_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 5) field TX_FRAMES_256_TO_511_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_256_TO_511_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_256_TO_511_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_set", value );

    /* (0x00001024 bits 4) field TX_FRAMES_128_TO_255_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_128_TO_255_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_128_TO_255_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 4) field TX_FRAMES_128_TO_255_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_128_TO_255_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_128_TO_255_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                                  enet_fege_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_set", value );

    /* (0x00001024 bits 3) field TX_FRAMES_65_TO_127_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_65_TO_127_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_65_TO_127_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 3) field TX_FRAMES_65_TO_127_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_65_TO_127_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_65_TO_127_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_FRAMES_64_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_FRAMES_64_OCTETS_E_set( mstatx_buffer_t *b_ptr,
                                                           enet_fege_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_FRAMES_64_OCTETS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_FRAMES_64_OCTETS_E_set", value );

    /* (0x00001024 bits 2) field TX_FRAMES_64_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_64_OCTETS_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_64_OCTETS_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_FRAMES_64_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_FRAMES_64_OCTETS_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 2) field TX_FRAMES_64_OCTETS_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_64_OCTETS_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_FRAMES_64_OCTETS_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_FRAMES_64_OCTETS_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_set", value );

    /* (0x00001024 bits 1) field TX_MULTICAST_FRAMES_ATTEMPTED_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_MULTICAST_FRAMES_ATTEMPTED_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_MULTICAST_FRAMES_ATTEMPTED_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 1) field TX_MULTICAST_FRAMES_ATTEMPTED_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_MULTICAST_FRAMES_ATTEMPTED_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_MULTICAST_FRAMES_ATTEMPTED_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_get", value );

    return value;
}
static INLINE void mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_set( mstatx_buffer_t *b_ptr,
                                                                     enet_fege_handle_t *h_ptr,
                                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_set", value );

    /* (0x00001024 bits 0) field TX_BROADCAST_FRAMES_ATTEMPTED_E of register PMC_MSTATX_REG_INT_EN_5 */
    mstatx_reg_INT_EN_5_field_set( b_ptr,
                                   h_ptr,
                                   MSTATX_REG_INT_EN_5_BIT_TX_BROADCAST_FRAMES_ATTEMPTED_E_MSK,
                                   MSTATX_REG_INT_EN_5_BIT_TX_BROADCAST_FRAMES_ATTEMPTED_E_OFF,
                                   value);
}

static INLINE UINT32 mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001024 bits 0) field TX_BROADCAST_FRAMES_ATTEMPTED_E of register PMC_MSTATX_REG_INT_EN_5 */
    reg_value = mstatx_reg_INT_EN_5_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_INT_EN_5_BIT_TX_BROADCAST_FRAMES_ATTEMPTED_E_MSK) >> MSTATX_REG_INT_EN_5_BIT_TX_BROADCAST_FRAMES_ATTEMPTED_E_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_get", value );

    return value;
}
static INLINE void mstatx_field_LCLK_MODE_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_LCLK_MODE_set( mstatx_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_LCLK_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_LCLK_MODE_set", value );

    /* (0x00001028 bits 15) field LCLK_MODE of register PMC_MSTATX_REG_CTRL */
    mstatx_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               MSTATX_REG_CTRL_BIT_LCLK_MODE_MSK,
                               MSTATX_REG_CTRL_BIT_LCLK_MODE_OFF,
                               value);
}

static INLINE UINT32 mstatx_field_LCLK_MODE_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_LCLK_MODE_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001028 bits 15) field LCLK_MODE of register PMC_MSTATX_REG_CTRL */
    reg_value = mstatx_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_CTRL_BIT_LCLK_MODE_MSK) >> MSTATX_REG_CTRL_BIT_LCLK_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_LCLK_MODE_get", value );

    return value;
}
static INLINE void mstatx_field_TX_LOAD_set( mstatx_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_LOAD_set( mstatx_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_LOAD_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_LOAD_set", value );

    /* (0x00001028 bits 6) field TX_LOAD of register PMC_MSTATX_REG_CTRL */
    mstatx_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               MSTATX_REG_CTRL_BIT_TX_LOAD_MSK,
                               MSTATX_REG_CTRL_BIT_TX_LOAD_OFF,
                               value);
}

static INLINE UINT32 mstatx_field_TX_LOAD_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_LOAD_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001028 bits 6) field TX_LOAD of register PMC_MSTATX_REG_CTRL */
    reg_value = mstatx_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_CTRL_BIT_TX_LOAD_MSK) >> MSTATX_REG_CTRL_BIT_TX_LOAD_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_LOAD_get", value );

    return value;
}
static INLINE void mstatx_field_TX_SNAP_CLEAR_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_SNAP_CLEAR_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_SNAP_CLEAR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_SNAP_CLEAR_set", value );

    /* (0x00001028 bits 5) field TX_SNAP_CLEAR of register PMC_MSTATX_REG_CTRL */
    mstatx_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               MSTATX_REG_CTRL_BIT_TX_SNAP_CLEAR_MSK,
                               MSTATX_REG_CTRL_BIT_TX_SNAP_CLEAR_OFF,
                               value);
}

static INLINE UINT32 mstatx_field_TX_SNAP_CLEAR_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_SNAP_CLEAR_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001028 bits 5) field TX_SNAP_CLEAR of register PMC_MSTATX_REG_CTRL */
    reg_value = mstatx_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_CTRL_BIT_TX_SNAP_CLEAR_MSK) >> MSTATX_REG_CTRL_BIT_TX_SNAP_CLEAR_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_SNAP_CLEAR_get", value );

    return value;
}
static INLINE void mstatx_field_TX_SNAP_POST_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_SNAP_POST_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_SNAP_POST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_SNAP_POST_set", value );

    /* (0x00001028 bits 4) field TX_SNAP_POST of register PMC_MSTATX_REG_CTRL */
    mstatx_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               MSTATX_REG_CTRL_BIT_TX_SNAP_POST_MSK,
                               MSTATX_REG_CTRL_BIT_TX_SNAP_POST_OFF,
                               value);
}

static INLINE UINT32 mstatx_field_TX_SNAP_POST_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_SNAP_POST_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001028 bits 4) field TX_SNAP_POST of register PMC_MSTATX_REG_CTRL */
    reg_value = mstatx_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_CTRL_BIT_TX_SNAP_POST_MSK) >> MSTATX_REG_CTRL_BIT_TX_SNAP_POST_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_SNAP_POST_get", value );

    return value;
}
static INLINE void mstatx_field_RX_LOAD_set( mstatx_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_LOAD_set( mstatx_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_LOAD_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_LOAD_set", value );

    /* (0x00001028 bits 2) field RX_LOAD of register PMC_MSTATX_REG_CTRL */
    mstatx_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               MSTATX_REG_CTRL_BIT_RX_LOAD_MSK,
                               MSTATX_REG_CTRL_BIT_RX_LOAD_OFF,
                               value);
}

static INLINE UINT32 mstatx_field_RX_LOAD_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_LOAD_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001028 bits 2) field RX_LOAD of register PMC_MSTATX_REG_CTRL */
    reg_value = mstatx_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_CTRL_BIT_RX_LOAD_MSK) >> MSTATX_REG_CTRL_BIT_RX_LOAD_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_LOAD_get", value );

    return value;
}
static INLINE void mstatx_field_RX_SNAP_CLEAR_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_SNAP_CLEAR_set( mstatx_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_SNAP_CLEAR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_SNAP_CLEAR_set", value );

    /* (0x00001028 bits 1) field RX_SNAP_CLEAR of register PMC_MSTATX_REG_CTRL */
    mstatx_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               MSTATX_REG_CTRL_BIT_RX_SNAP_CLEAR_MSK,
                               MSTATX_REG_CTRL_BIT_RX_SNAP_CLEAR_OFF,
                               value);
}

static INLINE UINT32 mstatx_field_RX_SNAP_CLEAR_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_SNAP_CLEAR_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001028 bits 1) field RX_SNAP_CLEAR of register PMC_MSTATX_REG_CTRL */
    reg_value = mstatx_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_CTRL_BIT_RX_SNAP_CLEAR_MSK) >> MSTATX_REG_CTRL_BIT_RX_SNAP_CLEAR_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_SNAP_CLEAR_get", value );

    return value;
}
static INLINE void mstatx_field_RX_SNAP_POST_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_SNAP_POST_set( mstatx_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_SNAP_POST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_SNAP_POST_set", value );

    /* (0x00001028 bits 0) field RX_SNAP_POST of register PMC_MSTATX_REG_CTRL */
    mstatx_reg_CTRL_field_set( b_ptr,
                               h_ptr,
                               MSTATX_REG_CTRL_BIT_RX_SNAP_POST_MSK,
                               MSTATX_REG_CTRL_BIT_RX_SNAP_POST_OFF,
                               value);
}

static INLINE UINT32 mstatx_field_RX_SNAP_POST_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_SNAP_POST_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001028 bits 0) field RX_SNAP_POST of register PMC_MSTATX_REG_CTRL */
    reg_value = mstatx_reg_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_CTRL_BIT_RX_SNAP_POST_MSK) >> MSTATX_REG_CTRL_BIT_RX_SNAP_POST_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_SNAP_POST_get", value );

    return value;
}
static INLINE void mstatx_field_TX_INDEX_set( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_TX_INDEX_set( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_TX_INDEX_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mstatx_field_TX_INDEX_set", value );

    /* (0x0000102c bits 12:8) bits 0:4 use field TX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
    mstatx_reg_VARIABLE_SELECT_field_set( b_ptr,
                                          h_ptr,
                                          MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_MSK,
                                          MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_OFF,
                                          value);
}

static INLINE UINT32 mstatx_field_TX_INDEX_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_TX_INDEX_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000102c bits 12:8) bits 0:4 use field TX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
    reg_value = mstatx_reg_VARIABLE_SELECT_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_MSK) >> MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_TX_INDEX_get", value );

    return value;
}
static INLINE void mstatx_field_range_TX_INDEX_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_range_TX_INDEX_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mstatx_field_range_TX_INDEX_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mstatx_field_range_TX_INDEX_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mstatx_field_range_TX_INDEX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000102c bits 12:8) bits 0:4 use field TX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
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
        /* (0x0000102c bits 12:8) bits 0:4 use field TX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
        mstatx_reg_VARIABLE_SELECT_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_OFF + subfield_offset),
                                              MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mstatx_field_range_TX_INDEX_get( mstatx_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_range_TX_INDEX_get( mstatx_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mstatx_field_range_TX_INDEX_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mstatx_field_range_TX_INDEX_get", stop_bit, 4 );
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
    /* (0x0000102c bits 12:8) bits 0:4 use field TX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
    reg_value = mstatx_reg_VARIABLE_SELECT_read(  b_ptr, h_ptr);
    field_value = (reg_value & MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_MSK)
                  >> MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_MSK, MSTATX_REG_VARIABLE_SELECT_BIT_TX_INDEX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mstatx_field_range_TX_INDEX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void mstatx_field_RX_INDEX_set( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_RX_INDEX_set( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "mstatx_field_RX_INDEX_set", value, 31);
    IOLOG( "%s <= 0x%08x", "mstatx_field_RX_INDEX_set", value );

    /* (0x0000102c bits 4:0) field RX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
    mstatx_reg_VARIABLE_SELECT_field_set( b_ptr,
                                          h_ptr,
                                          MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_MSK,
                                          MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_OFF,
                                          value);
}

static INLINE UINT32 mstatx_field_RX_INDEX_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_RX_INDEX_get( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000102c bits 4:0) field RX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
    reg_value = mstatx_reg_VARIABLE_SELECT_read(  b_ptr, h_ptr);
    value = (reg_value & MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_MSK) >> MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_OFF;
    IOLOG( "%s -> 0x%08x", "mstatx_field_RX_INDEX_get", value );

    return value;
}
static INLINE void mstatx_field_range_RX_INDEX_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void mstatx_field_range_RX_INDEX_set( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mstatx_field_range_RX_INDEX_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mstatx_field_range_RX_INDEX_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "mstatx_field_range_RX_INDEX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000102c bits 4:0) field RX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
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
        /* (0x0000102c bits 4:0) field RX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
        mstatx_reg_VARIABLE_SELECT_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_OFF + subfield_offset),
                                              MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 mstatx_field_range_RX_INDEX_get( mstatx_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_range_RX_INDEX_get( mstatx_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;
    UINT32 subfield_start_bit;
    UINT32 subfield_stop_bit;
    UINT32 subfield_offset;
    UINT32 subfield_shift;
    UINT32 subfield_num_bits;
    UINT32 subfield_mask;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mstatx_field_range_RX_INDEX_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mstatx_field_range_RX_INDEX_get", stop_bit, 4 );
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
    /* (0x0000102c bits 4:0) field RX_INDEX of register PMC_MSTATX_REG_VARIABLE_SELECT */
    reg_value = mstatx_reg_VARIABLE_SELECT_read(  b_ptr, h_ptr);
    field_value = (reg_value & MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_MSK)
                  >> MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_MSK, MSTATX_REG_VARIABLE_SELECT_BIT_RX_INDEX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "mstatx_field_range_RX_INDEX_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size65
 * ==================================================================================
 */
static INLINE UINT32 mstatx_field_COUNTL_get( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_COUNTL_get( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 64)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mstatx_field_COUNTL_get", N, 64);
    /* ((0x00001040 + (N) * 0x10) bits 15:0) bits 0:15 use field COUNTL of register PMC_MSTATX_REG_STATISTICAL_CNT_LOW index N=0..64 */
    reg_value = mstatx_reg_STATISTICAL_CNT_LOW_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & MSTATX_REG_STATISTICAL_CNT_LOW_BIT_COUNTL_MSK) >> MSTATX_REG_STATISTICAL_CNT_LOW_BIT_COUNTL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mstatx_field_COUNTL_get", N, value );

    return value;
}
static INLINE UINT32 mstatx_field_range_COUNTL_get( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_range_COUNTL_get( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
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

    if (N > 64)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mstatx_field_range_COUNTL_get", N, 64);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mstatx_field_range_COUNTL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mstatx_field_range_COUNTL_get", stop_bit, 15 );
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
    /* ((0x00001040 + (N) * 0x10) bits 15:0) bits 0:15 use field COUNTL of register PMC_MSTATX_REG_STATISTICAL_CNT_LOW index N=0..64 */
    reg_value = mstatx_reg_STATISTICAL_CNT_LOW_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & MSTATX_REG_STATISTICAL_CNT_LOW_BIT_COUNTL_MSK)
                  >> MSTATX_REG_STATISTICAL_CNT_LOW_BIT_COUNTL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MSTATX_REG_STATISTICAL_CNT_LOW_BIT_COUNTL_MSK, MSTATX_REG_STATISTICAL_CNT_LOW_BIT_COUNTL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mstatx_field_range_COUNTL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mstatx_field_COUNTM_get( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_COUNTM_get( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 64)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mstatx_field_COUNTM_get", N, 64);
    /* ((0x00001044 + (N) * 0x10) bits 15:0) bits 0:15 use field COUNTM of register PMC_MSTATX_REG_STATISTICAL_CNT_MID index N=0..64 */
    reg_value = mstatx_reg_STATISTICAL_CNT_MID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & MSTATX_REG_STATISTICAL_CNT_MID_BIT_COUNTM_MSK) >> MSTATX_REG_STATISTICAL_CNT_MID_BIT_COUNTM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mstatx_field_COUNTM_get", N, value );

    return value;
}
static INLINE UINT32 mstatx_field_range_COUNTM_get( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_range_COUNTM_get( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
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

    if (N > 64)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mstatx_field_range_COUNTM_get", N, 64);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mstatx_field_range_COUNTM_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mstatx_field_range_COUNTM_get", stop_bit, 15 );
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
    /* ((0x00001044 + (N) * 0x10) bits 15:0) bits 0:15 use field COUNTM of register PMC_MSTATX_REG_STATISTICAL_CNT_MID index N=0..64 */
    reg_value = mstatx_reg_STATISTICAL_CNT_MID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & MSTATX_REG_STATISTICAL_CNT_MID_BIT_COUNTM_MSK)
                  >> MSTATX_REG_STATISTICAL_CNT_MID_BIT_COUNTM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MSTATX_REG_STATISTICAL_CNT_MID_BIT_COUNTM_MSK, MSTATX_REG_STATISTICAL_CNT_MID_BIT_COUNTM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mstatx_field_range_COUNTM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 mstatx_field_COUNTH_get( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_COUNTH_get( mstatx_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 64)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mstatx_field_COUNTH_get", N, 64);
    /* ((0x00001048 + (N) * 0x10) bits 7:0) bits 0:7 use field COUNTH of register PMC_MSTATX_REG_STATISTICAL_CNT_HIGH index N=0..64 */
    reg_value = mstatx_reg_STATISTICAL_CNT_HIGH_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & MSTATX_REG_STATISTICAL_CNT_HIGH_BIT_COUNTH_MSK) >> MSTATX_REG_STATISTICAL_CNT_HIGH_BIT_COUNTH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "mstatx_field_COUNTH_get", N, value );

    return value;
}
static INLINE UINT32 mstatx_field_range_COUNTH_get( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 mstatx_field_range_COUNTH_get( mstatx_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
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

    if (N > 64)
        IO_RANGE_CHECK("%s N is %d but max is %d", "mstatx_field_range_COUNTH_get", N, 64);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "mstatx_field_range_COUNTH_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "mstatx_field_range_COUNTH_get", stop_bit, 7 );
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
    /* ((0x00001048 + (N) * 0x10) bits 7:0) bits 0:7 use field COUNTH of register PMC_MSTATX_REG_STATISTICAL_CNT_HIGH index N=0..64 */
    reg_value = mstatx_reg_STATISTICAL_CNT_HIGH_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & MSTATX_REG_STATISTICAL_CNT_HIGH_BIT_COUNTH_MSK)
                  >> MSTATX_REG_STATISTICAL_CNT_HIGH_BIT_COUNTH_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, MSTATX_REG_STATISTICAL_CNT_HIGH_BIT_COUNTH_MSK, MSTATX_REG_STATISTICAL_CNT_HIGH_BIT_COUNTH_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "mstatx_field_range_COUNTH_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MSTATX_IO_INLINE_H */
