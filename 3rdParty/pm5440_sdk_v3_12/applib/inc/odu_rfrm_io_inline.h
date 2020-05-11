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
 *     and register accessor functions for the odu_rfrm block
 *****************************************************************************/
#ifndef _ODU_RFRM_IO_INLINE_H
#define _ODU_RFRM_IO_INLINE_H

#include "odu_rfrm_loc.h"
#include "odu_rfrm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ODU_RFRM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for odu_rfrm
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
    odu_rfrm_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} odu_rfrm_buffer_t;
static INLINE void odu_rfrm_buffer_init( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void odu_rfrm_buffer_init( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "odu_rfrm_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void odu_rfrm_buffer_flush( odu_rfrm_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void odu_rfrm_buffer_flush( odu_rfrm_buffer_t *b_ptr )
{
    IOLOG( "odu_rfrm_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 odu_rfrm_reg_read( odu_rfrm_buffer_t *b_ptr,
                                        odu_rfrm_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_read( odu_rfrm_buffer_t *b_ptr,
                                        odu_rfrm_handle_t *h_ptr,
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
static INLINE void odu_rfrm_reg_write( odu_rfrm_buffer_t *b_ptr,
                                       odu_rfrm_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_write( odu_rfrm_buffer_t *b_ptr,
                                       odu_rfrm_handle_t *h_ptr,
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

static INLINE void odu_rfrm_field_set( odu_rfrm_buffer_t *b_ptr,
                                       odu_rfrm_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_set( odu_rfrm_buffer_t *b_ptr,
                                       odu_rfrm_handle_t *h_ptr,
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

static INLINE void odu_rfrm_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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

static INLINE void odu_rfrm_burst_read( odu_rfrm_buffer_t *b_ptr,
                                        odu_rfrm_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_burst_read( odu_rfrm_buffer_t *b_ptr,
                                        odu_rfrm_handle_t *h_ptr,
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

static INLINE void odu_rfrm_burst_write( odu_rfrm_buffer_t *b_ptr,
                                         odu_rfrm_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_burst_write( odu_rfrm_buffer_t *b_ptr,
                                         odu_rfrm_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_poll( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_poll( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
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
 *  register access functions for odu_rfrm
 * ==================================================================================
 */

static INLINE void odu_rfrm_reg_GLOBAL_CONFIG_write( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_GLOBAL_CONFIG_write( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_GLOBAL_CONFIG_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_RFRM_REG_GLOBAL_CONFIG,
                        value);
}

static INLINE void odu_rfrm_reg_GLOBAL_CONFIG_field_set( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_GLOBAL_CONFIG_field_set( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_GLOBAL_CONFIG_field_set", mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_RFRM_REG_GLOBAL_CONFIG,
                        mask,
                        PMC_ODU_RFRM_REG_GLOBAL_CONFIG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_GLOBAL_CONFIG_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_GLOBAL_CONFIG_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODU_RFRM_REG_GLOBAL_CONFIG);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_GLOBAL_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_CHANNEL_CTRL1_array_write( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_CTRL1_array_write( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_CHANNEL_CTRL1_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_RFRM_REG_CHANNEL_CTRL1(N),
                        value);
}

static INLINE void odu_rfrm_reg_CHANNEL_CTRL1_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_CTRL1_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_CHANNEL_CTRL1_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_RFRM_REG_CHANNEL_CTRL1(N),
                        mask,
                        PMC_ODU_RFRM_REG_CHANNEL_CTRL1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_CTRL1_array_read( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_CTRL1_array_read( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODU_RFRM_REG_CHANNEL_CTRL1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_CHANNEL_CTRL1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0,
                        value);
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_field_set", mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0,
                        mask,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1,
                        value);
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_field_set", mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1,
                        mask,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2,
                        value);
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_field_set", mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2,
                        mask,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_DEFECT_INT_EN_write( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_DEFECT_INT_EN_write( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_DEFECT_INT_EN_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_DEFECT_INT_EN,
                        value);
}

static INLINE void odu_rfrm_reg_DEFECT_INT_EN_field_set( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_DEFECT_INT_EN_field_set( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_DEFECT_INT_EN_field_set", mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_DEFECT_INT_EN,
                        mask,
                        PMC_ODU_RFRM_REG_DEFECT_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_DEFECT_INT_EN_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_DEFECT_INT_EN_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_DEFECT_INT_EN);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_DEFECT_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN,
                        value);
}

static INLINE void odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_field_set", mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN,
                        mask,
                        PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_DEFECT_INT_STAT_write( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_DEFECT_INT_STAT_write( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_DEFECT_INT_STAT_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_DEFECT_INT_STAT,
                        value);
}

static INLINE void odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                           odu_rfrm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                           odu_rfrm_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set", mask, ofs, value );
    odu_rfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_RFRM_REG_DEFECT_INT_STAT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_rfrm_reg_DEFECT_INT_STAT_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_DEFECT_INT_STAT_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_DEFECT_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_DEFECT_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_DEFECT_INT_STAT_poll( odu_rfrm_buffer_t *b_ptr,
                                                                      odu_rfrm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_DEFECT_INT_STAT_poll( odu_rfrm_buffer_t *b_ptr,
                                                                      odu_rfrm_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_rfrm_reg_DEFECT_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_rfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_RFRM_REG_DEFECT_INT_STAT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_write( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_write( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT,
                        value);
}

static INLINE void odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                                    odu_rfrm_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                                    odu_rfrm_handle_t *h_ptr,
                                                                                    UINT32 mask,
                                                                                    UINT32 ofs,
                                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_action_on_write_field_set", mask, ofs, value );
    odu_rfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_poll( odu_rfrm_buffer_t *b_ptr,
                                                                               odu_rfrm_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_poll( odu_rfrm_buffer_t *b_ptr,
                                                                               odu_rfrm_handle_t *h_ptr,
                                                                               UINT32 mask,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_rfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_write( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_write( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0,
                        value);
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                                      odu_rfrm_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                                      odu_rfrm_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_action_on_write_field_set", mask, ofs, value );
    odu_rfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_poll( odu_rfrm_buffer_t *b_ptr,
                                                                                 odu_rfrm_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_poll( odu_rfrm_buffer_t *b_ptr,
                                                                                 odu_rfrm_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_rfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_write( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_write( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1,
                        value);
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                                      odu_rfrm_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                                      odu_rfrm_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_action_on_write_field_set", mask, ofs, value );
    odu_rfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_poll( odu_rfrm_buffer_t *b_ptr,
                                                                                 odu_rfrm_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_poll( odu_rfrm_buffer_t *b_ptr,
                                                                                 odu_rfrm_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_rfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_write( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_write( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2,
                        value);
}

static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                                      odu_rfrm_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_action_on_write_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                                      odu_rfrm_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_action_on_write_field_set", mask, ofs, value );
    odu_rfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_poll( odu_rfrm_buffer_t *b_ptr,
                                                                                 odu_rfrm_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_poll( odu_rfrm_buffer_t *b_ptr,
                                                                                 odu_rfrm_handle_t *h_ptr,
                                                                                 UINT32 mask,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_rfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_rfrm_reg_AC_TTI_PM_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 ofs,
                                                            UINT32 len,
                                                            UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_AC_TTI_PM_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 ofs,
                                                            UINT32 len,
                                                            UINT32 *value )
{
    if (len + ofs > 1536)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_rfrm_reg_AC_TTI_PM_array_burst_read", ofs, len, 1536 );
    odu_rfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_RFRM_REG_AC_TTI_PM(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_rfrm_reg_AC_TTI_PM_array_burst_read", 1536, ofs, len);
}

static INLINE UINT32 odu_rfrm_reg_AC_TTI_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_AC_TTI_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_AC_TTI_PM(N));

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_AC_TTI_PM_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_EXP_TTI_PID_array_burst_write( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_EXP_TTI_PID_array_burst_write( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32 ofs,
                                                               UINT32 len,
                                                               UINT32 *value )
{
    if (len + ofs > 2048)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_rfrm_reg_EXP_TTI_PID_array_burst_write", ofs, len, 2048 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_rfrm_reg_EXP_TTI_PID_array_burst_write", 2048, ofs, len);
    odu_rfrm_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_RFRM_REG_EXP_TTI_PID(ofs),
                          len,
                          value);

}

static INLINE void odu_rfrm_reg_EXP_TTI_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_EXP_TTI_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 N,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_EXP_TTI_PID_array_field_set", mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_EXP_TTI_PID(N),
                        mask,
                        PMC_ODU_RFRM_REG_EXP_TTI_PID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odu_rfrm_reg_EXP_TTI_PID_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_EXP_TTI_PID_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value )
{
    if (len + ofs > 2048)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_rfrm_reg_EXP_TTI_PID_array_burst_read", ofs, len, 2048 );
    odu_rfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_RFRM_REG_EXP_TTI_PID(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_rfrm_reg_EXP_TTI_PID_array_burst_read", 2048, ofs, len);
}

static INLINE UINT32 odu_rfrm_reg_EXP_TTI_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_EXP_TTI_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_EXP_TTI_PID(N));

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_EXP_TTI_PID_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_AC_TTI_PID_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_AC_TTI_PID_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value )
{
    if (len + ofs > 4096)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_rfrm_reg_AC_TTI_PID_array_burst_read", ofs, len, 4096 );
    odu_rfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_RFRM_REG_AC_TTI_PID(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_rfrm_reg_AC_TTI_PID_array_burst_read", 4096, ofs, len);
}

static INLINE UINT32 odu_rfrm_reg_AC_TTI_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_AC_TTI_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_AC_TTI_PID(N));

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_AC_TTI_PID_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_EXP_TTI_PM_array_burst_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_EXP_TTI_PM_array_burst_write( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32 ofs,
                                                              UINT32 len,
                                                              UINT32 *value )
{
    if (len + ofs > 768)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_rfrm_reg_EXP_TTI_PM_array_burst_write", ofs, len, 768 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_rfrm_reg_EXP_TTI_PM_array_burst_write", 768, ofs, len);
    odu_rfrm_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_RFRM_REG_EXP_TTI_PM(ofs),
                          len,
                          value);

}

static INLINE void odu_rfrm_reg_EXP_TTI_PM_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_EXP_TTI_PM_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 N,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_EXP_TTI_PM_array_field_set", mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_EXP_TTI_PM(N),
                        mask,
                        PMC_ODU_RFRM_REG_EXP_TTI_PM_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odu_rfrm_reg_EXP_TTI_PM_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_EXP_TTI_PM_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value )
{
    if (len + ofs > 768)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_rfrm_reg_EXP_TTI_PM_array_burst_read", ofs, len, 768 );
    odu_rfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_RFRM_REG_EXP_TTI_PM(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_rfrm_reg_EXP_TTI_PM_array_burst_read", 768, ofs, len);
}

static INLINE UINT32 odu_rfrm_reg_EXP_TTI_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_EXP_TTI_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_EXP_TTI_PM(N));

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_EXP_TTI_PM_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_FTFL_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_FTFL_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value )
{
    if (len + ofs > 768)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_rfrm_reg_FTFL_array_burst_read", ofs, len, 768 );
    odu_rfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_RFRM_REG_FTFL(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_rfrm_reg_FTFL_array_burst_read", 768, ofs, len);
}

static INLINE UINT32 odu_rfrm_reg_FTFL_array_read( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_FTFL_array_read( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_FTFL(N));

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_FTFL_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_AC_APS_PCC_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_AC_APS_PCC_array_burst_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 ofs,
                                                             UINT32 len,
                                                             UINT32 *value )
{
    if (len + ofs > 768)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_rfrm_reg_AC_APS_PCC_array_burst_read", ofs, len, 768 );
    odu_rfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_RFRM_REG_AC_APS_PCC(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_rfrm_reg_AC_APS_PCC_array_burst_read", 768, ofs, len);
}

static INLINE UINT32 odu_rfrm_reg_AC_APS_PCC_array_read( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_AC_APS_PCC_array_read( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_AC_APS_PCC(N));

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_AC_APS_PCC_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_rfrm_reg_LAYER_CTRL0_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_LAYER_CTRL0_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_LAYER_CTRL0_PID_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_LAYER_CTRL0_PID(N),
                        value);
}

static INLINE void odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_LAYER_CTRL0_PID(N),
                        mask,
                        PMC_ODU_RFRM_REG_LAYER_CTRL0_PID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_LAYER_CTRL0_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_LAYER_CTRL0_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_LAYER_CTRL0_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_LAYER_CTRL0_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_rfrm_reg_LAYER_CTRL1_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_LAYER_CTRL1_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_LAYER_CTRL1_PID_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_LAYER_CTRL1_PID(N),
                        value);
}

static INLINE void odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_LAYER_CTRL1_PID(N),
                        mask,
                        PMC_ODU_RFRM_REG_LAYER_CTRL1_PID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_LAYER_CTRL1_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_LAYER_CTRL1_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_LAYER_CTRL1_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_LAYER_CTRL1_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_NEAR_END_PMON_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_NEAR_END_PMON_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_NEAR_END_PMON_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_NEAR_END_PMON_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_FAR_END_PMON_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_FAR_END_PMON_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_FAR_END_PMON_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_FAR_END_PMON_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_NE_FE_EBC_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_NE_FE_EBC_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_NE_FE_EBC_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_NE_FE_EBC_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_rfrm_reg_LAYER_CTRL0_PM_array_write( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_LAYER_CTRL0_PM_array_write( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_LAYER_CTRL0_PM_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_LAYER_CTRL0_PM(N),
                        value);
}

static INLINE void odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_LAYER_CTRL0_PM(N),
                        mask,
                        PMC_ODU_RFRM_REG_LAYER_CTRL0_PM_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_LAYER_CTRL0_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_LAYER_CTRL0_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_LAYER_CTRL0_PM(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_LAYER_CTRL0_PM_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_rfrm_reg_LAYER_CTRL1_PM_array_write( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_LAYER_CTRL1_PM_array_write( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_LAYER_CTRL1_PM_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_LAYER_CTRL1_PM(N),
                        value);
}

static INLINE void odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_LAYER_CTRL1_PM(N),
                        mask,
                        PMC_ODU_RFRM_REG_LAYER_CTRL1_PM_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_LAYER_CTRL1_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_LAYER_CTRL1_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_LAYER_CTRL1_PM(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_LAYER_CTRL1_PM_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_rfrm_reg_TCM1_2_3_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_TCM1_2_3_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_TCM1_2_3_PID_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_TCM1_2_3_PID(N),
                        value);
}

static INLINE void odu_rfrm_reg_TCM1_2_3_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_TCM1_2_3_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_TCM1_2_3_PID_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_TCM1_2_3_PID(N),
                        mask,
                        PMC_ODU_RFRM_REG_TCM1_2_3_PID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_TCM1_2_3_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_TCM1_2_3_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_TCM1_2_3_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_TCM1_2_3_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_rfrm_reg_TCM4_5_6_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_TCM4_5_6_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_TCM4_5_6_PID_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_TCM4_5_6_PID(N),
                        value);
}

static INLINE void odu_rfrm_reg_TCM4_5_6_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_TCM4_5_6_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_TCM4_5_6_PID_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_TCM4_5_6_PID(N),
                        mask,
                        PMC_ODU_RFRM_REG_TCM4_5_6_PID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_TCM4_5_6_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_TCM4_5_6_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_TCM4_5_6_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_TCM4_5_6_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_rfrm_reg_FLOATING_PM_NIM_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_FLOATING_PM_NIM_PID_array_write( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_FLOATING_PM_NIM_PID_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID(N),
                        value);
}

static INLINE void odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                     odu_rfrm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                                     odu_rfrm_handle_t *h_ptr,
                                                                     UINT32  N,
                                                                     UINT32 mask,
                                                                     UINT32 ofs,
                                                                     UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID(N),
                        mask,
                        PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_rfrm_reg_CHANNEL_CTRL0_array_write( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_CTRL0_array_write( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_rfrm_reg_CHANNEL_CTRL0_array_write", value );
    odu_rfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_CTRL0(N),
                        value);
}

static INLINE void odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 mask,
                                                               UINT32 ofs,
                                                               UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_rfrm_reg_CHANNEL_CTRL0_array_field_set", N, mask, ofs, value );
    odu_rfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_RFRM_REG_CHANNEL_CTRL0(N),
                        mask,
                        PMC_ODU_RFRM_REG_CHANNEL_CTRL0_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_CTRL0_array_read( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_CTRL0_array_read( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_CTRL0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_CHANNEL_CTRL0_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_ROW4_OH1_array_read( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_ROW4_OH1_array_read( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_ROW4_OH1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_ROW4_OH1_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_NEAR_END_PMON_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_NEAR_END_PMON_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_NEAR_END_PMON_PM(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_NEAR_END_PMON_PM_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_FAR_END_PMON_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_FAR_END_PMON_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_FAR_END_PMON_PM(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_FAR_END_PMON_PM_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_NE_FE_EBC_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_NE_FE_EBC_PM_array_read( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_NE_FE_EBC_PM(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_NE_FE_EBC_PM_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_AC_STAT_array_read( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_AC_STAT_array_read( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_AC_STAT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_AC_STAT_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_ROW2_OH_array_read( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_ROW2_OH_array_read( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_ROW2_OH(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_ROW2_OH_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_ROW3_4_OH_array_read( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_ROW3_4_OH_array_read( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_ROW3_4_OH(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_ROW3_4_OH_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_ROW4_OH0_array_read( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_ROW4_OH0_array_read( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_ROW4_OH0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_rfrm_reg_ROW4_OH0_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY0_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY0_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY1_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY1_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY2_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY2_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY2_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY0_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY0_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_PID_DEFECT_INT_SUMMARY0_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY1_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY1_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_PID_DEFECT_INT_SUMMARY1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY2_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY2_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_PID_DEFECT_INT_SUMMARY2_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY3_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY3_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_PID_DEFECT_INT_SUMMARY3_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY4_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY4_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_PID_DEFECT_INT_SUMMARY4_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY5_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY5_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_PID_DEFECT_INT_SUMMARY5_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY6_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY6_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_PID_DEFECT_INT_SUMMARY6_read", reg_value);
    return reg_value;
}

static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY7_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_reg_PID_DEFECT_INT_SUMMARY7_read( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_rfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7);

    IOLOG( "%s -> 0x%08x;", "odu_rfrm_reg_PID_DEFECT_INT_SUMMARY7_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void odu_rfrm_field_DLTC_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DLTC_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DLTC_ACT_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_DLTC_ACT_DIS_set", value );

    /* (0x00012464 bits 15) field DLTC_ACT_DIS of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    odu_rfrm_reg_GLOBAL_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLTC_ACT_DIS_MSK,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLTC_ACT_DIS_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_DLTC_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLTC_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012464 bits 15) field DLTC_ACT_DIS of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    reg_value = odu_rfrm_reg_GLOBAL_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLTC_ACT_DIS_MSK) >> ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLTC_ACT_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_DLTC_ACT_DIS_get", value );

    return value;
}
static INLINE void odu_rfrm_field_DAIS_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DAIS_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DAIS_ACT_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_DAIS_ACT_DIS_set", value );

    /* (0x00012464 bits 14) field DAIS_ACT_DIS of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    odu_rfrm_reg_GLOBAL_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DAIS_ACT_DIS_MSK,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DAIS_ACT_DIS_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_DAIS_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DAIS_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012464 bits 14) field DAIS_ACT_DIS of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    reg_value = odu_rfrm_reg_GLOBAL_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DAIS_ACT_DIS_MSK) >> ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DAIS_ACT_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_DAIS_ACT_DIS_get", value );

    return value;
}
static INLINE void odu_rfrm_field_DOCI_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DOCI_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DOCI_ACT_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_DOCI_ACT_DIS_set", value );

    /* (0x00012464 bits 13) field DOCI_ACT_DIS of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    odu_rfrm_reg_GLOBAL_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DOCI_ACT_DIS_MSK,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DOCI_ACT_DIS_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_DOCI_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DOCI_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012464 bits 13) field DOCI_ACT_DIS of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    reg_value = odu_rfrm_reg_GLOBAL_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DOCI_ACT_DIS_MSK) >> ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DOCI_ACT_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_DOCI_ACT_DIS_get", value );

    return value;
}
static INLINE void odu_rfrm_field_DLCK_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DLCK_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DLCK_ACT_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_DLCK_ACT_DIS_set", value );

    /* (0x00012464 bits 12) field DLCK_ACT_DIS of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    odu_rfrm_reg_GLOBAL_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLCK_ACT_DIS_MSK,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLCK_ACT_DIS_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_DLCK_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLCK_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012464 bits 12) field DLCK_ACT_DIS of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    reg_value = odu_rfrm_reg_GLOBAL_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLCK_ACT_DIS_MSK) >> ODU_RFRM_REG_GLOBAL_CONFIG_BIT_DLCK_ACT_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_DLCK_ACT_DIS_get", value );

    return value;
}
static INLINE void odu_rfrm_field_AC_TTI_set( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_AC_TTI_set( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_AC_TTI_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_AC_TTI_set", value );

    /* (0x00012464 bits 10) field AC_TTI_FIELD of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    odu_rfrm_reg_GLOBAL_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_AC_TTI_FIELD_MSK,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_AC_TTI_FIELD_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_AC_TTI_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AC_TTI_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012464 bits 10) field AC_TTI_FIELD of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    reg_value = odu_rfrm_reg_GLOBAL_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_GLOBAL_CONFIG_BIT_AC_TTI_FIELD_MSK) >> ODU_RFRM_REG_GLOBAL_CONFIG_BIT_AC_TTI_FIELD_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_AC_TTI_get", value );

    return value;
}
static INLINE void odu_rfrm_field_LOCK_TTI_set( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_LOCK_TTI_set( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_LOCK_TTI_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_LOCK_TTI_set", value );

    /* (0x00012464 bits 9) field LOCK_TTI of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    odu_rfrm_reg_GLOBAL_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_TTI_MSK,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_TTI_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_LOCK_TTI_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_LOCK_TTI_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012464 bits 9) field LOCK_TTI of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    reg_value = odu_rfrm_reg_GLOBAL_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_TTI_MSK) >> ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_TTI_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_LOCK_TTI_get", value );

    return value;
}
static INLINE void odu_rfrm_field_LOCK_set( odu_rfrm_buffer_t *b_ptr,
                                            odu_rfrm_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_LOCK_set( odu_rfrm_buffer_t *b_ptr,
                                            odu_rfrm_handle_t *h_ptr,
                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_LOCK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_LOCK_set", value );

    /* (0x00012464 bits 8) field LOCK of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    odu_rfrm_reg_GLOBAL_CONFIG_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_MSK,
                                          ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_LOCK_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_LOCK_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012464 bits 8) field LOCK of register PMC_ODU_RFRM_REG_GLOBAL_CONFIG */
    reg_value = odu_rfrm_reg_GLOBAL_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_MSK) >> ODU_RFRM_REG_GLOBAL_CONFIG_BIT_LOCK_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_LOCK_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void odu_rfrm_field_LOFLOM_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_LOFLOM_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_LOFLOM_ACT_DIS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_LOFLOM_ACT_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_LOFLOM_ACT_DIS_set", N, value );

    /* ((0x00010c18 + (N) * 0x40) bits 4) field LOFLOM_ACT_DIS of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL1_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL1_BIT_LOFLOM_ACT_DIS_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL1_BIT_LOFLOM_ACT_DIS_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_LOFLOM_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_LOFLOM_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_LOFLOM_ACT_DIS_get", N, 95);
    /* ((0x00010c18 + (N) * 0x40) bits 4) field LOFLOM_ACT_DIS of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL1_BIT_LOFLOM_ACT_DIS_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL1_BIT_LOFLOM_ACT_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_LOFLOM_ACT_DIS_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_FAU_PASSTHRU_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_FAU_PASSTHRU_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FAU_PASSTHRU_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_FAU_PASSTHRU_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_FAU_PASSTHRU_set", N, value );

    /* ((0x00010c18 + (N) * 0x40) bits 3) field FAU_PASSTHRU of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL1_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL1_BIT_FAU_PASSTHRU_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL1_BIT_FAU_PASSTHRU_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_FAU_PASSTHRU_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_FAU_PASSTHRU_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FAU_PASSTHRU_get", N, 95);
    /* ((0x00010c18 + (N) * 0x40) bits 3) field FAU_PASSTHRU of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL1_BIT_FAU_PASSTHRU_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL1_BIT_FAU_PASSTHRU_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_FAU_PASSTHRU_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_MAINT_SIG_set( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_MAINT_SIG_set( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_MAINT_SIG_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_MAINT_SIG_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_MAINT_SIG_set", N, value );

    /* ((0x00010c18 + (N) * 0x40) bits 2:1) field MAINT_SIG of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL1_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_MAINT_SIG_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_MAINT_SIG_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_MAINT_SIG_get", N, 95);
    /* ((0x00010c18 + (N) * 0x40) bits 2:1) field MAINT_SIG of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_MAINT_SIG_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_MAINT_SIG_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_MAINT_SIG_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_MAINT_SIG_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_MAINT_SIG_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_MAINT_SIG_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_MAINT_SIG_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c18 + (N) * 0x40) bits 2:1) field MAINT_SIG of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
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
        /* ((0x00010c18 + (N) * 0x40) bits 2:1) field MAINT_SIG of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
        odu_rfrm_reg_CHANNEL_CTRL1_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_OFF + subfield_offset),
                                                    ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_MAINT_SIG_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_MAINT_SIG_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_MAINT_SIG_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_MAINT_SIG_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_MAINT_SIG_get", stop_bit, 1 );
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
    /* ((0x00010c18 + (N) * 0x40) bits 2:1) field MAINT_SIG of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_MSK)
                  >> ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_MSK, ODU_RFRM_REG_CHANNEL_CTRL1_BIT_MAINT_SIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_MAINT_SIG_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_PASSTHRU_set( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PASSTHRU_set( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PASSTHRU_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PASSTHRU_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_PASSTHRU_set", N, value );

    /* ((0x00010c18 + (N) * 0x40) bits 0) field PASSTHRU of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL1_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL1_BIT_PASSTHRU_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL1_BIT_PASSTHRU_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_PASSTHRU_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PASSTHRU_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PASSTHRU_get", N, 95);
    /* ((0x00010c18 + (N) * 0x40) bits 0) field PASSTHRU of register PMC_ODU_RFRM_REG_CHANNEL_CTRL1 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL1_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL1_BIT_PASSTHRU_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL1_BIT_PASSTHRU_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PASSTHRU_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void odu_rfrm_lfield_AC_APS_E_set( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_AC_APS_E_set( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_rfrm_lfield_AC_APS_E_set", value[2] , value[1] , value[0] );

    /* (0x00012448 bits 31:0) bits 0:31 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0 */
    odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_field_set( b_ptr,
                                                   h_ptr,
                                                   ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_MSK,
                                                   ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_OFF,
                                                   value[0]);

    /* (0x0001244c bits 31:0) bits 32:63 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1 */
    odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_field_set( b_ptr,
                                                   h_ptr,
                                                   ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_MSK,
                                                   ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_OFF,
                                                   value[1]);

    /* (0x00012450 bits 31:0) bits 64:95 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2 */
    odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_field_set( b_ptr,
                                                   h_ptr,
                                                   ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_MSK,
                                                   ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_OFF,
                                                   value[2]);
}

static INLINE void odu_rfrm_lfield_AC_APS_E_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_AC_APS_E_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00012448 bits 31:0) bits 0:31 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0 */
    reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_MSK) >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_OFF;
    value[0] |= field_value;

    /* (0x0001244c bits 31:0) bits 32:63 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1 */
    reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_MSK) >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_OFF;
    value[1] |= field_value;

    /* (0x00012450 bits 31:0) bits 64:95 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2 */
    reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_MSK) >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_rfrm_lfield_AC_APS_E_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_rfrm_lfield_range_AC_APS_E_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_range_AC_APS_E_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_lfield_range_AC_APS_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_lfield_range_AC_APS_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_lfield_range_AC_APS_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00012448 bits 31:0) bits 0:31 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0 */
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
        /* (0x00012448 bits 31:0) bits 0:31 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0 */
        odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_OFF + subfield_offset),
                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001244c bits 31:0) bits 32:63 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1 */
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
        /* (0x0001244c bits 31:0) bits 32:63 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1 */
        odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_OFF + subfield_offset),
                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00012450 bits 31:0) bits 64:95 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2 */
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
        /* (0x00012450 bits 31:0) bits 64:95 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2 */
        odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_OFF + subfield_offset),
                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_lfield_range_AC_APS_E_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_AC_APS_E_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_lfield_range_AC_APS_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_lfield_range_AC_APS_E_get", stop_bit, 95 );
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
    /* (0x00012448 bits 31:0) bits 0:31 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0 */
        reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_MSK)
                      >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_MSK, ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN0_BIT_AC_APS_E_OFF, field_value );
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
    /* (0x0001244c bits 31:0) bits 32:63 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1 */
        reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_MSK)
                      >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_MSK, ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN1_BIT_AC_APS_E_OFF, field_value );
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
    /* (0x00012450 bits 31:0) bits 64:95 use field AC_APS_E of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2 */
        reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_MSK)
                      >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_MSK, ODU_RFRM_REG_CHANNEL_AC_APS_INT_EN2_BIT_AC_APS_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_lfield_range_AC_APS_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_ABDI_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_ABDI_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_ABDI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_ABDI_E_set", value );

    /* (0x00012454 bits 15) field COMB_ABDI_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ABDI_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ABDI_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_ABDI_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_ABDI_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 15) field COMB_ABDI_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ABDI_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ABDI_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_ABDI_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_AAIS_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_AAIS_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_AAIS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_AAIS_E_set", value );

    /* (0x00012454 bits 14) field COMB_AAIS_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_AAIS_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_AAIS_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_AAIS_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_AAIS_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 14) field COMB_AAIS_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_AAIS_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_AAIS_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_AAIS_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_ATSD_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_ATSD_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_ATSD_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_ATSD_E_set", value );

    /* (0x00012454 bits 13) field COMB_ATSD_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSD_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSD_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_ATSD_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_ATSD_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 13) field COMB_ATSD_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSD_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSD_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_ATSD_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_ATSF_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_ATSF_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_ATSF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_ATSF_E_set", value );

    /* (0x00012454 bits 12) field COMB_ATSF_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSF_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSF_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_ATSF_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_ATSF_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 12) field COMB_ATSF_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSF_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_ATSF_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_ATSF_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_OOF_E_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_OOF_E_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_OOF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_OOF_E_set", value );

    /* (0x00012454 bits 10) field COMB_OOF_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_OOF_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_OOF_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_OOF_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_OOF_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 10) field COMB_OOF_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_OOF_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_OOF_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_OOF_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DLOFLOM_E_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DLOFLOM_E_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DLOFLOM_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DLOFLOM_E_set", value );

    /* (0x00012454 bits 9) field COMB_DLOFLOM_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLOFLOM_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLOFLOM_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DLOFLOM_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DLOFLOM_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 9) field COMB_DLOFLOM_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLOFLOM_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLOFLOM_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DLOFLOM_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DBIAE_E_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DBIAE_E_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DBIAE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DBIAE_E_set", value );

    /* (0x00012454 bits 8) field COMB_DBIAE_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBIAE_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBIAE_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DBIAE_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DBIAE_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 8) field COMB_DBIAE_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBIAE_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBIAE_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DBIAE_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DIAE_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DIAE_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DIAE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DIAE_E_set", value );

    /* (0x00012454 bits 7) field COMB_DIAE_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DIAE_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DIAE_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DIAE_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DIAE_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 7) field COMB_DIAE_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DIAE_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DIAE_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DIAE_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DLTC_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DLTC_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DLTC_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DLTC_E_set", value );

    /* (0x00012454 bits 6) field COMB_DLTC_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLTC_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLTC_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DLTC_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DLTC_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 6) field COMB_DLTC_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLTC_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLTC_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DLTC_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DBDI_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DBDI_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DBDI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DBDI_E_set", value );

    /* (0x00012454 bits 5) field COMB_DBDI_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBDI_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBDI_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DBDI_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DBDI_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 5) field COMB_DBDI_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBDI_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DBDI_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DBDI_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DDEG_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DDEG_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DDEG_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DDEG_E_set", value );

    /* (0x00012454 bits 4) field COMB_DDEG_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DDEG_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DDEG_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DDEG_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DDEG_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 4) field COMB_DDEG_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DDEG_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DDEG_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DDEG_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DTIM_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DTIM_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DTIM_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DTIM_E_set", value );

    /* (0x00012454 bits 3) field COMB_DTIM_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DTIM_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DTIM_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DTIM_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DTIM_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 3) field COMB_DTIM_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DTIM_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DTIM_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DTIM_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DAIS_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DAIS_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DAIS_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DAIS_E_set", value );

    /* (0x00012454 bits 2) field COMB_DAIS_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DAIS_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DAIS_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DAIS_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DAIS_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 2) field COMB_DAIS_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DAIS_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DAIS_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DAIS_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DOCI_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DOCI_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DOCI_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DOCI_E_set", value );

    /* (0x00012454 bits 1) field COMB_DOCI_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DOCI_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DOCI_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DOCI_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DOCI_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 1) field COMB_DOCI_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DOCI_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DOCI_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DOCI_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_COMB_DLCK_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DLCK_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DLCK_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DLCK_E_set", value );

    /* (0x00012454 bits 0) field COMB_DLCK_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    odu_rfrm_reg_DEFECT_INT_EN_field_set( b_ptr,
                                          h_ptr,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLCK_E_MSK,
                                          ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLCK_E_OFF,
                                          value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DLCK_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DLCK_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012454 bits 0) field COMB_DLCK_E of register PMC_ODU_RFRM_REG_DEFECT_INT_EN */
    reg_value = odu_rfrm_reg_DEFECT_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLCK_E_MSK) >> ODU_RFRM_REG_DEFECT_INT_EN_BIT_COMB_DLCK_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DLCK_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_PMON_DONE_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PMON_DONE_E_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PMON_DONE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_PMON_DONE_E_set", value );

    /* (0x00012458 bits 5) field PMON_DONE_E of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN */
    odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_field_set( b_ptr,
                                                   h_ptr,
                                                   ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_DONE_E_MSK,
                                                   ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_DONE_E_OFF,
                                                   value);
}

static INLINE UINT32 odu_rfrm_field_PMON_DONE_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PMON_DONE_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012458 bits 5) field PMON_DONE_E of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN */
    reg_value = odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_DONE_E_MSK) >> ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_DONE_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_PMON_DONE_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_DEF_ACT_DONE_E_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DEF_ACT_DONE_E_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DEF_ACT_DONE_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_DEF_ACT_DONE_E_set", value );

    /* (0x00012458 bits 4) field DEF_ACT_DONE_E of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN */
    odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_field_set( b_ptr,
                                                   h_ptr,
                                                   ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_DONE_E_MSK,
                                                   ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_DONE_E_OFF,
                                                   value);
}

static INLINE UINT32 odu_rfrm_field_DEF_ACT_DONE_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DEF_ACT_DONE_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012458 bits 4) field DEF_ACT_DONE_E of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN */
    reg_value = odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_DONE_E_MSK) >> ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_DONE_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_DEF_ACT_DONE_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_PMON_START_E_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PMON_START_E_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PMON_START_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_PMON_START_E_set", value );

    /* (0x00012458 bits 1) field PMON_START_E of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN */
    odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_field_set( b_ptr,
                                                   h_ptr,
                                                   ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_START_E_MSK,
                                                   ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_START_E_OFF,
                                                   value);
}

static INLINE UINT32 odu_rfrm_field_PMON_START_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PMON_START_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012458 bits 1) field PMON_START_E of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN */
    reg_value = odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_START_E_MSK) >> ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_PMON_START_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_PMON_START_E_get", value );

    return value;
}
static INLINE void odu_rfrm_field_DEF_ACT_START_E_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DEF_ACT_START_E_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DEF_ACT_START_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_DEF_ACT_START_E_set", value );

    /* (0x00012458 bits 0) field DEF_ACT_START_E of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN */
    odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_field_set( b_ptr,
                                                   h_ptr,
                                                   ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_START_E_MSK,
                                                   ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_START_E_OFF,
                                                   value);
}

static INLINE UINT32 odu_rfrm_field_DEF_ACT_START_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DEF_ACT_START_E_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012458 bits 0) field DEF_ACT_START_E of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN */
    reg_value = odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_START_E_MSK) >> ODU_RFRM_REG_SHADOW_TRANSFER_INT_EN_BIT_DEF_ACT_START_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_DEF_ACT_START_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void odu_rfrm_field_COMB_ABDI_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_ABDI_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_ABDI_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_ABDI_I_set_to_clear", value );

    /* (0x00012400 bits 15) field COMB_ABDI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ABDI_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ABDI_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_ABDI_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_ABDI_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 15) field COMB_ABDI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ABDI_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ABDI_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_ABDI_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ABDI_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ABDI_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_ABDI_I_poll", value );

    /* (0x00012400 bits 15) field COMB_ABDI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ABDI_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ABDI_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_AAIS_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_AAIS_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_AAIS_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_AAIS_I_set_to_clear", value );

    /* (0x00012400 bits 14) field COMB_AAIS_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_AAIS_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_AAIS_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_AAIS_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_AAIS_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 14) field COMB_AAIS_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_AAIS_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_AAIS_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_AAIS_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_AAIS_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_AAIS_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_AAIS_I_poll", value );

    /* (0x00012400 bits 14) field COMB_AAIS_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_AAIS_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_AAIS_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_ATSD_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_ATSD_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_ATSD_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_ATSD_I_set_to_clear", value );

    /* (0x00012400 bits 13) field COMB_ATSD_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSD_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSD_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_ATSD_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_ATSD_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 13) field COMB_ATSD_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSD_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSD_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_ATSD_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ATSD_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ATSD_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_ATSD_I_poll", value );

    /* (0x00012400 bits 13) field COMB_ATSD_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSD_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSD_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_ATSF_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_ATSF_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_ATSF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_ATSF_I_set_to_clear", value );

    /* (0x00012400 bits 12) field COMB_ATSF_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSF_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSF_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_ATSF_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_ATSF_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 12) field COMB_ATSF_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSF_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSF_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_ATSF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ATSF_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ATSF_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_ATSF_I_poll", value );

    /* (0x00012400 bits 12) field COMB_ATSF_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSF_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_ATSF_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_OOF_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_OOF_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_OOF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_OOF_I_set_to_clear", value );

    /* (0x00012400 bits 10) field COMB_OOF_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_OOF_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_OOF_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_OOF_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_OOF_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 10) field COMB_OOF_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_OOF_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_OOF_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_OOF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_OOF_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                   odu_rfrm_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_OOF_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                   odu_rfrm_handle_t *h_ptr,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_OOF_I_poll", value );

    /* (0x00012400 bits 10) field COMB_OOF_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_OOF_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_OOF_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DLOFLOM_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DLOFLOM_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DLOFLOM_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DLOFLOM_I_set_to_clear", value );

    /* (0x00012400 bits 9) field COMB_DLOFLOM_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLOFLOM_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLOFLOM_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DLOFLOM_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DLOFLOM_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 9) field COMB_DLOFLOM_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLOFLOM_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLOFLOM_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DLOFLOM_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLOFLOM_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                       odu_rfrm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLOFLOM_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                       odu_rfrm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DLOFLOM_I_poll", value );

    /* (0x00012400 bits 9) field COMB_DLOFLOM_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLOFLOM_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLOFLOM_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DBIAE_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DBIAE_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DBIAE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DBIAE_I_set_to_clear", value );

    /* (0x00012400 bits 8) field COMB_DBIAE_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBIAE_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBIAE_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DBIAE_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DBIAE_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 8) field COMB_DBIAE_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBIAE_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBIAE_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DBIAE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DBIAE_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                     odu_rfrm_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DBIAE_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                     odu_rfrm_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DBIAE_I_poll", value );

    /* (0x00012400 bits 8) field COMB_DBIAE_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBIAE_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBIAE_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DIAE_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DIAE_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DIAE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DIAE_I_set_to_clear", value );

    /* (0x00012400 bits 7) field COMB_DIAE_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DIAE_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DIAE_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DIAE_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DIAE_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 7) field COMB_DIAE_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DIAE_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DIAE_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DIAE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DIAE_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DIAE_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DIAE_I_poll", value );

    /* (0x00012400 bits 7) field COMB_DIAE_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DIAE_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DIAE_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DLTC_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DLTC_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DLTC_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DLTC_I_set_to_clear", value );

    /* (0x00012400 bits 6) field COMB_DLTC_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLTC_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLTC_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DLTC_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DLTC_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 6) field COMB_DLTC_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLTC_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLTC_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DLTC_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLTC_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLTC_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DLTC_I_poll", value );

    /* (0x00012400 bits 6) field COMB_DLTC_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLTC_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLTC_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DBDI_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DBDI_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DBDI_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DBDI_I_set_to_clear", value );

    /* (0x00012400 bits 5) field COMB_DBDI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBDI_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBDI_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DBDI_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DBDI_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 5) field COMB_DBDI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBDI_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBDI_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DBDI_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DBDI_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DBDI_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DBDI_I_poll", value );

    /* (0x00012400 bits 5) field COMB_DBDI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBDI_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DBDI_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DDEG_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DDEG_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DDEG_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DDEG_I_set_to_clear", value );

    /* (0x00012400 bits 4) field COMB_DDEG_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DDEG_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DDEG_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DDEG_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DDEG_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 4) field COMB_DDEG_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DDEG_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DDEG_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DDEG_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DDEG_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DDEG_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DDEG_I_poll", value );

    /* (0x00012400 bits 4) field COMB_DDEG_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DDEG_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DDEG_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DTIM_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DTIM_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DTIM_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DTIM_I_set_to_clear", value );

    /* (0x00012400 bits 3) field COMB_DTIM_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DTIM_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DTIM_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DTIM_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DTIM_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 3) field COMB_DTIM_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DTIM_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DTIM_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DTIM_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DTIM_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DTIM_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DTIM_I_poll", value );

    /* (0x00012400 bits 3) field COMB_DTIM_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DTIM_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DTIM_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DAIS_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DAIS_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DAIS_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DAIS_I_set_to_clear", value );

    /* (0x00012400 bits 2) field COMB_DAIS_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DAIS_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DAIS_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DAIS_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DAIS_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 2) field COMB_DAIS_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DAIS_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DAIS_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DAIS_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DAIS_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DAIS_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DAIS_I_poll", value );

    /* (0x00012400 bits 2) field COMB_DAIS_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DAIS_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DAIS_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DOCI_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DOCI_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DOCI_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DOCI_I_set_to_clear", value );

    /* (0x00012400 bits 1) field COMB_DOCI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DOCI_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DOCI_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DOCI_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DOCI_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 1) field COMB_DOCI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DOCI_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DOCI_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DOCI_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DOCI_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DOCI_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DOCI_I_poll", value );

    /* (0x00012400 bits 1) field COMB_DOCI_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DOCI_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DOCI_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_COMB_DLCK_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_COMB_DLCK_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_COMB_DLCK_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_COMB_DLCK_I_set_to_clear", value );

    /* (0x00012400 bits 0) field COMB_DLCK_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set( b_ptr,
                                                            h_ptr,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLCK_I_MSK,
                                                            ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLCK_I_OFF,
                                                            value);
}

static INLINE UINT32 odu_rfrm_field_COMB_DLCK_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_COMB_DLCK_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012400 bits 0) field COMB_DLCK_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    reg_value = odu_rfrm_reg_DEFECT_INT_STAT_read(  b_ptr, h_ptr);
    value = (reg_value & ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLCK_I_MSK) >> ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLCK_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_COMB_DLCK_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLCK_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLCK_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_COMB_DLCK_I_poll", value );

    /* (0x00012400 bits 0) field COMB_DLCK_I of register PMC_ODU_RFRM_REG_DEFECT_INT_STAT */
    return odu_rfrm_reg_DEFECT_INT_STAT_poll( b_ptr,
                                              h_ptr,
                                              ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLCK_I_MSK,
                                              (value<<ODU_RFRM_REG_DEFECT_INT_STAT_BIT_COMB_DLCK_I_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_PMON_DONE_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PMON_DONE_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PMON_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_PMON_DONE_I_set_to_clear", value );

    /* (0x00012430 bits 5) field PMON_DONE_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_DONE_I_MSK,
                                                                     ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_DONE_I_OFF,
                                                                     value);
}

static INLINE UINT32 odu_rfrm_field_PMON_DONE_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PMON_DONE_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012430 bits 5) field PMON_DONE_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    reg_value = odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_DONE_I_MSK) >> ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_DONE_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_PMON_DONE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_PMON_DONE_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_PMON_DONE_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_PMON_DONE_I_poll", value );

    /* (0x00012430 bits 5) field PMON_DONE_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    return odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_poll( b_ptr,
                                                       h_ptr,
                                                       ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_DONE_I_MSK,
                                                       (value<<ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_DONE_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_DEF_ACT_DONE_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DEF_ACT_DONE_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DEF_ACT_DONE_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_DEF_ACT_DONE_I_set_to_clear", value );

    /* (0x00012430 bits 4) field DEF_ACT_DONE_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_DONE_I_MSK,
                                                                     ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_DONE_I_OFF,
                                                                     value);
}

static INLINE UINT32 odu_rfrm_field_DEF_ACT_DONE_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DEF_ACT_DONE_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012430 bits 4) field DEF_ACT_DONE_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    reg_value = odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_DONE_I_MSK) >> ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_DONE_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_DEF_ACT_DONE_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_DEF_ACT_DONE_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                       odu_rfrm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_DEF_ACT_DONE_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                       odu_rfrm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_DEF_ACT_DONE_I_poll", value );

    /* (0x00012430 bits 4) field DEF_ACT_DONE_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    return odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_poll( b_ptr,
                                                       h_ptr,
                                                       ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_DONE_I_MSK,
                                                       (value<<ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_DONE_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_PMON_START_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PMON_START_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PMON_START_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_PMON_START_I_set_to_clear", value );

    /* (0x00012430 bits 1) field PMON_START_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_START_I_MSK,
                                                                     ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_START_I_OFF,
                                                                     value);
}

static INLINE UINT32 odu_rfrm_field_PMON_START_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PMON_START_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012430 bits 1) field PMON_START_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    reg_value = odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_START_I_MSK) >> ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_START_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_PMON_START_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_PMON_START_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                     odu_rfrm_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_PMON_START_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                     odu_rfrm_handle_t *h_ptr,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_PMON_START_I_poll", value );

    /* (0x00012430 bits 1) field PMON_START_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    return odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_poll( b_ptr,
                                                       h_ptr,
                                                       ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_START_I_MSK,
                                                       (value<<ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_PMON_START_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_field_DEF_ACT_START_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DEF_ACT_START_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DEF_ACT_START_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_rfrm_field_DEF_ACT_START_I_set_to_clear", value );

    /* (0x00012430 bits 0) field DEF_ACT_START_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_action_on_write_field_set( b_ptr,
                                                                     h_ptr,
                                                                     ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_START_I_MSK,
                                                                     ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_START_I_OFF,
                                                                     value);
}

static INLINE UINT32 odu_rfrm_field_DEF_ACT_START_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DEF_ACT_START_I_get( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00012430 bits 0) field DEF_ACT_START_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    reg_value = odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_START_I_MSK) >> ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_START_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_rfrm_field_DEF_ACT_START_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_DEF_ACT_START_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                        odu_rfrm_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_field_DEF_ACT_START_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                        odu_rfrm_handle_t *h_ptr,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_rfrm_field_DEF_ACT_START_I_poll", value );

    /* (0x00012430 bits 0) field DEF_ACT_START_I of register PMC_ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT */
    return odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_poll( b_ptr,
                                                       h_ptr,
                                                       ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_START_I_MSK,
                                                       (value<<ODU_RFRM_REG_SHADOW_TRANSFER_INT_STAT_BIT_DEF_ACT_START_I_OFF),
                                                       cmp,
                                                       max_count,
                                                       num_failed_polls,
                                                       delay_between_polls_in_microseconds);
}

static INLINE void odu_rfrm_lfield_AC_APS_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_AC_APS_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_rfrm_lfield_AC_APS_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x0001243c bits 31:0) bits 0:31 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0 */
    odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_MSK,
                                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF,
                                                                       value[0]);

    /* (0x00012440 bits 31:0) bits 32:63 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1 */
    odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_MSK,
                                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF,
                                                                       value[1]);

    /* (0x00012444 bits 31:0) bits 64:95 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2 */
    odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_action_on_write_field_set( b_ptr,
                                                                       h_ptr,
                                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_MSK,
                                                                       ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF,
                                                                       value[2]);
}

static INLINE void odu_rfrm_lfield_AC_APS_I_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_AC_APS_I_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0001243c bits 31:0) bits 0:31 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0 */
    reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_MSK) >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF;
    value[0] |= field_value;

    /* (0x00012440 bits 31:0) bits 32:63 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1 */
    reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_MSK) >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF;
    value[1] |= field_value;

    /* (0x00012444 bits 31:0) bits 64:95 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2 */
    reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_MSK) >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_rfrm_lfield_AC_APS_I_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_rfrm_lfield_range_AC_APS_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_range_AC_APS_I_set_to_clear( odu_rfrm_buffer_t *b_ptr,
                                                                odu_rfrm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_lfield_range_AC_APS_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_lfield_range_AC_APS_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_lfield_range_AC_APS_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001243c bits 31:0) bits 0:31 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0 */
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
        /* (0x0001243c bits 31:0) bits 0:31 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0 */
        odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF + subfield_offset),
                                                                           ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00012440 bits 31:0) bits 32:63 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1 */
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
        /* (0x00012440 bits 31:0) bits 32:63 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1 */
        odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF + subfield_offset),
                                                                           ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00012444 bits 31:0) bits 64:95 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2 */
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
        /* (0x00012444 bits 31:0) bits 64:95 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2 */
        odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_action_on_write_field_set( b_ptr,
                                                                           h_ptr,
                                                                           subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF + subfield_offset),
                                                                           ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_lfield_range_AC_APS_I_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_AC_APS_I_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_lfield_range_AC_APS_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_lfield_range_AC_APS_I_get", stop_bit, 95 );
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
    /* (0x0001243c bits 31:0) bits 0:31 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0 */
        reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_read( b_ptr,
                                                                  h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_MSK)
                      >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_MSK, ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF, field_value );
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
    /* (0x00012440 bits 31:0) bits 32:63 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1 */
        reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_read( b_ptr,
                                                                  h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_MSK)
                      >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_MSK, ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF, field_value );
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
    /* (0x00012444 bits 31:0) bits 64:95 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2 */
        reg_value = odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_read( b_ptr,
                                                                  h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_MSK)
                      >> ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_MSK, ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_lfield_range_AC_APS_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_lfield_range_AC_APS_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                        odu_rfrm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_lfield_range_AC_APS_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                        odu_rfrm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_lfield_range_AC_APS_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_lfield_range_AC_APS_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_lfield_range_AC_APS_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001243c bits 31:0) bits 0:31 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0 */
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
        /* (0x0001243c bits 31:0) bits 0:31 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0 */
        return odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF + subfield_offset),
                                                             value << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00012440 bits 31:0) bits 32:63 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1 */
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
        /* (0x00012440 bits 31:0) bits 32:63 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1 */
        return odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF + subfield_offset),
                                                             value << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00012444 bits 31:0) bits 64:95 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2 */
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
        /* (0x00012444 bits 31:0) bits 64:95 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2 */
        return odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_poll( b_ptr,
                                                             h_ptr,
                                                             subfield_mask << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF + subfield_offset),
                                                             value << (ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF + subfield_offset),
                                                             cmp,
                                                             max_count,
                                                             num_failed_polls,
                                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_lfield_AC_APS_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 value[3],
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_rfrm_lfield_AC_APS_I_poll( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32 value[3],
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "odu_rfrm_lfield_AC_APS_I_poll", value[2] , value[1] , value[0] );

    /* (0x0001243c bits 31:0) bits 0:31 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0 */
    return odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_poll( b_ptr,
                                                         h_ptr,
                                                         ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_MSK,
                                                         (value[0]<<ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS0_BIT_AC_APS_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);

    /* (0x00012440 bits 31:0) bits 32:63 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1 */
    return odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_poll( b_ptr,
                                                         h_ptr,
                                                         ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_MSK,
                                                         (value[1]<<ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS1_BIT_AC_APS_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);

    /* (0x00012444 bits 31:0) bits 64:95 use field AC_APS_I of register PMC_ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2 */
    return odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_poll( b_ptr,
                                                         h_ptr,
                                                         ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_MSK,
                                                         (value[2]<<ODU_RFRM_REG_CHANNEL_AC_APS_INT_STATUS2_BIT_AC_APS_I_OFF),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_burstsize1536
 * ==================================================================================
 */
static INLINE void odu_rfrm_lfield_AC_TTI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value[1536] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_AC_TTI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value[1536] )
{
    IOLOG( "%s ", "odu_rfrm_lfield_AC_TTI_PM_get");
    odu_rfrm_reg_AC_TTI_PM_array_burst_read( b_ptr,
                                             h_ptr,
                                             0,
                                             1536,
                                             (UINT32 *)value);
}

static INLINE UINT32 odu_rfrm_lfield_range_AC_TTI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_AC_TTI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_rfrm_lfield_range_AC_TTI_PM_get", start_bit, stop_bit );
    }
    reg_value = odu_rfrm_reg_AC_TTI_PM_array_read( b_ptr,
                                                   h_ptr,
                                                   word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_rfrm_lfield_range_AC_TTI_PM_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_burstsize2048
 * ==================================================================================
 */
static INLINE void odu_rfrm_lfield_EX_TTI_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32 value[2048] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_EX_TTI_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32 value[2048] )
{
    IOLOG( "%s ", "odu_rfrm_lfield_EX_TTI_set");
    odu_rfrm_reg_EXP_TTI_PID_array_burst_write( b_ptr,
                                                h_ptr,
                                                0,
                                                2048,
                                                value);
}

static INLINE void odu_rfrm_lfield_EX_TTI_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32 value[2048] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_EX_TTI_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32 value[2048] )
{
    IOLOG( "%s ", "odu_rfrm_lfield_EX_TTI_get");
    odu_rfrm_reg_EXP_TTI_PID_array_burst_read( b_ptr,
                                               h_ptr,
                                               0,
                                               2048,
                                               (UINT32 *)value);
}

static INLINE void odu_rfrm_lfield_range_EX_TTI_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_range_EX_TTI_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_rfrm_lfield_range_EX_TTI_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_rfrm_lfield_range_EX_TTI_set", start_bit, stop_bit, value);
    odu_rfrm_reg_EXP_TTI_PID_array_field_set( b_ptr,
                                              h_ptr,
                                              word_number,
                                              mask,
                                              field_ofs,
                                              value);
}

static INLINE UINT32 odu_rfrm_lfield_range_EX_TTI_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_EX_TTI_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_rfrm_lfield_range_EX_TTI_get", start_bit, stop_bit );
    }
    reg_value = odu_rfrm_reg_EXP_TTI_PID_array_read( b_ptr,
                                                     h_ptr,
                                                     word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_rfrm_lfield_range_EX_TTI_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_burstsize4096
 * ==================================================================================
 */

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_burstsize768
 * ==================================================================================
 */
static INLINE void odu_rfrm_lfield_EX_TTI_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value[768] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_EX_TTI_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value[768] )
{
    IOLOG( "%s ", "odu_rfrm_lfield_EX_TTI_PM_set");
    odu_rfrm_reg_EXP_TTI_PM_array_burst_write( b_ptr,
                                               h_ptr,
                                               0,
                                               768,
                                               value);
}

static INLINE void odu_rfrm_lfield_EX_TTI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value[768] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_EX_TTI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value[768] )
{
    IOLOG( "%s ", "odu_rfrm_lfield_EX_TTI_PM_get");
    odu_rfrm_reg_EXP_TTI_PM_array_burst_read( b_ptr,
                                              h_ptr,
                                              0,
                                              768,
                                              (UINT32 *)value);
}

static INLINE void odu_rfrm_lfield_range_EX_TTI_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_range_EX_TTI_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_rfrm_lfield_range_EX_TTI_PM_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_rfrm_lfield_range_EX_TTI_PM_set", start_bit, stop_bit, value);
    odu_rfrm_reg_EXP_TTI_PM_array_field_set( b_ptr,
                                             h_ptr,
                                             word_number,
                                             mask,
                                             field_ofs,
                                             value);
}

static INLINE UINT32 odu_rfrm_lfield_range_EX_TTI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_EX_TTI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_rfrm_lfield_range_EX_TTI_PM_get", start_bit, stop_bit );
    }
    reg_value = odu_rfrm_reg_EXP_TTI_PM_array_read( b_ptr,
                                                    h_ptr,
                                                    word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_rfrm_lfield_range_EX_TTI_PM_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_rfrm_lfield_FTFL_get( odu_rfrm_buffer_t *b_ptr,
                                             odu_rfrm_handle_t *h_ptr,
                                             UINT32 value[768] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_FTFL_get( odu_rfrm_buffer_t *b_ptr,
                                             odu_rfrm_handle_t *h_ptr,
                                             UINT32 value[768] )
{
    IOLOG( "%s ", "odu_rfrm_lfield_FTFL_get");
    odu_rfrm_reg_FTFL_array_burst_read( b_ptr,
                                        h_ptr,
                                        0,
                                        768,
                                        (UINT32 *)value);
}

static INLINE UINT32 odu_rfrm_lfield_range_FTFL_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_FTFL_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_rfrm_lfield_range_FTFL_get", start_bit, stop_bit );
    }
    reg_value = odu_rfrm_reg_FTFL_array_read( b_ptr,
                                              h_ptr,
                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_rfrm_lfield_range_FTFL_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_rfrm_lfield_AC_APS_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32 value[768] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_AC_APS_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32 value[768] )
{
    IOLOG( "%s ", "odu_rfrm_lfield_AC_APS_get");
    odu_rfrm_reg_AC_APS_PCC_array_burst_read( b_ptr,
                                              h_ptr,
                                              0,
                                              768,
                                              (UINT32 *)value);
}

static INLINE UINT32 odu_rfrm_lfield_range_AC_APS_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_AC_APS_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_rfrm_lfield_range_AC_APS_get", start_bit, stop_bit );
    }
    reg_value = odu_rfrm_reg_AC_APS_PCC_array_read( b_ptr,
                                                    h_ptr,
                                                    word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_rfrm_lfield_range_AC_APS_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size256
 * ==================================================================================
 */
static INLINE void odu_rfrm_field_LTC_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_LTC_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_LTC_ACT_DIS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_LTC_ACT_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_LTC_ACT_DIS_set", N, value );

    /* ((0x0000cc00 + (N) * 0x40) bits 27) field LTC_ACT_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_LTC_ACT_DIS_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_LTC_ACT_DIS_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_LTC_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_LTC_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_LTC_ACT_DIS_get", N, 255);
    /* ((0x0000cc00 + (N) * 0x40) bits 27) field LTC_ACT_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_LTC_ACT_DIS_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_LTC_ACT_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_LTC_ACT_DIS_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TIM_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TIM_ACT_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TIM_ACT_DIS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TIM_ACT_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TIM_ACT_DIS_set", N, value );

    /* ((0x0000cc00 + (N) * 0x40) bits 26) field TIM_ACT_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_ACT_DIS_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_ACT_DIS_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_TIM_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TIM_ACT_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TIM_ACT_DIS_get", N, 255);
    /* ((0x0000cc00 + (N) * 0x40) bits 26) field TIM_ACT_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_ACT_DIS_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_ACT_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TIM_ACT_DIS_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TIM_DET_MO_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TIM_DET_MO_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TIM_DET_MO_set", N, 255);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TIM_DET_MO_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TIM_DET_MO_set", N, value );

    /* ((0x0000cc00 + (N) * 0x40) bits 25:24) field TIM_DET_MO of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_TIM_DET_MO_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TIM_DET_MO_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TIM_DET_MO_get", N, 255);
    /* ((0x0000cc00 + (N) * 0x40) bits 25:24) field TIM_DET_MO of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TIM_DET_MO_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TIM_DET_MO_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TIM_DET_MO_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TIM_DET_MO_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TIM_DET_MO_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TIM_DET_MO_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TIM_DET_MO_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000cc00 + (N) * 0x40) bits 25:24) field TIM_DET_MO of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
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
        /* ((0x0000cc00 + (N) * 0x40) bits 25:24) field TIM_DET_MO of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
        odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_OFF + subfield_offset),
                                                      ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TIM_DET_MO_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TIM_DET_MO_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TIM_DET_MO_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TIM_DET_MO_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TIM_DET_MO_get", stop_bit, 1 );
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
    /* ((0x0000cc00 + (N) * 0x40) bits 25:24) field TIM_DET_MO of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_MSK, ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_TIM_DET_MO_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TIM_DET_MO_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_DEG_M_set( odu_rfrm_buffer_t *b_ptr,
                                             odu_rfrm_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DEG_M_set( odu_rfrm_buffer_t *b_ptr,
                                             odu_rfrm_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DEG_M_set", N, 255);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DEG_M_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_DEG_M_set", N, value );

    /* ((0x0000cc00 + (N) * 0x40) bits 23:20) field DEG_M of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_DEG_M_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DEG_M_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DEG_M_get", N, 255);
    /* ((0x0000cc00 + (N) * 0x40) bits 23:20) field DEG_M of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DEG_M_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_DEG_M_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_DEG_M_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_DEG_M_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_DEG_M_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_DEG_M_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_DEG_M_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000cc00 + (N) * 0x40) bits 23:20) field DEG_M of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
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
        /* ((0x0000cc00 + (N) * 0x40) bits 23:20) field DEG_M of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
        odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_OFF + subfield_offset),
                                                      ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_DEG_M_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_DEG_M_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_DEG_M_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_DEG_M_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_DEG_M_get", stop_bit, 3 );
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
    /* ((0x0000cc00 + (N) * 0x40) bits 23:20) field DEG_M of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_MSK, ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_M_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_DEG_M_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_DEG_THR_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DEG_THR_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DEG_THR_set", N, 255);
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DEG_THR_set", value, 1048575);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_DEG_THR_set", N, value );

    /* ((0x0000cc00 + (N) * 0x40) bits 19:0) field DEG_THR of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_DEG_THR_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DEG_THR_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DEG_THR_get", N, 255);
    /* ((0x0000cc00 + (N) * 0x40) bits 19:0) field DEG_THR of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DEG_THR_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_DEG_THR_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_DEG_THR_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_DEG_THR_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_DEG_THR_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_DEG_THR_set", stop_bit, 19 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_DEG_THR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000cc00 + (N) * 0x40) bits 19:0) field DEG_THR of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
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
        /* ((0x0000cc00 + (N) * 0x40) bits 19:0) field DEG_THR of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
        odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_OFF + subfield_offset),
                                                      ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_DEG_THR_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_DEG_THR_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_DEG_THR_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_DEG_THR_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_DEG_THR_get", stop_bit, 19 );
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
    /* ((0x0000cc00 + (N) * 0x40) bits 19:0) field DEG_THR of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_MSK, ODU_RFRM_REG_LAYER_CTRL0_PID_BIT_DEG_THR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_DEG_THR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_INT_EN_set( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_INT_EN_set( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_INT_EN_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_INT_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_INT_EN_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 20) field INT_EN of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_INT_EN_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_INT_EN_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_INT_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_INT_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_INT_EN_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 20) field INT_EN of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_INT_EN_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_INT_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_INT_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_AIS_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_AIS_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AIS_DIS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_AIS_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_AIS_DIS_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 19) field AIS_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_AIS_DIS_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_AIS_DIS_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_AIS_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AIS_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AIS_DIS_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 19) field AIS_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_AIS_DIS_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_AIS_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AIS_DIS_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_BW_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_BW_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_BW_DIS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_BW_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_BW_DIS_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 18) field BW_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_BW_DIS_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_BW_DIS_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_BW_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_BW_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_BW_DIS_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 18) field BW_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_BW_DIS_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_BW_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_BW_DIS_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TSD_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TSD_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TSD_DIS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TSD_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TSD_DIS_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 17) field TSD_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSD_DIS_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSD_DIS_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_TSD_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TSD_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TSD_DIS_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 17) field TSD_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSD_DIS_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSD_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TSD_DIS_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TSF_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TSF_DIS_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TSF_DIS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TSF_DIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TSF_DIS_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 16) field TSF_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSF_DIS_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSF_DIS_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_TSF_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TSF_DIS_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TSF_DIS_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 16) field TSF_DIS of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSF_DIS_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TSF_DIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TSF_DIS_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_MODE_set( odu_rfrm_buffer_t *b_ptr,
                                            odu_rfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_MODE_set( odu_rfrm_buffer_t *b_ptr,
                                            odu_rfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_MODE_set", N, 255);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_MODE_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_MODE_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 15:14) field MODE of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_MODE_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_MODE_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_MODE_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 15:14) field MODE of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_MODE_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_MODE_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_MODE_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_MODE_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_MODE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000cc04 + (N) * 0x40) bits 15:14) field MODE of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
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
        /* ((0x0000cc04 + (N) * 0x40) bits 15:14) field MODE of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
        odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_OFF + subfield_offset),
                                                      ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_MODE_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_MODE_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_MODE_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_MODE_get", stop_bit, 1 );
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
    /* ((0x0000cc04 + (N) * 0x40) bits 15:14) field MODE of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_MSK, ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_MODE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM_MAINT_SIG_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM_MAINT_SIG_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM_MAINT_SIG_set", N, 255);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM_MAINT_SIG_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM_MAINT_SIG_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 13:12) field TCM_MAINT_SIG of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_TCM_MAINT_SIG_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM_MAINT_SIG_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM_MAINT_SIG_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 13:12) field TCM_MAINT_SIG of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM_MAINT_SIG_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TCM_MAINT_SIG_set( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TCM_MAINT_SIG_set( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM_MAINT_SIG_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM_MAINT_SIG_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM_MAINT_SIG_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM_MAINT_SIG_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000cc04 + (N) * 0x40) bits 13:12) field TCM_MAINT_SIG of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
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
        /* ((0x0000cc04 + (N) * 0x40) bits 13:12) field TCM_MAINT_SIG of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
        odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_OFF + subfield_offset),
                                                      ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TCM_MAINT_SIG_get( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TCM_MAINT_SIG_get( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM_MAINT_SIG_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM_MAINT_SIG_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM_MAINT_SIG_get", stop_bit, 1 );
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
    /* ((0x0000cc04 + (N) * 0x40) bits 13:12) field TCM_MAINT_SIG of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_MSK, ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_TCM_MAINT_SIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM_MAINT_SIG_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_PARENT_IS_DCI_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PARENT_IS_DCI_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PARENT_IS_DCI_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PARENT_IS_DCI_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_PARENT_IS_DCI_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 8) field PARENT_IS_DCI of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_IS_DCI_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_IS_DCI_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_PARENT_IS_DCI_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PARENT_IS_DCI_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PARENT_IS_DCI_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 8) field PARENT_IS_DCI of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_IS_DCI_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_IS_DCI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PARENT_IS_DCI_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_PARENT_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PARENT_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PARENT_POOL_ID_set", N, 255);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PARENT_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_PARENT_POOL_ID_set", N, value );

    /* ((0x0000cc04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_MSK,
                                                  ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_OFF,
                                                  value);
}

static INLINE UINT32 odu_rfrm_field_PARENT_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PARENT_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PARENT_POOL_ID_get", N, 255);
    /* ((0x0000cc04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PARENT_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_PARENT_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_PARENT_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PARENT_POOL_ID_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PARENT_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PARENT_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PARENT_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000cc04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
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
        /* ((0x0000cc04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
        odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      subfield_mask << (ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_OFF + subfield_offset),
                                                      ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_PARENT_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_PARENT_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PARENT_POOL_ID_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PARENT_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PARENT_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x0000cc04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PID index N=0..255 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PID_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_MSK, ODU_RFRM_REG_LAYER_CTRL1_PID_BIT_PARENT_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PARENT_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PIAE_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PIAE_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PIAE_get", N, 255);
    /* ((0x0000cc20 + (N) * 0x40) bits 21) field PIAE of register PMC_ODU_RFRM_REG_NEAR_END_PMON_PID index N=0..255 */
    reg_value = odu_rfrm_reg_NEAR_END_PMON_PID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PIAE_MSK) >> ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PIAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PIAE_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PN_DS_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PN_DS_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PN_DS_get", N, 255);
    /* ((0x0000cc20 + (N) * 0x40) bits 20) field PN_DS of register PMC_ODU_RFRM_REG_NEAR_END_PMON_PID index N=0..255 */
    reg_value = odu_rfrm_reg_NEAR_END_PMON_PID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_DS_MSK) >> ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_DS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PN_DS_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PN_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PN_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PN_EBC_get", N, 255);
    /* ((0x0000cc20 + (N) * 0x40) bits 19:0) field PN_EBC of register PMC_ODU_RFRM_REG_NEAR_END_PMON_PID index N=0..255 */
    reg_value = odu_rfrm_reg_NEAR_END_PMON_PID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    value = (reg_value & ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_EBC_MSK) >> ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_EBC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PN_EBC_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_PN_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_PN_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PN_EBC_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PN_EBC_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PN_EBC_get", stop_bit, 19 );
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
    /* ((0x0000cc20 + (N) * 0x40) bits 19:0) field PN_EBC of register PMC_ODU_RFRM_REG_NEAR_END_PMON_PID index N=0..255 */
    reg_value = odu_rfrm_reg_NEAR_END_PMON_PID_array_read( b_ptr,
                                                           h_ptr,
                                                           N);
    field_value = (reg_value & ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_EBC_MSK)
                  >> ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_EBC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_EBC_MSK, ODU_RFRM_REG_NEAR_END_PMON_PID_BIT_PN_EBC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PN_EBC_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PBIAE_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PBIAE_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PBIAE_get", N, 255);
    /* ((0x0000cc24 + (N) * 0x40) bits 21) field PBIAE of register PMC_ODU_RFRM_REG_FAR_END_PMON_PID index N=0..255 */
    reg_value = odu_rfrm_reg_FAR_END_PMON_PID_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PBIAE_MSK) >> ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PBIAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PBIAE_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PF_DS_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PF_DS_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PF_DS_get", N, 255);
    /* ((0x0000cc24 + (N) * 0x40) bits 20) field PF_DS of register PMC_ODU_RFRM_REG_FAR_END_PMON_PID index N=0..255 */
    reg_value = odu_rfrm_reg_FAR_END_PMON_PID_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_DS_MSK) >> ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_DS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PF_DS_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PF_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PF_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PF_EBC_get", N, 255);
    /* ((0x0000cc24 + (N) * 0x40) bits 19:0) field PF_EBC of register PMC_ODU_RFRM_REG_FAR_END_PMON_PID index N=0..255 */
    reg_value = odu_rfrm_reg_FAR_END_PMON_PID_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_EBC_MSK) >> ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_EBC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PF_EBC_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_PF_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_PF_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PF_EBC_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PF_EBC_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PF_EBC_get", stop_bit, 19 );
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
    /* ((0x0000cc24 + (N) * 0x40) bits 19:0) field PF_EBC of register PMC_ODU_RFRM_REG_FAR_END_PMON_PID index N=0..255 */
    reg_value = odu_rfrm_reg_FAR_END_PMON_PID_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_EBC_MSK)
                  >> ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_EBC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_EBC_MSK, ODU_RFRM_REG_FAR_END_PMON_PID_BIT_PF_EBC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PF_EBC_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ABDI_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ABDI_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ABDI_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 31) field ABDI_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ABDI_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ABDI_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ABDI_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AAIS_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AAIS_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AAIS_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 30) field AAIS_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_AAIS_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_AAIS_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AAIS_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ATSD_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ATSD_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ATSD_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 29) field ATSD_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSD_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSD_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ATSD_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ATSF_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ATSF_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ATSF_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 28) field ATSF_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSF_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSF_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ATSF_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_CI_SSF_V_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_CI_SSF_V_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_CI_SSF_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 25) field CI_SSF_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_CI_SSF_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_CI_SSF_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_CI_SSF_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DIAE_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DIAE_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DIAE_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 24) field DIAE_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DIAE_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DIAE_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DIAE_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DLTC_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLTC_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DLTC_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 23) field DLTC_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLTC_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLTC_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DLTC_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DBIAE_V_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DBIAE_V_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DBIAE_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 22) field DBIAE_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBIAE_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBIAE_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DBIAE_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DBDI_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DBDI_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DBDI_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 21) field DBDI_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBDI_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBDI_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DBDI_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DDEG_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DDEG_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DDEG_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 20) field DDEG_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DDEG_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DDEG_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DDEG_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DTIM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DTIM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DTIM_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 19) field DTIM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DTIM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DTIM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DTIM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DAIS_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DAIS_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DAIS_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 18) field DAIS_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DAIS_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DAIS_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DAIS_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DOCI_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DOCI_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DOCI_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 17) field DOCI_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DOCI_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DOCI_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DOCI_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DLCK_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLCK_V_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DLCK_V_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 16) field DLCK_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLCK_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLCK_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DLCK_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ABDI_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ABDI_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ABDI_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 15) field ABDI_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ABDI_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ABDI_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ABDI_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AAIS_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AAIS_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AAIS_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 14) field AAIS_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_AAIS_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_AAIS_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AAIS_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ATSD_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ATSD_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ATSD_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 13) field ATSD_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSD_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSD_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ATSD_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ATSF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ATSF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ATSF_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 12) field ATSF_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSF_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_ATSF_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ATSF_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DIAE_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DIAE_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DIAE_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 8) field DIAE_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DIAE_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DIAE_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DIAE_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DLTC_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLTC_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DLTC_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 7) field DLTC_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLTC_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLTC_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DLTC_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DBIAE_I_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DBIAE_I_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DBIAE_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 6) field DBIAE_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBIAE_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBIAE_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DBIAE_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DBDI_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DBDI_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DBDI_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 5) field DBDI_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBDI_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DBDI_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DBDI_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DDEG_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DDEG_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DDEG_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 4) field DDEG_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DDEG_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DDEG_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DDEG_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DTIM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DTIM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DTIM_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 3) field DTIM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DTIM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DTIM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DTIM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DAIS_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DAIS_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DAIS_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 2) field DAIS_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DAIS_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DAIS_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DAIS_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DOCI_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DOCI_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DOCI_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 1) field DOCI_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DOCI_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DOCI_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DOCI_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DLCK_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLCK_I_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DLCK_I_get", N, 255);
    /* ((0x0000cc28 + (N) * 0x40) bits 0) field DLCK_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PID index N=0..255 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLCK_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PID_BIT_DLCK_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DLCK_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_FE_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_FE_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FE_EBC_get", N, 255);
    /* ((0x0000cc2c + (N) * 0x40) bits 28:16) field FE_EBC of register PMC_ODU_RFRM_REG_NE_FE_EBC_PID index N=0..255 */
    reg_value = odu_rfrm_reg_NE_FE_EBC_PID_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_NE_FE_EBC_PID_BIT_FE_EBC_MSK) >> ODU_RFRM_REG_NE_FE_EBC_PID_BIT_FE_EBC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_FE_EBC_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_FE_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_FE_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_FE_EBC_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_FE_EBC_get", stop_bit, start_bit );
    if (stop_bit > 12) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_FE_EBC_get", stop_bit, 12 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 12) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 12;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x0000cc2c + (N) * 0x40) bits 28:16) field FE_EBC of register PMC_ODU_RFRM_REG_NE_FE_EBC_PID index N=0..255 */
    reg_value = odu_rfrm_reg_NE_FE_EBC_PID_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & ODU_RFRM_REG_NE_FE_EBC_PID_BIT_FE_EBC_MSK)
                  >> ODU_RFRM_REG_NE_FE_EBC_PID_BIT_FE_EBC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_NE_FE_EBC_PID_BIT_FE_EBC_MSK, ODU_RFRM_REG_NE_FE_EBC_PID_BIT_FE_EBC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_FE_EBC_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_NE_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_NE_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_NE_EBC_get", N, 255);
    /* ((0x0000cc2c + (N) * 0x40) bits 12:0) field NE_EBC of register PMC_ODU_RFRM_REG_NE_FE_EBC_PID index N=0..255 */
    reg_value = odu_rfrm_reg_NE_FE_EBC_PID_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_NE_FE_EBC_PID_BIT_NE_EBC_MSK) >> ODU_RFRM_REG_NE_FE_EBC_PID_BIT_NE_EBC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_NE_EBC_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_NE_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_NE_EBC_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
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

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_NE_EBC_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_NE_EBC_get", stop_bit, start_bit );
    if (stop_bit > 12) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_NE_EBC_get", stop_bit, 12 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 12) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 12;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x0000cc2c + (N) * 0x40) bits 12:0) field NE_EBC of register PMC_ODU_RFRM_REG_NE_FE_EBC_PID index N=0..255 */
    reg_value = odu_rfrm_reg_NE_FE_EBC_PID_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & ODU_RFRM_REG_NE_FE_EBC_PID_BIT_NE_EBC_MSK)
                  >> ODU_RFRM_REG_NE_FE_EBC_PID_BIT_NE_EBC_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_NE_FE_EBC_PID_BIT_NE_EBC_MSK, ODU_RFRM_REG_NE_FE_EBC_PID_BIT_NE_EBC_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_NE_EBC_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void odu_rfrm_field_TIM_ACT_DIS_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TIM_ACT_DIS_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TIM_ACT_DIS_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TIM_ACT_DIS_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TIM_ACT_DIS_PM_set", N, value );

    /* ((0x00010c00 + (N) * 0x40) bits 26) field TIM_ACT_DIS_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_ACT_DIS_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_ACT_DIS_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_TIM_ACT_DIS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TIM_ACT_DIS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TIM_ACT_DIS_PM_get", N, 95);
    /* ((0x00010c00 + (N) * 0x40) bits 26) field TIM_ACT_DIS_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_ACT_DIS_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_ACT_DIS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TIM_ACT_DIS_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TIM_DET_MO_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TIM_DET_MO_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TIM_DET_MO_PM_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TIM_DET_MO_PM_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TIM_DET_MO_PM_set", N, value );

    /* ((0x00010c00 + (N) * 0x40) bits 25:24) field TIM_DET_MO_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_TIM_DET_MO_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TIM_DET_MO_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TIM_DET_MO_PM_get", N, 95);
    /* ((0x00010c00 + (N) * 0x40) bits 25:24) field TIM_DET_MO_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TIM_DET_MO_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TIM_DET_MO_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TIM_DET_MO_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TIM_DET_MO_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TIM_DET_MO_PM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TIM_DET_MO_PM_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TIM_DET_MO_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c00 + (N) * 0x40) bits 25:24) field TIM_DET_MO_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
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
        /* ((0x00010c00 + (N) * 0x40) bits 25:24) field TIM_DET_MO_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
        odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_OFF + subfield_offset),
                                                     ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TIM_DET_MO_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TIM_DET_MO_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TIM_DET_MO_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TIM_DET_MO_PM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TIM_DET_MO_PM_get", stop_bit, 1 );
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
    /* ((0x00010c00 + (N) * 0x40) bits 25:24) field TIM_DET_MO_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_MSK, ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_TIM_DET_MO_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TIM_DET_MO_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_DEG_M_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DEG_M_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DEG_M_PM_set", N, 95);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DEG_M_PM_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_DEG_M_PM_set", N, value );

    /* ((0x00010c00 + (N) * 0x40) bits 23:20) field DEG_M_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_DEG_M_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DEG_M_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DEG_M_PM_get", N, 95);
    /* ((0x00010c00 + (N) * 0x40) bits 23:20) field DEG_M_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DEG_M_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_DEG_M_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_DEG_M_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_DEG_M_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_DEG_M_PM_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_DEG_M_PM_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_DEG_M_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c00 + (N) * 0x40) bits 23:20) field DEG_M_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
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
        /* ((0x00010c00 + (N) * 0x40) bits 23:20) field DEG_M_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
        odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_OFF + subfield_offset),
                                                     ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_DEG_M_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_DEG_M_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_DEG_M_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_DEG_M_PM_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_DEG_M_PM_get", stop_bit, 3 );
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
    /* ((0x00010c00 + (N) * 0x40) bits 23:20) field DEG_M_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_MSK, ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_M_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_DEG_M_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_DEG_THR_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_DEG_THR_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DEG_THR_PM_set", N, 95);
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_DEG_THR_PM_set", value, 1048575);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_DEG_THR_PM_set", N, value );

    /* ((0x00010c00 + (N) * 0x40) bits 19:0) field DEG_THR_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_DEG_THR_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DEG_THR_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DEG_THR_PM_get", N, 95);
    /* ((0x00010c00 + (N) * 0x40) bits 19:0) field DEG_THR_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DEG_THR_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_DEG_THR_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_DEG_THR_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_DEG_THR_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_DEG_THR_PM_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_DEG_THR_PM_set", stop_bit, 19 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_DEG_THR_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c00 + (N) * 0x40) bits 19:0) field DEG_THR_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
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
        /* ((0x00010c00 + (N) * 0x40) bits 19:0) field DEG_THR_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
        odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_OFF + subfield_offset),
                                                     ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_DEG_THR_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_DEG_THR_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_DEG_THR_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_DEG_THR_PM_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_DEG_THR_PM_get", stop_bit, 19 );
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
    /* ((0x00010c00 + (N) * 0x40) bits 19:0) field DEG_THR_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL0_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL0_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_MSK, ODU_RFRM_REG_LAYER_CTRL0_PM_BIT_DEG_THR_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_DEG_THR_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_INT_EN_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_INT_EN_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_INT_EN_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_INT_EN_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_INT_EN_PM_set", N, value );

    /* ((0x00010c04 + (N) * 0x40) bits 20) field INT_EN_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_INT_EN_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_INT_EN_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_INT_EN_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_INT_EN_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_INT_EN_PM_get", N, 95);
    /* ((0x00010c04 + (N) * 0x40) bits 20) field INT_EN_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_INT_EN_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_INT_EN_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_INT_EN_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_BW_DIS_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_BW_DIS_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_BW_DIS_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_BW_DIS_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_BW_DIS_PM_set", N, value );

    /* ((0x00010c04 + (N) * 0x40) bits 18) field BW_DIS_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_BW_DIS_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_BW_DIS_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_BW_DIS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_BW_DIS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_BW_DIS_PM_get", N, 95);
    /* ((0x00010c04 + (N) * 0x40) bits 18) field BW_DIS_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_BW_DIS_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_BW_DIS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_BW_DIS_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TSD_DIS_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TSD_DIS_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TSD_DIS_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TSD_DIS_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TSD_DIS_PM_set", N, value );

    /* ((0x00010c04 + (N) * 0x40) bits 17) field TSD_DIS_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSD_DIS_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSD_DIS_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_TSD_DIS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TSD_DIS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TSD_DIS_PM_get", N, 95);
    /* ((0x00010c04 + (N) * 0x40) bits 17) field TSD_DIS_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSD_DIS_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSD_DIS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TSD_DIS_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TSF_DIS_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TSF_DIS_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TSF_DIS_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TSF_DIS_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TSF_DIS_PM_set", N, value );

    /* ((0x00010c04 + (N) * 0x40) bits 16) field TSF_DIS_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSF_DIS_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSF_DIS_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_TSF_DIS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TSF_DIS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TSF_DIS_PM_get", N, 95);
    /* ((0x00010c04 + (N) * 0x40) bits 16) field TSF_DIS_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSF_DIS_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_TSF_DIS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TSF_DIS_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_MODE_PM_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_MODE_PM_set( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_MODE_PM_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_MODE_PM_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_MODE_PM_set", N, value );

    /* ((0x00010c04 + (N) * 0x40) bits 15:14) field MODE_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_MODE_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_MODE_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_MODE_PM_get", N, 95);
    /* ((0x00010c04 + (N) * 0x40) bits 15:14) field MODE_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_MODE_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_MODE_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_MODE_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_MODE_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_MODE_PM_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_MODE_PM_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_MODE_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c04 + (N) * 0x40) bits 15:14) field MODE_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
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
        /* ((0x00010c04 + (N) * 0x40) bits 15:14) field MODE_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
        odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_OFF + subfield_offset),
                                                     ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_MODE_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_MODE_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_MODE_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_MODE_PM_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_MODE_PM_get", stop_bit, 1 );
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
    /* ((0x00010c04 + (N) * 0x40) bits 15:14) field MODE_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_MSK, ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_MODE_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_MODE_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_PARENT_IS_DCI_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PARENT_IS_DCI_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PARENT_IS_DCI_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PARENT_IS_DCI_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_PARENT_IS_DCI_PM_set", N, value );

    /* ((0x00010c04 + (N) * 0x40) bits 8) field PARENT_IS_DCI_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_IS_DCI_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_IS_DCI_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_PARENT_IS_DCI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PARENT_IS_DCI_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PARENT_IS_DCI_PM_get", N, 95);
    /* ((0x00010c04 + (N) * 0x40) bits 8) field PARENT_IS_DCI_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_IS_DCI_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_IS_DCI_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PARENT_IS_DCI_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_PARENT_POOL_ID_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PARENT_POOL_ID_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PARENT_POOL_ID_PM_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PARENT_POOL_ID_PM_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_PARENT_POOL_ID_PM_set", N, value );

    /* ((0x00010c04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_MSK,
                                                 ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_OFF,
                                                 value);
}

static INLINE UINT32 odu_rfrm_field_PARENT_POOL_ID_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PARENT_POOL_ID_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                           odu_rfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PARENT_POOL_ID_PM_get", N, 95);
    /* ((0x00010c04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_MSK) >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PARENT_POOL_ID_PM_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_PARENT_POOL_ID_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_PARENT_POOL_ID_PM_set( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PARENT_POOL_ID_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PARENT_POOL_ID_PM_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PARENT_POOL_ID_PM_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PARENT_POOL_ID_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
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
        /* ((0x00010c04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
        odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     subfield_mask << (ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_OFF + subfield_offset),
                                                     ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_PARENT_POOL_ID_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_PARENT_POOL_ID_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                                 odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PARENT_POOL_ID_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PARENT_POOL_ID_PM_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PARENT_POOL_ID_PM_get", stop_bit, 7 );
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
    /* ((0x00010c04 + (N) * 0x40) bits 7:0) field PARENT_POOL_ID_PM of register PMC_ODU_RFRM_REG_LAYER_CTRL1_PM index N=0..95 */
    reg_value = odu_rfrm_reg_LAYER_CTRL1_PM_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    field_value = (reg_value & ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_MSK)
                  >> ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_MSK, ODU_RFRM_REG_LAYER_CTRL1_PM_BIT_PARENT_POOL_ID_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PARENT_POOL_ID_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM3_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM3_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM3_WR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM3_WR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM3_WR_EN_set", N, value );

    /* ((0x00010c08 + (N) * 0x40) bits 30) field TCM3_WR_EN of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_WR_EN_MSK,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_WR_EN_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM3_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM3_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM3_WR_EN_get", N, 95);
    /* ((0x00010c08 + (N) * 0x40) bits 30) field TCM3_WR_EN of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_WR_EN_MSK) >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_WR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM3_WR_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM2_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM2_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM2_WR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM2_WR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM2_WR_EN_set", N, value );

    /* ((0x00010c08 + (N) * 0x40) bits 29) field TCM2_WR_EN of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_WR_EN_MSK,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_WR_EN_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM2_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM2_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM2_WR_EN_get", N, 95);
    /* ((0x00010c08 + (N) * 0x40) bits 29) field TCM2_WR_EN of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_WR_EN_MSK) >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_WR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM2_WR_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM1_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM1_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM1_WR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM1_WR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM1_WR_EN_set", N, value );

    /* ((0x00010c08 + (N) * 0x40) bits 28) field TCM1_WR_EN of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_WR_EN_MSK,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_WR_EN_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM1_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM1_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM1_WR_EN_get", N, 95);
    /* ((0x00010c08 + (N) * 0x40) bits 28) field TCM1_WR_EN of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_WR_EN_MSK) >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_WR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM1_WR_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM3_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM3_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM3_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM3_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM3_POOL_ID_set", N, value );

    /* ((0x00010c08 + (N) * 0x40) bits 23:16) field TCM3_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_MSK,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM3_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM3_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM3_POOL_ID_get", N, 95);
    /* ((0x00010c08 + (N) * 0x40) bits 23:16) field TCM3_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_MSK) >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM3_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TCM3_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TCM3_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM3_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM3_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM3_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM3_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c08 + (N) * 0x40) bits 23:16) field TCM3_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
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
        /* ((0x00010c08 + (N) * 0x40) bits 23:16) field TCM3_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
        odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_OFF + subfield_offset),
                                                   ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TCM3_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TCM3_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM3_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM3_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM3_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x00010c08 + (N) * 0x40) bits 23:16) field TCM3_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_MSK)
                  >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_MSK, ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM3_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM3_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM2_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM2_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM2_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM2_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM2_POOL_ID_set", N, value );

    /* ((0x00010c08 + (N) * 0x40) bits 15:8) field TCM2_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_MSK,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM2_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM2_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM2_POOL_ID_get", N, 95);
    /* ((0x00010c08 + (N) * 0x40) bits 15:8) field TCM2_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_MSK) >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM2_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TCM2_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TCM2_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM2_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM2_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM2_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM2_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c08 + (N) * 0x40) bits 15:8) field TCM2_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
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
        /* ((0x00010c08 + (N) * 0x40) bits 15:8) field TCM2_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
        odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_OFF + subfield_offset),
                                                   ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TCM2_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TCM2_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM2_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM2_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM2_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x00010c08 + (N) * 0x40) bits 15:8) field TCM2_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_MSK)
                  >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_MSK, ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM2_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM2_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM1_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM1_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM1_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM1_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM1_POOL_ID_set", N, value );

    /* ((0x00010c08 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_MSK,
                                               ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM1_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM1_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM1_POOL_ID_get", N, 95);
    /* ((0x00010c08 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_MSK) >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM1_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TCM1_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TCM1_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM1_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM1_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM1_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM1_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c08 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
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
        /* ((0x00010c08 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
        odu_rfrm_reg_TCM1_2_3_PID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_OFF + subfield_offset),
                                                   ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TCM1_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TCM1_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM1_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM1_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM1_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x00010c08 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_RFRM_REG_TCM1_2_3_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM1_2_3_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_MSK)
                  >> ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_MSK, ODU_RFRM_REG_TCM1_2_3_PID_BIT_TCM1_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM1_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM6_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM6_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM6_WR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM6_WR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM6_WR_EN_set", N, value );

    /* ((0x00010c0c + (N) * 0x40) bits 30) field TCM6_WR_EN of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_WR_EN_MSK,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_WR_EN_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM6_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM6_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM6_WR_EN_get", N, 95);
    /* ((0x00010c0c + (N) * 0x40) bits 30) field TCM6_WR_EN of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_WR_EN_MSK) >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_WR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM6_WR_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM5_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM5_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM5_WR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM5_WR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM5_WR_EN_set", N, value );

    /* ((0x00010c0c + (N) * 0x40) bits 29) field TCM5_WR_EN of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_WR_EN_MSK,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_WR_EN_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM5_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM5_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM5_WR_EN_get", N, 95);
    /* ((0x00010c0c + (N) * 0x40) bits 29) field TCM5_WR_EN of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_WR_EN_MSK) >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_WR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM5_WR_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM4_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM4_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM4_WR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM4_WR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM4_WR_EN_set", N, value );

    /* ((0x00010c0c + (N) * 0x40) bits 28) field TCM4_WR_EN of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_WR_EN_MSK,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_WR_EN_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM4_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM4_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM4_WR_EN_get", N, 95);
    /* ((0x00010c0c + (N) * 0x40) bits 28) field TCM4_WR_EN of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_WR_EN_MSK) >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_WR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM4_WR_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM6_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM6_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM6_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM6_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM6_POOL_ID_set", N, value );

    /* ((0x00010c0c + (N) * 0x40) bits 23:16) field TCM6_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_MSK,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM6_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM6_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM6_POOL_ID_get", N, 95);
    /* ((0x00010c0c + (N) * 0x40) bits 23:16) field TCM6_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_MSK) >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM6_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TCM6_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TCM6_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM6_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM6_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM6_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM6_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c0c + (N) * 0x40) bits 23:16) field TCM6_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
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
        /* ((0x00010c0c + (N) * 0x40) bits 23:16) field TCM6_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
        odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_OFF + subfield_offset),
                                                   ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TCM6_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TCM6_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM6_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM6_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM6_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x00010c0c + (N) * 0x40) bits 23:16) field TCM6_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_MSK)
                  >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_MSK, ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM6_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM6_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM5_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM5_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM5_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM5_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM5_POOL_ID_set", N, value );

    /* ((0x00010c0c + (N) * 0x40) bits 15:8) field TCM5_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_MSK,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM5_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM5_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM5_POOL_ID_get", N, 95);
    /* ((0x00010c0c + (N) * 0x40) bits 15:8) field TCM5_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_MSK) >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM5_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TCM5_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TCM5_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM5_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM5_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM5_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM5_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c0c + (N) * 0x40) bits 15:8) field TCM5_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
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
        /* ((0x00010c0c + (N) * 0x40) bits 15:8) field TCM5_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
        odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_OFF + subfield_offset),
                                                   ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TCM5_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TCM5_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM5_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM5_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM5_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x00010c0c + (N) * 0x40) bits 15:8) field TCM5_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_MSK)
                  >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_MSK, ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM5_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM5_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM4_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM4_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM4_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM4_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM4_POOL_ID_set", N, value );

    /* ((0x00010c0c + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_MSK,
                                               ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_rfrm_field_TCM4_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM4_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM4_POOL_ID_get", N, 95);
    /* ((0x00010c0c + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_MSK) >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM4_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_TCM4_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_TCM4_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM4_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM4_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM4_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM4_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c0c + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
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
        /* ((0x00010c0c + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
        odu_rfrm_reg_TCM4_5_6_PID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_OFF + subfield_offset),
                                                   ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_TCM4_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TCM4_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM4_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM4_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM4_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x00010c0c + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_RFRM_REG_TCM4_5_6_PID index N=0..95 */
    reg_value = odu_rfrm_reg_TCM4_5_6_PID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_MSK)
                  >> ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_MSK, ODU_RFRM_REG_TCM4_5_6_PID_BIT_TCM4_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM4_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_FLOATING_TCM_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_FLOATING_TCM_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FLOATING_TCM_WR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_FLOATING_TCM_WR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_FLOATING_TCM_WR_EN_set", N, value );

    /* ((0x00010c10 + (N) * 0x40) bits 29) field FLOATING_TCM_WR_EN of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_WR_EN_MSK,
                                                      ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_WR_EN_OFF,
                                                      value);
}

static INLINE UINT32 odu_rfrm_field_FLOATING_TCM_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_FLOATING_TCM_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FLOATING_TCM_WR_EN_get", N, 95);
    /* ((0x00010c10 + (N) * 0x40) bits 29) field FLOATING_TCM_WR_EN of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    reg_value = odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_WR_EN_MSK) >> ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_WR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_FLOATING_TCM_WR_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_PM_NIM_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PM_NIM_WR_EN_set( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PM_NIM_WR_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PM_NIM_WR_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_PM_NIM_WR_EN_set", N, value );

    /* ((0x00010c10 + (N) * 0x40) bits 28) field PM_NIM_WR_EN of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_WR_EN_MSK,
                                                      ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_WR_EN_OFF,
                                                      value);
}

static INLINE UINT32 odu_rfrm_field_PM_NIM_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PM_NIM_WR_EN_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PM_NIM_WR_EN_get", N, 95);
    /* ((0x00010c10 + (N) * 0x40) bits 28) field PM_NIM_WR_EN of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    reg_value = odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_WR_EN_MSK) >> ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_WR_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PM_NIM_WR_EN_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_FLOATING_TCM_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_FLOATING_TCM_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FLOATING_TCM_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_FLOATING_TCM_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_FLOATING_TCM_POOL_ID_set", N, value );

    /* ((0x00010c10 + (N) * 0x40) bits 15:8) field FLOATING_TCM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_MSK,
                                                      ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_OFF,
                                                      value);
}

static INLINE UINT32 odu_rfrm_field_FLOATING_TCM_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_FLOATING_TCM_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FLOATING_TCM_POOL_ID_get", N, 95);
    /* ((0x00010c10 + (N) * 0x40) bits 15:8) field FLOATING_TCM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    reg_value = odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_MSK) >> ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_FLOATING_TCM_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_FLOATING_TCM_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_FLOATING_TCM_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                                  odu_rfrm_handle_t *h_ptr,
                                                                  UINT32  N,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit,
                                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_FLOATING_TCM_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_FLOATING_TCM_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_FLOATING_TCM_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_FLOATING_TCM_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c10 + (N) * 0x40) bits 15:8) field FLOATING_TCM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
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
        /* ((0x00010c10 + (N) * 0x40) bits 15:8) field FLOATING_TCM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
        odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_OFF + subfield_offset),
                                                          ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_FLOATING_TCM_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_FLOATING_TCM_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                                    odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_FLOATING_TCM_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_FLOATING_TCM_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_FLOATING_TCM_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x00010c10 + (N) * 0x40) bits 15:8) field FLOATING_TCM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    reg_value = odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_MSK)
                  >> ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_MSK, ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_FLOATING_TCM_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_FLOATING_TCM_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_PM_NIM_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PM_NIM_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PM_NIM_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PM_NIM_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_PM_NIM_POOL_ID_set", N, value );

    /* ((0x00010c10 + (N) * 0x40) bits 7:0) field PM_NIM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set( b_ptr,
                                                      h_ptr,
                                                      N,
                                                      ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_MSK,
                                                      ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_OFF,
                                                      value);
}

static INLINE UINT32 odu_rfrm_field_PM_NIM_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PM_NIM_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PM_NIM_POOL_ID_get", N, 95);
    /* ((0x00010c10 + (N) * 0x40) bits 7:0) field PM_NIM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    reg_value = odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_MSK) >> ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PM_NIM_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_PM_NIM_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_PM_NIM_POOL_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PM_NIM_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PM_NIM_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PM_NIM_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PM_NIM_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c10 + (N) * 0x40) bits 7:0) field PM_NIM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
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
        /* ((0x00010c10 + (N) * 0x40) bits 7:0) field PM_NIM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
        odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set( b_ptr,
                                                          h_ptr,
                                                          N,
                                                          subfield_mask << (ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_OFF + subfield_offset),
                                                          ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_OFF + subfield_offset,
                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_PM_NIM_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_PM_NIM_POOL_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                              odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PM_NIM_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PM_NIM_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PM_NIM_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x00010c10 + (N) * 0x40) bits 7:0) field PM_NIM_POOL_ID of register PMC_ODU_RFRM_REG_FLOATING_PM_NIM_PID index N=0..95 */
    reg_value = odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_MSK)
                  >> ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_MSK, ODU_RFRM_REG_FLOATING_PM_NIM_PID_BIT_PM_NIM_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PM_NIM_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM6_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM6_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM6_REMOVE_OH_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM6_REMOVE_OH_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM6_REMOVE_OH_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 25) field TCM6_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_REMOVE_OH_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_REMOVE_OH_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM6_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM6_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM6_REMOVE_OH_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 25) field TCM6_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_REMOVE_OH_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_REMOVE_OH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM6_REMOVE_OH_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM5_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM5_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM5_REMOVE_OH_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM5_REMOVE_OH_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM5_REMOVE_OH_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 24) field TCM5_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_REMOVE_OH_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_REMOVE_OH_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM5_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM5_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM5_REMOVE_OH_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 24) field TCM5_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_REMOVE_OH_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_REMOVE_OH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM5_REMOVE_OH_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM4_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM4_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM4_REMOVE_OH_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM4_REMOVE_OH_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM4_REMOVE_OH_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 23) field TCM4_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_REMOVE_OH_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_REMOVE_OH_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM4_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM4_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM4_REMOVE_OH_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 23) field TCM4_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_REMOVE_OH_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_REMOVE_OH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM4_REMOVE_OH_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM3_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM3_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM3_REMOVE_OH_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM3_REMOVE_OH_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM3_REMOVE_OH_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 22) field TCM3_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_REMOVE_OH_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_REMOVE_OH_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM3_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM3_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM3_REMOVE_OH_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 22) field TCM3_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_REMOVE_OH_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_REMOVE_OH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM3_REMOVE_OH_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM2_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM2_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM2_REMOVE_OH_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM2_REMOVE_OH_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM2_REMOVE_OH_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 21) field TCM2_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_REMOVE_OH_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_REMOVE_OH_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM2_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM2_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM2_REMOVE_OH_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 21) field TCM2_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_REMOVE_OH_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_REMOVE_OH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM2_REMOVE_OH_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM1_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM1_REMOVE_OH_set( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM1_REMOVE_OH_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM1_REMOVE_OH_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM1_REMOVE_OH_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 20) field TCM1_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_REMOVE_OH_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_REMOVE_OH_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM1_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM1_REMOVE_OH_get( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM1_REMOVE_OH_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 20) field TCM1_REMOVE_OH of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_REMOVE_OH_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_REMOVE_OH_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM1_REMOVE_OH_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_PM_NIM_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_PM_NIM_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PM_NIM_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_PM_NIM_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_PM_NIM_TRANSPARENT_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 18) field PM_NIM_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_PM_NIM_TRANSPARENT_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_PM_NIM_TRANSPARENT_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_PM_NIM_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PM_NIM_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                            odu_rfrm_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PM_NIM_TRANSPARENT_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 18) field PM_NIM_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_PM_NIM_TRANSPARENT_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_PM_NIM_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PM_NIM_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM6_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM6_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM6_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM6_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM6_TRANSPARENT_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 17) field TCM6_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_TRANSPARENT_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_TRANSPARENT_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM6_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM6_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM6_TRANSPARENT_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 17) field TCM6_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_TRANSPARENT_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM6_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM6_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM5_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM5_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM5_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM5_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM5_TRANSPARENT_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 16) field TCM5_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_TRANSPARENT_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_TRANSPARENT_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM5_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM5_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM5_TRANSPARENT_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 16) field TCM5_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_TRANSPARENT_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM5_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM5_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM4_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM4_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM4_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM4_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM4_TRANSPARENT_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 15) field TCM4_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_TRANSPARENT_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_TRANSPARENT_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM4_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM4_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM4_TRANSPARENT_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 15) field TCM4_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_TRANSPARENT_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM4_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM4_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM3_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM3_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM3_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM3_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM3_TRANSPARENT_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 14) field TCM3_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_TRANSPARENT_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_TRANSPARENT_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM3_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM3_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM3_TRANSPARENT_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 14) field TCM3_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_TRANSPARENT_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM3_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM3_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM2_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM2_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM2_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM2_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM2_TRANSPARENT_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 13) field TCM2_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_TRANSPARENT_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_TRANSPARENT_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM2_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM2_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM2_TRANSPARENT_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 13) field TCM2_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_TRANSPARENT_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM2_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM2_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_TCM1_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_TCM1_TRANSPARENT_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM1_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_TCM1_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_TCM1_TRANSPARENT_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 12) field TCM1_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_TRANSPARENT_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_TRANSPARENT_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_TCM1_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM1_TRANSPARENT_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM1_TRANSPARENT_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 12) field TCM1_TRANSPARENT of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_TRANSPARENT_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_TCM1_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM1_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_FLOATING_TCM_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_FLOATING_TCM_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FLOATING_TCM_ID_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_FLOATING_TCM_ID_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_FLOATING_TCM_ID_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 10:8) field FLOATING_TCM_ID of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_FLOATING_TCM_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_FLOATING_TCM_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FLOATING_TCM_ID_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 10:8) field FLOATING_TCM_ID of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_FLOATING_TCM_ID_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_FLOATING_TCM_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_FLOATING_TCM_ID_set( odu_rfrm_buffer_t *b_ptr,
                                                             odu_rfrm_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_FLOATING_TCM_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_FLOATING_TCM_ID_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_FLOATING_TCM_ID_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_FLOATING_TCM_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c14 + (N) * 0x40) bits 10:8) field FLOATING_TCM_ID of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
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
        /* ((0x00010c14 + (N) * 0x40) bits 10:8) field FLOATING_TCM_ID of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
        odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_OFF + subfield_offset),
                                                    ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_FLOATING_TCM_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_FLOATING_TCM_ID_get( odu_rfrm_buffer_t *b_ptr,
                                                               odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_FLOATING_TCM_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_FLOATING_TCM_ID_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_FLOATING_TCM_ID_get", stop_bit, 2 );
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
    /* ((0x00010c14 + (N) * 0x40) bits 10:8) field FLOATING_TCM_ID of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_MSK)
                  >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_MSK, ODU_RFRM_REG_CHANNEL_CTRL0_BIT_FLOATING_TCM_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_FLOATING_TCM_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_field_LAST_IS_DCI_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_LAST_IS_DCI_set( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_LAST_IS_DCI_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_LAST_IS_DCI_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_LAST_IS_DCI_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 3) field LAST_IS_DCI of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_IS_DCI_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_IS_DCI_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_LAST_IS_DCI_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_LAST_IS_DCI_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_LAST_IS_DCI_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 3) field LAST_IS_DCI of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_IS_DCI_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_IS_DCI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_LAST_IS_DCI_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_LAST_LAYER_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_LAST_LAYER_set( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_LAST_LAYER_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_rfrm_field_LAST_LAYER_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "odu_rfrm_field_LAST_LAYER_set", N, value );

    /* ((0x00010c14 + (N) * 0x40) bits 2:0) field LAST_LAYER of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_MSK,
                                                ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_OFF,
                                                value);
}

static INLINE UINT32 odu_rfrm_field_LAST_LAYER_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_LAST_LAYER_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_LAST_LAYER_get", N, 95);
    /* ((0x00010c14 + (N) * 0x40) bits 2:0) field LAST_LAYER of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_MSK) >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_LAST_LAYER_get", N, value );

    return value;
}
static INLINE void odu_rfrm_field_range_LAST_LAYER_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_rfrm_field_range_LAST_LAYER_set( odu_rfrm_buffer_t *b_ptr,
                                                        odu_rfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_LAST_LAYER_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_LAST_LAYER_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_LAST_LAYER_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_LAST_LAYER_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00010c14 + (N) * 0x40) bits 2:0) field LAST_LAYER of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
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
        /* ((0x00010c14 + (N) * 0x40) bits 2:0) field LAST_LAYER of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
        odu_rfrm_reg_CHANNEL_CTRL0_array_field_set( b_ptr,
                                                    h_ptr,
                                                    N,
                                                    subfield_mask << (ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_OFF + subfield_offset),
                                                    ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_OFF + subfield_offset,
                                                    value >> subfield_shift);
    }
}

static INLINE UINT32 odu_rfrm_field_range_LAST_LAYER_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_LAST_LAYER_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_LAST_LAYER_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_LAST_LAYER_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_LAST_LAYER_get", stop_bit, 2 );
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
    /* ((0x00010c14 + (N) * 0x40) bits 2:0) field LAST_LAYER of register PMC_ODU_RFRM_REG_CHANNEL_CTRL0 index N=0..95 */
    reg_value = odu_rfrm_reg_CHANNEL_CTRL0_array_read( b_ptr,
                                                       h_ptr,
                                                       N);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_MSK)
                  >> ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_MSK, ODU_RFRM_REG_CHANNEL_CTRL0_BIT_LAST_LAYER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_LAST_LAYER_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_GCC2_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_GCC2_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_GCC2_get", N, 95);
    /* ((0x00010c1c + (N) * 0x40) bits 31:16) field GCC2 of register PMC_ODU_RFRM_REG_ROW4_OH1 index N=0..95 */
    reg_value = odu_rfrm_reg_ROW4_OH1_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_ROW4_OH1_BIT_GCC2_MSK) >> ODU_RFRM_REG_ROW4_OH1_BIT_GCC2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_GCC2_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_GCC2_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_GCC2_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_GCC2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_GCC2_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_GCC2_get", stop_bit, 15 );
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
    /* ((0x00010c1c + (N) * 0x40) bits 31:16) field GCC2 of register PMC_ODU_RFRM_REG_ROW4_OH1 index N=0..95 */
    reg_value = odu_rfrm_reg_ROW4_OH1_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_ROW4_OH1_BIT_GCC2_MSK)
                  >> ODU_RFRM_REG_ROW4_OH1_BIT_GCC2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_ROW4_OH1_BIT_GCC2_MSK, ODU_RFRM_REG_ROW4_OH1_BIT_GCC2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_GCC2_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_GCC1_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_GCC1_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_GCC1_get", N, 95);
    /* ((0x00010c1c + (N) * 0x40) bits 15:0) field GCC1 of register PMC_ODU_RFRM_REG_ROW4_OH1 index N=0..95 */
    reg_value = odu_rfrm_reg_ROW4_OH1_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_ROW4_OH1_BIT_GCC1_MSK) >> ODU_RFRM_REG_ROW4_OH1_BIT_GCC1_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_GCC1_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_GCC1_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_GCC1_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_GCC1_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_GCC1_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_GCC1_get", stop_bit, 15 );
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
    /* ((0x00010c1c + (N) * 0x40) bits 15:0) field GCC1 of register PMC_ODU_RFRM_REG_ROW4_OH1 index N=0..95 */
    reg_value = odu_rfrm_reg_ROW4_OH1_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_ROW4_OH1_BIT_GCC1_MSK)
                  >> ODU_RFRM_REG_ROW4_OH1_BIT_GCC1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_ROW4_OH1_BIT_GCC1_MSK, ODU_RFRM_REG_ROW4_OH1_BIT_GCC1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_GCC1_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PN_DS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PN_DS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PN_DS_PM_get", N, 95);
    /* ((0x00010c20 + (N) * 0x40) bits 20) field PN_DS_PM of register PMC_ODU_RFRM_REG_NEAR_END_PMON_PM index N=0..95 */
    reg_value = odu_rfrm_reg_NEAR_END_PMON_PM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_DS_PM_MSK) >> ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_DS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PN_DS_PM_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PN_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PN_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PN_EBC_PM_get", N, 95);
    /* ((0x00010c20 + (N) * 0x40) bits 19:0) field PN_EBC_PM of register PMC_ODU_RFRM_REG_NEAR_END_PMON_PM index N=0..95 */
    reg_value = odu_rfrm_reg_NEAR_END_PMON_PM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_EBC_PM_MSK) >> ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_EBC_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PN_EBC_PM_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_PN_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_PN_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PN_EBC_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PN_EBC_PM_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PN_EBC_PM_get", stop_bit, 19 );
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
    /* ((0x00010c20 + (N) * 0x40) bits 19:0) field PN_EBC_PM of register PMC_ODU_RFRM_REG_NEAR_END_PMON_PM index N=0..95 */
    reg_value = odu_rfrm_reg_NEAR_END_PMON_PM_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    field_value = (reg_value & ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_EBC_PM_MSK)
                  >> ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_EBC_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_EBC_PM_MSK, ODU_RFRM_REG_NEAR_END_PMON_PM_BIT_PN_EBC_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PN_EBC_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PF_DS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PF_DS_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PF_DS_PM_get", N, 95);
    /* ((0x00010c24 + (N) * 0x40) bits 20) field PF_DS_PM of register PMC_ODU_RFRM_REG_FAR_END_PMON_PM index N=0..95 */
    reg_value = odu_rfrm_reg_FAR_END_PMON_PM_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_DS_PM_MSK) >> ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_DS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PF_DS_PM_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PF_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PF_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PF_EBC_PM_get", N, 95);
    /* ((0x00010c24 + (N) * 0x40) bits 19:0) field PF_EBC_PM of register PMC_ODU_RFRM_REG_FAR_END_PMON_PM index N=0..95 */
    reg_value = odu_rfrm_reg_FAR_END_PMON_PM_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_EBC_PM_MSK) >> ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_EBC_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PF_EBC_PM_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_PF_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_PF_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PF_EBC_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PF_EBC_PM_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PF_EBC_PM_get", stop_bit, 19 );
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
    /* ((0x00010c24 + (N) * 0x40) bits 19:0) field PF_EBC_PM of register PMC_ODU_RFRM_REG_FAR_END_PMON_PM index N=0..95 */
    reg_value = odu_rfrm_reg_FAR_END_PMON_PM_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    field_value = (reg_value & ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_EBC_PM_MSK)
                  >> ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_EBC_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_EBC_PM_MSK, ODU_RFRM_REG_FAR_END_PMON_PM_BIT_PF_EBC_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PF_EBC_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ABDI_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ABDI_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ABDI_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 31) field ABDI_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ABDI_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ABDI_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ABDI_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ATSD_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ATSD_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ATSD_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 29) field ATSD_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSD_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSD_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ATSD_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ATSF_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ATSF_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ATSF_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 28) field ATSF_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSF_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSF_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ATSF_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_CI_SSF_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_CI_SSF_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_CI_SSF_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 25) field CI_SSF_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_CI_SSF_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_CI_SSF_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_CI_SSF_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DLOFLOM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLOFLOM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DLOFLOM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 24) field DLOFLOM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLOFLOM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLOFLOM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DLOFLOM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_OOF_V_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_OOF_V_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_OOF_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 23) field OOF_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_OOF_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_OOF_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_OOF_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DBDI_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DBDI_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DBDI_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 21) field DBDI_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DBDI_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DBDI_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DBDI_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DDEG_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DDEG_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DDEG_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 20) field DDEG_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DDEG_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DDEG_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DDEG_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DTIM_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DTIM_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DTIM_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 19) field DTIM_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DTIM_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DTIM_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DTIM_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DAIS_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DAIS_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DAIS_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 18) field DAIS_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DAIS_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DAIS_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DAIS_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DOCI_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DOCI_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DOCI_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 17) field DOCI_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DOCI_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DOCI_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DOCI_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DLCK_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLCK_PM_V_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DLCK_PM_V_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 16) field DLCK_PM_V of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLCK_PM_V_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLCK_PM_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DLCK_PM_V_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ABDI_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ABDI_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ABDI_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 15) field ABDI_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ABDI_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ABDI_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ABDI_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ATSD_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ATSD_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ATSD_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 13) field ATSD_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSD_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSD_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ATSD_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_ATSF_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_ATSF_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_ATSF_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 12) field ATSF_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSF_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_ATSF_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_ATSF_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DLOFLOM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLOFLOM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DLOFLOM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 8) field DLOFLOM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLOFLOM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLOFLOM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DLOFLOM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_OOF_I_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_OOF_I_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_OOF_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 7) field OOF_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_OOF_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_OOF_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_OOF_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DBDI_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DBDI_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DBDI_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 5) field DBDI_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DBDI_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DBDI_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DBDI_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DDEG_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DDEG_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DDEG_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 4) field DDEG_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DDEG_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DDEG_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DDEG_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DTIM_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DTIM_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DTIM_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 3) field DTIM_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DTIM_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DTIM_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DTIM_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DAIS_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DAIS_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DAIS_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 2) field DAIS_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DAIS_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DAIS_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DAIS_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DOCI_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DOCI_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DOCI_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 1) field DOCI_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DOCI_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DOCI_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DOCI_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_DLCK_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_DLCK_PM_I_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_DLCK_PM_I_get", N, 95);
    /* ((0x00010c28 + (N) * 0x40) bits 0) field DLCK_PM_I of register PMC_ODU_RFRM_REG_DEFECTS_ACTIONS_PM index N=0..95 */
    reg_value = odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLCK_PM_I_MSK) >> ODU_RFRM_REG_DEFECTS_ACTIONS_PM_BIT_DLCK_PM_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_DLCK_PM_I_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_FE_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_FE_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_FE_EBC_PM_get", N, 95);
    /* ((0x00010c2c + (N) * 0x40) bits 28:16) field FE_EBC_PM of register PMC_ODU_RFRM_REG_NE_FE_EBC_PM index N=0..95 */
    reg_value = odu_rfrm_reg_NE_FE_EBC_PM_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_NE_FE_EBC_PM_BIT_FE_EBC_PM_MSK) >> ODU_RFRM_REG_NE_FE_EBC_PM_BIT_FE_EBC_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_FE_EBC_PM_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_FE_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_FE_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_FE_EBC_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_FE_EBC_PM_get", stop_bit, start_bit );
    if (stop_bit > 12) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_FE_EBC_PM_get", stop_bit, 12 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 12) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 12;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00010c2c + (N) * 0x40) bits 28:16) field FE_EBC_PM of register PMC_ODU_RFRM_REG_NE_FE_EBC_PM index N=0..95 */
    reg_value = odu_rfrm_reg_NE_FE_EBC_PM_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_RFRM_REG_NE_FE_EBC_PM_BIT_FE_EBC_PM_MSK)
                  >> ODU_RFRM_REG_NE_FE_EBC_PM_BIT_FE_EBC_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_NE_FE_EBC_PM_BIT_FE_EBC_PM_MSK, ODU_RFRM_REG_NE_FE_EBC_PM_BIT_FE_EBC_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_FE_EBC_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_NE_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_NE_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_NE_EBC_PM_get", N, 95);
    /* ((0x00010c2c + (N) * 0x40) bits 12:0) field NE_EBC_PM of register PMC_ODU_RFRM_REG_NE_FE_EBC_PM index N=0..95 */
    reg_value = odu_rfrm_reg_NE_FE_EBC_PM_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_RFRM_REG_NE_FE_EBC_PM_BIT_NE_EBC_PM_MSK) >> ODU_RFRM_REG_NE_FE_EBC_PM_BIT_NE_EBC_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_NE_EBC_PM_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_NE_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_NE_EBC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_NE_EBC_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_NE_EBC_PM_get", stop_bit, start_bit );
    if (stop_bit > 12) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_NE_EBC_PM_get", stop_bit, 12 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 12) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 12;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00010c2c + (N) * 0x40) bits 12:0) field NE_EBC_PM of register PMC_ODU_RFRM_REG_NE_FE_EBC_PM index N=0..95 */
    reg_value = odu_rfrm_reg_NE_FE_EBC_PM_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_RFRM_REG_NE_FE_EBC_PM_BIT_NE_EBC_PM_MSK)
                  >> ODU_RFRM_REG_NE_FE_EBC_PM_BIT_NE_EBC_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_NE_FE_EBC_PM_BIT_NE_EBC_PM_MSK, ODU_RFRM_REG_NE_FE_EBC_PM_BIT_NE_EBC_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_NE_EBC_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AC_TCM6_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AC_TCM6_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AC_TCM6_get", N, 95);
    /* ((0x00010c30 + (N) * 0x40) bits 26:24) field AC_TCM6 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM6_MSK) >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM6_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AC_TCM6_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_AC_TCM6_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_AC_TCM6_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_AC_TCM6_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_AC_TCM6_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_AC_TCM6_get", stop_bit, 2 );
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
    /* ((0x00010c30 + (N) * 0x40) bits 26:24) field AC_TCM6 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM6_MSK)
                  >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM6_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM6_MSK, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM6_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_AC_TCM6_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AC_TCM5_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AC_TCM5_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AC_TCM5_get", N, 95);
    /* ((0x00010c30 + (N) * 0x40) bits 22:20) field AC_TCM5 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM5_MSK) >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM5_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AC_TCM5_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_AC_TCM5_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_AC_TCM5_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_AC_TCM5_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_AC_TCM5_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_AC_TCM5_get", stop_bit, 2 );
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
    /* ((0x00010c30 + (N) * 0x40) bits 22:20) field AC_TCM5 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM5_MSK)
                  >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM5_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM5_MSK, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM5_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_AC_TCM5_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AC_TCM4_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AC_TCM4_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AC_TCM4_get", N, 95);
    /* ((0x00010c30 + (N) * 0x40) bits 18:16) field AC_TCM4 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM4_MSK) >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM4_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AC_TCM4_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_AC_TCM4_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_AC_TCM4_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_AC_TCM4_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_AC_TCM4_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_AC_TCM4_get", stop_bit, 2 );
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
    /* ((0x00010c30 + (N) * 0x40) bits 18:16) field AC_TCM4 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM4_MSK)
                  >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM4_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM4_MSK, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM4_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_AC_TCM4_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AC_TCM3_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AC_TCM3_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AC_TCM3_get", N, 95);
    /* ((0x00010c30 + (N) * 0x40) bits 14:12) field AC_TCM3 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM3_MSK) >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM3_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AC_TCM3_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_AC_TCM3_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_AC_TCM3_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_AC_TCM3_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_AC_TCM3_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_AC_TCM3_get", stop_bit, 2 );
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
    /* ((0x00010c30 + (N) * 0x40) bits 14:12) field AC_TCM3 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM3_MSK)
                  >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM3_MSK, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_AC_TCM3_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AC_TCM2_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AC_TCM2_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AC_TCM2_get", N, 95);
    /* ((0x00010c30 + (N) * 0x40) bits 10:8) field AC_TCM2 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM2_MSK) >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AC_TCM2_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_AC_TCM2_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_AC_TCM2_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_AC_TCM2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_AC_TCM2_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_AC_TCM2_get", stop_bit, 2 );
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
    /* ((0x00010c30 + (N) * 0x40) bits 10:8) field AC_TCM2 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM2_MSK)
                  >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM2_MSK, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_AC_TCM2_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AC_TCM1_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AC_TCM1_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AC_TCM1_get", N, 95);
    /* ((0x00010c30 + (N) * 0x40) bits 6:4) field AC_TCM1 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM1_MSK) >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM1_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AC_TCM1_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_AC_TCM1_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_AC_TCM1_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_AC_TCM1_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_AC_TCM1_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_AC_TCM1_get", stop_bit, 2 );
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
    /* ((0x00010c30 + (N) * 0x40) bits 6:4) field AC_TCM1 of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_TCM1_MSK)
                  >> ODU_RFRM_REG_AC_STAT_BIT_AC_TCM1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM1_MSK, ODU_RFRM_REG_AC_STAT_BIT_AC_TCM1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_AC_TCM1_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_AC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_AC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                               odu_rfrm_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_AC_PM_get", N, 95);
    /* ((0x00010c30 + (N) * 0x40) bits 2:0) field AC_PM of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_PM_MSK) >> ODU_RFRM_REG_AC_STAT_BIT_AC_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_AC_PM_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_AC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_AC_PM_get( odu_rfrm_buffer_t *b_ptr,
                                                     odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_AC_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_AC_PM_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_AC_PM_get", stop_bit, 2 );
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
    /* ((0x00010c30 + (N) * 0x40) bits 2:0) field AC_PM of register PMC_ODU_RFRM_REG_AC_STAT index N=0..95 */
    reg_value = odu_rfrm_reg_AC_STAT_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_AC_STAT_BIT_AC_PM_MSK)
                  >> ODU_RFRM_REG_AC_STAT_BIT_AC_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_AC_STAT_BIT_AC_PM_MSK, ODU_RFRM_REG_AC_STAT_BIT_AC_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_AC_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_PM_TCM_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_PM_TCM_get( odu_rfrm_buffer_t *b_ptr,
                                                odu_rfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_PM_TCM_get", N, 95);
    /* ((0x00010c34 + (N) * 0x40) bits 31:24) field PM_TCM of register PMC_ODU_RFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_ROW2_OH_BIT_PM_TCM_MSK) >> ODU_RFRM_REG_ROW2_OH_BIT_PM_TCM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_PM_TCM_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_PM_TCM_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_PM_TCM_get( odu_rfrm_buffer_t *b_ptr,
                                                      odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_PM_TCM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_PM_TCM_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_PM_TCM_get", stop_bit, 7 );
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
    /* ((0x00010c34 + (N) * 0x40) bits 31:24) field PM_TCM of register PMC_ODU_RFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_ROW2_OH_BIT_PM_TCM_MSK)
                  >> ODU_RFRM_REG_ROW2_OH_BIT_PM_TCM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_ROW2_OH_BIT_PM_TCM_MSK, ODU_RFRM_REG_ROW2_OH_BIT_PM_TCM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_PM_TCM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_TCM_ACT_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_TCM_ACT_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_TCM_ACT_get", N, 95);
    /* ((0x00010c34 + (N) * 0x40) bits 23:16) field TCM_ACT of register PMC_ODU_RFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK) >> ODU_RFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_TCM_ACT_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_TCM_ACT_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_TCM_ACT_get( odu_rfrm_buffer_t *b_ptr,
                                                       odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_TCM_ACT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_TCM_ACT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_TCM_ACT_get", stop_bit, 7 );
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
    /* ((0x00010c34 + (N) * 0x40) bits 23:16) field TCM_ACT of register PMC_ODU_RFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK)
                  >> ODU_RFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK, ODU_RFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_TCM_ACT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_RES1_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_RES1_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_RES1_get", N, 95);
    /* ((0x00010c34 + (N) * 0x40) bits 15:0) field RES1 of register PMC_ODU_RFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_ROW2_OH_BIT_RES1_MSK) >> ODU_RFRM_REG_ROW2_OH_BIT_RES1_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_RES1_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_RES1_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_RES1_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_RES1_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_RES1_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_RES1_get", stop_bit, 15 );
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
    /* ((0x00010c34 + (N) * 0x40) bits 15:0) field RES1 of register PMC_ODU_RFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_ROW2_OH_BIT_RES1_MSK)
                  >> ODU_RFRM_REG_ROW2_OH_BIT_RES1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_ROW2_OH_BIT_RES1_MSK, ODU_RFRM_REG_ROW2_OH_BIT_RES1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_RES1_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_RES2_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_RES2_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_RES2_get", N, 95);
    /* ((0x00010c38 + (N) * 0x40) bits 31:16) field RES2 of register PMC_ODU_RFRM_REG_ROW3_4_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW3_4_OH_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_RFRM_REG_ROW3_4_OH_BIT_RES2_MSK) >> ODU_RFRM_REG_ROW3_4_OH_BIT_RES2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_RES2_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_RES2_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_RES2_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_RES2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_RES2_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_RES2_get", stop_bit, 15 );
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
    /* ((0x00010c38 + (N) * 0x40) bits 31:16) field RES2 of register PMC_ODU_RFRM_REG_ROW3_4_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW3_4_OH_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    field_value = (reg_value & ODU_RFRM_REG_ROW3_4_OH_BIT_RES2_MSK)
                  >> ODU_RFRM_REG_ROW3_4_OH_BIT_RES2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_ROW3_4_OH_BIT_RES2_MSK, ODU_RFRM_REG_ROW3_4_OH_BIT_RES2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_RES2_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_EXP_get( odu_rfrm_buffer_t *b_ptr,
                                             odu_rfrm_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_EXP_get( odu_rfrm_buffer_t *b_ptr,
                                             odu_rfrm_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_EXP_get", N, 95);
    /* ((0x00010c38 + (N) * 0x40) bits 15:0) field EXP of register PMC_ODU_RFRM_REG_ROW3_4_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW3_4_OH_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_RFRM_REG_ROW3_4_OH_BIT_EXP_MSK) >> ODU_RFRM_REG_ROW3_4_OH_BIT_EXP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_EXP_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_EXP_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_EXP_get( odu_rfrm_buffer_t *b_ptr,
                                                   odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_EXP_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_EXP_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_EXP_get", stop_bit, 15 );
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
    /* ((0x00010c38 + (N) * 0x40) bits 15:0) field EXP of register PMC_ODU_RFRM_REG_ROW3_4_OH index N=0..95 */
    reg_value = odu_rfrm_reg_ROW3_4_OH_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    field_value = (reg_value & ODU_RFRM_REG_ROW3_4_OH_BIT_EXP_MSK)
                  >> ODU_RFRM_REG_ROW3_4_OH_BIT_EXP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_ROW3_4_OH_BIT_EXP_MSK, ODU_RFRM_REG_ROW3_4_OH_BIT_EXP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_EXP_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_RES3_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_RES3_get( odu_rfrm_buffer_t *b_ptr,
                                              odu_rfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_RES3_get", N, 95);
    /* ((0x00010c3c + (N) * 0x40) bits 31:0) field RES3 of register PMC_ODU_RFRM_REG_ROW4_OH0 index N=0..95 */
    reg_value = odu_rfrm_reg_ROW4_OH0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_RFRM_REG_ROW4_OH0_BIT_RES3_MSK) >> ODU_RFRM_REG_ROW4_OH0_BIT_RES3_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_rfrm_field_RES3_get", N, value );

    return value;
}
static INLINE UINT32 odu_rfrm_field_range_RES3_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_field_range_RES3_get( odu_rfrm_buffer_t *b_ptr,
                                                    odu_rfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_rfrm_field_range_RES3_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_field_range_RES3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_field_range_RES3_get", stop_bit, 31 );
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
    /* ((0x00010c3c + (N) * 0x40) bits 31:0) field RES3 of register PMC_ODU_RFRM_REG_ROW4_OH0 index N=0..95 */
    reg_value = odu_rfrm_reg_ROW4_OH0_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_RFRM_REG_ROW4_OH0_BIT_RES3_MSK)
                  >> ODU_RFRM_REG_ROW4_OH0_BIT_RES3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_ROW4_OH0_BIT_RES3_MSK, ODU_RFRM_REG_ROW4_OH0_BIT_RES3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_field_range_RES3_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE void odu_rfrm_lfield_PM_DEF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_PM_DEF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                 odu_rfrm_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x00012404 bits 31:0) bits 0:31 use field PM_DEF_I of register PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0 */
    reg_value = odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY0_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_BIT_PM_DEF_I_MSK) >> ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_BIT_PM_DEF_I_OFF;
    value[0] |= field_value;

    /* (0x00012408 bits 31:0) bits 32:63 use field PM_DEF_I of register PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1 */
    reg_value = odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY1_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_BIT_PM_DEF_I_MSK) >> ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_BIT_PM_DEF_I_OFF;
    value[1] |= field_value;

    /* (0x0001240c bits 31:0) bits 64:95 use field PM_DEF_I of register PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2 */
    reg_value = odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY2_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_BIT_PM_DEF_I_MSK) >> ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_BIT_PM_DEF_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_rfrm_lfield_PM_DEF_I_get", value[2] , value[1] , value[0] );

}
static INLINE UINT32 odu_rfrm_lfield_range_PM_DEF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_PM_DEF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                         odu_rfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_lfield_range_PM_DEF_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_lfield_range_PM_DEF_I_get", stop_bit, 95 );
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
    /* (0x00012404 bits 31:0) bits 0:31 use field PM_DEF_I of register PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0 */
        reg_value = odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY0_read( b_ptr,
                                                                   h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_BIT_PM_DEF_I_MSK)
                      >> ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_BIT_PM_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_BIT_PM_DEF_I_MSK, ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY0_BIT_PM_DEF_I_OFF, field_value );
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
    /* (0x00012408 bits 31:0) bits 32:63 use field PM_DEF_I of register PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1 */
        reg_value = odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY1_read( b_ptr,
                                                                   h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_BIT_PM_DEF_I_MSK)
                      >> ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_BIT_PM_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_BIT_PM_DEF_I_MSK, ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY1_BIT_PM_DEF_I_OFF, field_value );
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
    /* (0x0001240c bits 31:0) bits 64:95 use field PM_DEF_I of register PMC_ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2 */
        reg_value = odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY2_read( b_ptr,
                                                                   h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_BIT_PM_DEF_I_MSK)
                      >> ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_BIT_PM_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_BIT_PM_DEF_I_MSK, ODU_RFRM_REG_CHANNEL_DEFECT_INT_SUMMARY2_BIT_PM_DEF_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_lfield_range_PM_DEF_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_rfrm_lfield_PID_DEF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void odu_rfrm_lfield_PID_DEF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                  odu_rfrm_handle_t *h_ptr,
                                                  UINT32 value[8] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<8;i++)
        value[i] = 0;

    /* (0x00012410 bits 31:0) bits 0:31 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0 */
    reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY0_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_BIT_PID_DEF_I_MSK) >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_BIT_PID_DEF_I_OFF;
    value[0] |= field_value;

    /* (0x00012414 bits 31:0) bits 32:63 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1 */
    reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY1_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_BIT_PID_DEF_I_MSK) >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_BIT_PID_DEF_I_OFF;
    value[1] |= field_value;

    /* (0x00012418 bits 31:0) bits 64:95 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2 */
    reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY2_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_BIT_PID_DEF_I_MSK) >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_BIT_PID_DEF_I_OFF;
    value[2] |= field_value;

    /* (0x0001241c bits 31:0) bits 96:127 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3 */
    reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY3_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_BIT_PID_DEF_I_MSK) >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_BIT_PID_DEF_I_OFF;
    value[3] |= field_value;

    /* (0x00012420 bits 31:0) bits 128:159 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4 */
    reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY4_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_BIT_PID_DEF_I_MSK) >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_BIT_PID_DEF_I_OFF;
    value[4] |= field_value;

    /* (0x00012424 bits 31:0) bits 160:191 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5 */
    reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY5_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_BIT_PID_DEF_I_MSK) >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_BIT_PID_DEF_I_OFF;
    value[5] |= field_value;

    /* (0x00012428 bits 31:0) bits 192:223 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6 */
    reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY6_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_BIT_PID_DEF_I_MSK) >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_BIT_PID_DEF_I_OFF;
    value[6] |= field_value;

    /* (0x0001242c bits 31:0) bits 224:255 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7 */
    reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY7_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_BIT_PID_DEF_I_MSK) >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_BIT_PID_DEF_I_OFF;
    value[7] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "odu_rfrm_lfield_PID_DEF_I_get", value[7] , value[6] , value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

}
static INLINE UINT32 odu_rfrm_lfield_range_PID_DEF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_rfrm_lfield_range_PID_DEF_I_get( odu_rfrm_buffer_t *b_ptr,
                                                          odu_rfrm_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_rfrm_lfield_range_PID_DEF_I_get", stop_bit, start_bit );
    if (stop_bit > 255) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_rfrm_lfield_range_PID_DEF_I_get", stop_bit, 255 );
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
    /* (0x00012410 bits 31:0) bits 0:31 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0 */
        reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY0_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_BIT_PID_DEF_I_MSK)
                      >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_BIT_PID_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_BIT_PID_DEF_I_MSK, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY0_BIT_PID_DEF_I_OFF, field_value );
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
    /* (0x00012414 bits 31:0) bits 32:63 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1 */
        reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY1_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_BIT_PID_DEF_I_MSK)
                      >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_BIT_PID_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_BIT_PID_DEF_I_MSK, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY1_BIT_PID_DEF_I_OFF, field_value );
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
    /* (0x00012418 bits 31:0) bits 64:95 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2 */
        reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY2_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_BIT_PID_DEF_I_MSK)
                      >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_BIT_PID_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_BIT_PID_DEF_I_MSK, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY2_BIT_PID_DEF_I_OFF, field_value );
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
    /* (0x0001241c bits 31:0) bits 96:127 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3 */
        reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY3_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_BIT_PID_DEF_I_MSK)
                      >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_BIT_PID_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_BIT_PID_DEF_I_MSK, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY3_BIT_PID_DEF_I_OFF, field_value );
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
    /* (0x00012420 bits 31:0) bits 128:159 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4 */
        reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY4_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_BIT_PID_DEF_I_MSK)
                      >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_BIT_PID_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_BIT_PID_DEF_I_MSK, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY4_BIT_PID_DEF_I_OFF, field_value );
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
    /* (0x00012424 bits 31:0) bits 160:191 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5 */
        reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY5_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_BIT_PID_DEF_I_MSK)
                      >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_BIT_PID_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_BIT_PID_DEF_I_MSK, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY5_BIT_PID_DEF_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 223) && (stop_bit >= 192)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 192) {
            subfield_offset = start_bit - 192;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 192 - start_bit;
            subfield_start_bit = 192;
        }
        if (stop_bit < 223) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 223;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00012428 bits 31:0) bits 192:223 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6 */
        reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY6_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_BIT_PID_DEF_I_MSK)
                      >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_BIT_PID_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_BIT_PID_DEF_I_MSK, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY6_BIT_PID_DEF_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 255) && (stop_bit >= 224)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 224) {
            subfield_offset = start_bit - 224;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 224 - start_bit;
            subfield_start_bit = 224;
        }
        if (stop_bit < 255) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 255;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0001242c bits 31:0) bits 224:255 use field PID_DEF_I of register PMC_ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7 */
        reg_value = odu_rfrm_reg_PID_DEFECT_INT_SUMMARY7_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_BIT_PID_DEF_I_MSK)
                      >> ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_BIT_PID_DEF_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_BIT_PID_DEF_I_MSK, ODU_RFRM_REG_PID_DEFECT_INT_SUMMARY7_BIT_PID_DEF_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_rfrm_lfield_range_PID_DEF_I_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODU_RFRM_IO_INLINE_H */
