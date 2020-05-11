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
 *     and register accessor functions for the odu_tfrm block
 *****************************************************************************/
#ifndef _ODU_TFRM_IO_INLINE_H
#define _ODU_TFRM_IO_INLINE_H

#include "odu_tfrm_loc.h"
#include "odu_tfrm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ODU_TFRM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for odu_tfrm
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
    odu_tfrm_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} odu_tfrm_buffer_t;
static INLINE void odu_tfrm_buffer_init( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void odu_tfrm_buffer_init( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "odu_tfrm_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void odu_tfrm_buffer_flush( odu_tfrm_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void odu_tfrm_buffer_flush( odu_tfrm_buffer_t *b_ptr )
{
    IOLOG( "odu_tfrm_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 odu_tfrm_reg_read( odu_tfrm_buffer_t *b_ptr,
                                        odu_tfrm_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_read( odu_tfrm_buffer_t *b_ptr,
                                        odu_tfrm_handle_t *h_ptr,
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
static INLINE void odu_tfrm_reg_write( odu_tfrm_buffer_t *b_ptr,
                                       odu_tfrm_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_write( odu_tfrm_buffer_t *b_ptr,
                                       odu_tfrm_handle_t *h_ptr,
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

static INLINE void odu_tfrm_field_set( odu_tfrm_buffer_t *b_ptr,
                                       odu_tfrm_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_set( odu_tfrm_buffer_t *b_ptr,
                                       odu_tfrm_handle_t *h_ptr,
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

static INLINE void odu_tfrm_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
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

static INLINE void odu_tfrm_burst_read( odu_tfrm_buffer_t *b_ptr,
                                        odu_tfrm_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_burst_read( odu_tfrm_buffer_t *b_ptr,
                                        odu_tfrm_handle_t *h_ptr,
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

static INLINE void odu_tfrm_burst_write( odu_tfrm_buffer_t *b_ptr,
                                         odu_tfrm_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_burst_write( odu_tfrm_buffer_t *b_ptr,
                                         odu_tfrm_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_poll( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_poll( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
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
 *  register access functions for odu_tfrm
 * ==================================================================================
 */

static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU0_write( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU0_write( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_PASSTHRU0_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU0_field_set( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU0_field_set( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_PASSTHRU0_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_PASSTHRU0_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_PASSTHRU0_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_PASSTHRU0_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU1_write( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU1_write( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_PASSTHRU1_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU1_field_set( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU1_field_set( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_PASSTHRU1_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_PASSTHRU1_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_PASSTHRU1_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_PASSTHRU1_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU2_write( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU2_write( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_PASSTHRU2_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU2_field_set( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_PASSTHRU2_field_set( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_PASSTHRU2_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_PASSTHRU2_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_PASSTHRU2_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_PASSTHRU2_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM_CTRL2_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM_CTRL2_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM_CTRL2_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_TFRM_REG_TCM_CTRL2(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM_CTRL2_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM_CTRL2_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM_CTRL2_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_ODU_TFRM_REG_TCM_CTRL2(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM_CTRL2_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM_CTRL2_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM_CTRL2_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_ODU_TFRM_REG_TCM_CTRL2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM_CTRL2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                   odu_tfrm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                   odu_tfrm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                   odu_tfrm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                   odu_tfrm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                   odu_tfrm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                   odu_tfrm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E,
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E,
                        mask,
                        PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E,
                        mask,
                        PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E,
                        mask,
                        PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E,
                        mask,
                        PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E,
                        mask,
                        PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E,
                        mask,
                        PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E,
                        mask,
                        PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_write( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E,
                        mask,
                        PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_write( odu_tfrm_buffer_t *b_ptr,
                                                                     odu_tfrm_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_write( odu_tfrm_buffer_t *b_ptr,
                                                                     odu_tfrm_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E,
                        value);
}

static INLINE void odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                         odu_tfrm_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                         odu_tfrm_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E,
                        mask,
                        PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_write( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_write( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                     odu_tfrm_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                     odu_tfrm_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                                odu_tfrm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                                odu_tfrm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_write( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_write( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                     odu_tfrm_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                     odu_tfrm_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                                odu_tfrm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                                odu_tfrm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_write( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_write( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I,
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                     odu_tfrm_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                     odu_tfrm_handle_t *h_ptr,
                                                                                     UINT32 mask,
                                                                                     UINT32 ofs,
                                                                                     UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                                odu_tfrm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                                odu_tfrm_handle_t *h_ptr,
                                                                                UINT32 mask,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_write( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I,
                        value);
}

static INLINE void odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                   odu_tfrm_handle_t *h_ptr,
                                                                                   UINT32 mask,
                                                                                   UINT32 ofs,
                                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                              odu_tfrm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_write( odu_tfrm_buffer_t *b_ptr,
                                                                       odu_tfrm_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_write( odu_tfrm_buffer_t *b_ptr,
                                                                       odu_tfrm_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I,
                        value);
}

static INLINE void odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                           odu_tfrm_handle_t *h_ptr,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_action_on_write_field_set( odu_tfrm_buffer_t *b_ptr,
                                                                                           odu_tfrm_handle_t *h_ptr,
                                                                                           UINT32 mask,
                                                                                           UINT32 ofs,
                                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_action_on_write_field_set", mask, ofs, value );
    odu_tfrm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                                      odu_tfrm_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                                      odu_tfrm_handle_t *h_ptr,
                                                                                      UINT32 mask,
                                                                                      UINT32 value,
                                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                                      UINT32 max_count,
                                                                                      UINT32 *num_failed_polls,
                                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                                             odu_tfrm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                                             odu_tfrm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                                             odu_tfrm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                                             odu_tfrm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_read( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V);

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                                             odu_tfrm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                                             odu_tfrm_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odu_tfrm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE void odu_tfrm_reg_TTI_PM_array_burst_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TTI_PM_array_burst_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value )
{
    if (len + ofs > 1536)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_tfrm_reg_TTI_PM_array_burst_write", ofs, len, 1536 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_tfrm_reg_TTI_PM_array_burst_write", 1536, ofs, len);
    odu_tfrm_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_TTI_PM(ofs),
                          len,
                          value);

}

static INLINE void odu_tfrm_reg_TTI_PM_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32 N,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TTI_PM_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32 N,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TTI_PM_array_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TTI_PM(N),
                        mask,
                        PMC_ODU_TFRM_REG_TTI_PM_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odu_tfrm_reg_TTI_PM_array_burst_read( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 ofs,
                                                         UINT32 len,
                                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TTI_PM_array_burst_read( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 ofs,
                                                         UINT32 len,
                                                         UINT32 *value )
{
    if (len + ofs > 1536)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_tfrm_reg_TTI_PM_array_burst_read", ofs, len, 1536 );
    odu_tfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_TFRM_REG_TTI_PM(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_tfrm_reg_TTI_PM_array_burst_read", 1536, ofs, len);
}

static INLINE UINT32 odu_tfrm_reg_TTI_PM_array_read( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TTI_PM_array_read( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TTI_PM(N));

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_TTI_PM_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TTI_PID_array_burst_write( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TTI_PID_array_burst_write( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value )
{
    if (len + ofs > 4096)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_tfrm_reg_TTI_PID_array_burst_write", ofs, len, 4096 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_tfrm_reg_TTI_PID_array_burst_write", 4096, ofs, len);
    odu_tfrm_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_TTI_PID(ofs),
                          len,
                          value);

}

static INLINE void odu_tfrm_reg_TTI_PID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TTI_PID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TTI_PID_array_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TTI_PID(N),
                        mask,
                        PMC_ODU_TFRM_REG_TTI_PID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odu_tfrm_reg_TTI_PID_array_burst_read( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TTI_PID_array_burst_read( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value )
{
    if (len + ofs > 4096)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_tfrm_reg_TTI_PID_array_burst_read", ofs, len, 4096 );
    odu_tfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_TFRM_REG_TTI_PID(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_tfrm_reg_TTI_PID_array_burst_read", 4096, ofs, len);
}

static INLINE UINT32 odu_tfrm_reg_TTI_PID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TTI_PID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TTI_PID(N));

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_TTI_PID_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_FTFL_array_burst_write( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32 ofs,
                                                        UINT32 len,
                                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_FTFL_array_burst_write( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32 ofs,
                                                        UINT32 len,
                                                        UINT32 *value )
{
    if (len + ofs > 768)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_tfrm_reg_FTFL_array_burst_write", ofs, len, 768 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_tfrm_reg_FTFL_array_burst_write", 768, ofs, len);
    odu_tfrm_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_FTFL(ofs),
                          len,
                          value);

}

static INLINE void odu_tfrm_reg_FTFL_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 N,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_FTFL_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 N,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_FTFL_array_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_FTFL(N),
                        mask,
                        PMC_ODU_TFRM_REG_FTFL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odu_tfrm_reg_FTFL_array_burst_read( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_FTFL_array_burst_read( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 ofs,
                                                       UINT32 len,
                                                       UINT32 *value )
{
    if (len + ofs > 768)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_tfrm_reg_FTFL_array_burst_read", ofs, len, 768 );
    odu_tfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_TFRM_REG_FTFL(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_tfrm_reg_FTFL_array_burst_read", 768, ofs, len);
}

static INLINE UINT32 odu_tfrm_reg_FTFL_array_read( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_FTFL_array_read( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_FTFL(N));

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_FTFL_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_APS_PCC_array_burst_write( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_APS_PCC_array_burst_write( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32 ofs,
                                                           UINT32 len,
                                                           UINT32 *value )
{
    if (len + ofs > 768)
        IO_RANGE_CHECK("%s burst write beyond max (ofs:%d + len:%d) > %d", "odu_tfrm_reg_APS_PCC_array_burst_write", ofs, len, 768 );
    IOLOG( "%s of %d words ofs=%d len=%d", "odu_tfrm_reg_APS_PCC_array_burst_write", 768, ofs, len);
    odu_tfrm_burst_write( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_ODU_TFRM_REG_APS_PCC(ofs),
                          len,
                          value);

}

static INLINE void odu_tfrm_reg_APS_PCC_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_APS_PCC_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_APS_PCC_array_field_set", mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_APS_PCC(N),
                        mask,
                        PMC_ODU_TFRM_REG_APS_PCC_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE void odu_tfrm_reg_APS_PCC_array_burst_read( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_APS_PCC_array_burst_read( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32 ofs,
                                                          UINT32 len,
                                                          UINT32 *value )
{
    if (len + ofs > 768)
        IO_RANGE_CHECK("%s burst read beyond max (ofs:%d + len:%d) > %d", "odu_tfrm_reg_APS_PCC_array_burst_read", ofs, len, 768 );
    odu_tfrm_burst_read( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_ODU_TFRM_REG_APS_PCC(ofs),
                         len,
                         value);

    IOLOG( "%s of %d words ofs=%d len=%d", "odu_tfrm_reg_APS_PCC_array_burst_read", 768, ofs, len);
}

static INLINE UINT32 odu_tfrm_reg_APS_PCC_array_read( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_APS_PCC_array_read( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_APS_PCC(N));

    IOLOG( "%s -> 0x%08x;", "odu_tfrm_reg_APS_PCC_array_read", reg_value);
    return reg_value;
}

static INLINE void odu_tfrm_reg_DM_PID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_DM_PID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_DM_PID_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_DM_PID(N),
                        value);
}

static INLINE void odu_tfrm_reg_DM_PID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_DM_PID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_DM_PID_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_DM_PID(N),
                        mask,
                        PMC_ODU_TFRM_REG_DM_PID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_DM_PID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_DM_PID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_DM_PID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_DM_PID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM_CTRL1_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM_CTRL1_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM_CTRL1_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM_CTRL1(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM_CTRL1_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM_CTRL1_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM_CTRL1_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM_CTRL1(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM_CTRL1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM_CTRL1_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM_CTRL1_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TCM_CTRL1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM_CTRL1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM_CTRL3_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM_CTRL3_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM_CTRL3_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM_CTRL3(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM_CTRL3_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM_CTRL3_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM_CTRL3_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM_CTRL3(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM_CTRL3_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM_CTRL3_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM_CTRL3_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TCM_CTRL3(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM_CTRL3_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_DM_PM_array_write( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_DM_PM_array_write( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_DM_PM_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_DM_PM(N),
                        value);
}

static INLINE void odu_tfrm_reg_DM_PM_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_DM_PM_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_DM_PM_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_DM_PM(N),
                        mask,
                        PMC_ODU_TFRM_REG_DM_PM_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_DM_PM_array_read( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_DM_PM_array_read( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_DM_PM(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_DM_PM_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_PM_CTRL1_array_write( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_PM_CTRL1_array_write( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_PM_CTRL1_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_PM_CTRL1(N),
                        value);
}

static INLINE void odu_tfrm_reg_PM_CTRL1_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_PM_CTRL1_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_PM_CTRL1_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_PM_CTRL1(N),
                        mask,
                        PMC_ODU_TFRM_REG_PM_CTRL1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_PM_CTRL1_array_read( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_PM_CTRL1_array_read( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_PM_CTRL1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_PM_CTRL1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_ROW2_OH_array_write( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_ROW2_OH_array_write( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_ROW2_OH_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_ROW2_OH(N),
                        value);
}

static INLINE void odu_tfrm_reg_ROW2_OH_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_ROW2_OH_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_ROW2_OH_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_ROW2_OH(N),
                        mask,
                        PMC_ODU_TFRM_REG_ROW2_OH_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_ROW2_OH_array_read( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_ROW2_OH_array_read( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_ROW2_OH(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_ROW2_OH_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_ROW3_4_OH_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_ROW3_4_OH_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_ROW3_4_OH_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_ROW3_4_OH(N),
                        value);
}

static INLINE void odu_tfrm_reg_ROW3_4_OH_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_ROW3_4_OH_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_ROW3_4_OH_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_ROW3_4_OH(N),
                        mask,
                        PMC_ODU_TFRM_REG_ROW3_4_OH_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_ROW3_4_OH_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_ROW3_4_OH_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_ROW3_4_OH(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_ROW3_4_OH_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_ROW4_OH0_array_write( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_ROW4_OH0_array_write( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_ROW4_OH0_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_ROW4_OH0(N),
                        value);
}

static INLINE void odu_tfrm_reg_ROW4_OH0_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_ROW4_OH0_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_ROW4_OH0_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_ROW4_OH0(N),
                        mask,
                        PMC_ODU_TFRM_REG_ROW4_OH0_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_ROW4_OH0_array_read( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_ROW4_OH0_array_read( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_ROW4_OH0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_ROW4_OH0_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_ROW4_OH1_array_write( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_ROW4_OH1_array_write( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_ROW4_OH1_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_ROW4_OH1(N),
                        value);
}

static INLINE void odu_tfrm_reg_ROW4_OH1_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_ROW4_OH1_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_ROW4_OH1_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_ROW4_OH1(N),
                        mask,
                        PMC_ODU_TFRM_REG_ROW4_OH1_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_ROW4_OH1_array_read( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_ROW4_OH1_array_read( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_ROW4_OH1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_ROW4_OH1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM1_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM1_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM1_POOL_ID_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM1_POOL_ID(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM1_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM1_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM1_POOL_ID_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM1_POOL_ID(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM1_POOL_ID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM1_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM1_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TCM1_POOL_ID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM1_POOL_ID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM2_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM2_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM2_POOL_ID_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM2_POOL_ID(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM2_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM2_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM2_POOL_ID_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM2_POOL_ID(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM2_POOL_ID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM2_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM2_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TCM2_POOL_ID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM2_POOL_ID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM3_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM3_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM3_POOL_ID_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM3_POOL_ID(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM3_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM3_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM3_POOL_ID_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM3_POOL_ID(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM3_POOL_ID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM3_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM3_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TCM3_POOL_ID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM3_POOL_ID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM4_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM4_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM4_POOL_ID_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM4_POOL_ID(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM4_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM4_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM4_POOL_ID_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM4_POOL_ID(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM4_POOL_ID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM4_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM4_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TCM4_POOL_ID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM4_POOL_ID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM5_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM5_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM5_POOL_ID_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM5_POOL_ID(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM5_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM5_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM5_POOL_ID_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM5_POOL_ID(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM5_POOL_ID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM5_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM5_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TCM5_POOL_ID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM5_POOL_ID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_TCM6_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM6_POOL_ID_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_TCM6_POOL_ID_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM6_POOL_ID(N),
                        value);
}

static INLINE void odu_tfrm_reg_TCM6_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_TCM6_POOL_ID_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_TCM6_POOL_ID_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_TCM6_POOL_ID(N),
                        mask,
                        PMC_ODU_TFRM_REG_TCM6_POOL_ID_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_TCM6_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_TCM6_POOL_ID_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_TCM6_POOL_ID(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_TCM6_POOL_ID_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_OH_INSERT_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_OH_INSERT_array_write( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_OH_INSERT_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_OH_INSERT(N),
                        value);
}

static INLINE void odu_tfrm_reg_OH_INSERT_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_OH_INSERT_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_OH_INSERT_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_OH_INSERT(N),
                        mask,
                        PMC_ODU_TFRM_REG_OH_INSERT_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_OH_INSERT_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_OH_INSERT_array_read( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_OH_INSERT(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_OH_INSERT_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odu_tfrm_reg_CHANNEL_CTRL_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_CTRL_array_write( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odu_tfrm_reg_CHANNEL_CTRL_array_write", value );
    odu_tfrm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_CTRL(N),
                        value);
}

static INLINE void odu_tfrm_reg_CHANNEL_CTRL_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_reg_CHANNEL_CTRL_array_field_set( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odu_tfrm_reg_CHANNEL_CTRL_array_field_set", N, mask, ofs, value );
    odu_tfrm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODU_TFRM_REG_CHANNEL_CTRL(N),
                        mask,
                        PMC_ODU_TFRM_REG_CHANNEL_CTRL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 odu_tfrm_reg_CHANNEL_CTRL_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_reg_CHANNEL_CTRL_array_read( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = odu_tfrm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_ODU_TFRM_REG_CHANNEL_CTRL(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odu_tfrm_reg_CHANNEL_CTRL_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void odu_tfrm_lfield_PASSTHRU_set( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_PASSTHRU_set( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_PASSTHRU_set", value[2] , value[1] , value[0] );

    /* (0x0000d500 bits 31:0) bits 0:31 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0 */
    odu_tfrm_reg_CHANNEL_PASSTHRU0_field_set( b_ptr,
                                              h_ptr,
                                              ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_MSK,
                                              ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_OFF,
                                              value[0]);

    /* (0x0000d504 bits 31:0) bits 32:63 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1 */
    odu_tfrm_reg_CHANNEL_PASSTHRU1_field_set( b_ptr,
                                              h_ptr,
                                              ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_MSK,
                                              ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_OFF,
                                              value[1]);

    /* (0x0000d508 bits 31:0) bits 64:95 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2 */
    odu_tfrm_reg_CHANNEL_PASSTHRU2_field_set( b_ptr,
                                              h_ptr,
                                              ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_MSK,
                                              ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_OFF,
                                              value[2]);
}

static INLINE void odu_tfrm_lfield_PASSTHRU_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_PASSTHRU_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000d500 bits 31:0) bits 0:31 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0 */
    reg_value = odu_tfrm_reg_CHANNEL_PASSTHRU0_read(  b_ptr, h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_MSK) >> ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_OFF;
    value[0] |= field_value;

    /* (0x0000d504 bits 31:0) bits 32:63 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1 */
    reg_value = odu_tfrm_reg_CHANNEL_PASSTHRU1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_MSK) >> ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_OFF;
    value[1] |= field_value;

    /* (0x0000d508 bits 31:0) bits 64:95 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2 */
    reg_value = odu_tfrm_reg_CHANNEL_PASSTHRU2_read(  b_ptr, h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_MSK) >> ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_PASSTHRU_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_tfrm_lfield_range_PASSTHRU_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_PASSTHRU_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_PASSTHRU_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_PASSTHRU_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_PASSTHRU_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d500 bits 31:0) bits 0:31 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0 */
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
        /* (0x0000d500 bits 31:0) bits 0:31 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0 */
        odu_tfrm_reg_CHANNEL_PASSTHRU0_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_OFF + subfield_offset),
                                                  ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d504 bits 31:0) bits 32:63 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1 */
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
        /* (0x0000d504 bits 31:0) bits 32:63 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1 */
        odu_tfrm_reg_CHANNEL_PASSTHRU1_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_OFF + subfield_offset),
                                                  ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d508 bits 31:0) bits 64:95 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2 */
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
        /* (0x0000d508 bits 31:0) bits 64:95 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2 */
        odu_tfrm_reg_CHANNEL_PASSTHRU2_field_set( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_OFF + subfield_offset),
                                                  ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_OFF + subfield_offset,
                                                  value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_lfield_range_PASSTHRU_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_PASSTHRU_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_PASSTHRU_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_PASSTHRU_get", stop_bit, 95 );
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
    /* (0x0000d500 bits 31:0) bits 0:31 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU0 */
        reg_value = odu_tfrm_reg_CHANNEL_PASSTHRU0_read(  b_ptr, h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_MSK)
                      >> ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_MSK, ODU_TFRM_REG_CHANNEL_PASSTHRU0_BIT_PASSTHRU_OFF, field_value );
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
    /* (0x0000d504 bits 31:0) bits 32:63 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU1 */
        reg_value = odu_tfrm_reg_CHANNEL_PASSTHRU1_read(  b_ptr, h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_MSK)
                      >> ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_MSK, ODU_TFRM_REG_CHANNEL_PASSTHRU1_BIT_PASSTHRU_OFF, field_value );
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
    /* (0x0000d508 bits 31:0) bits 64:95 use field PASSTHRU of register PMC_ODU_TFRM_REG_CHANNEL_PASSTHRU2 */
        reg_value = odu_tfrm_reg_CHANNEL_PASSTHRU2_read(  b_ptr, h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_MSK)
                      >> ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_MSK, ODU_TFRM_REG_CHANNEL_PASSTHRU2_BIT_PASSTHRU_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_PASSTHRU_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size256
 * ==================================================================================
 */
static INLINE void odu_tfrm_field_DMT_SRC_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DMT_SRC_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DMT_SRC_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DMT_SRC_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_DMT_SRC_set", N, value );

    /* ((0x00007c08 + (N) * 0x40) bits 1) field DMT_SRC of register PMC_ODU_TFRM_REG_TCM_CTRL2 index N=0..255 */
    odu_tfrm_reg_TCM_CTRL2_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_SRC_MSK,
                                            ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_SRC_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_DMT_SRC_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DMT_SRC_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DMT_SRC_get", N, 255);
    /* ((0x00007c08 + (N) * 0x40) bits 1) field DMT_SRC of register PMC_ODU_TFRM_REG_TCM_CTRL2 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL2_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_SRC_MSK) >> ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_SRC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_DMT_SRC_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_DMT_VAL_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DMT_VAL_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DMT_VAL_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DMT_VAL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_DMT_VAL_set", N, value );

    /* ((0x00007c08 + (N) * 0x40) bits 0) field DMT_VAL of register PMC_ODU_TFRM_REG_TCM_CTRL2 index N=0..255 */
    odu_tfrm_reg_TCM_CTRL2_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_VAL_MSK,
                                            ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_VAL_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_DMT_VAL_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DMT_VAL_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DMT_VAL_get", N, 255);
    /* ((0x00007c08 + (N) * 0x40) bits 0) field DMT_VAL of register PMC_ODU_TFRM_REG_TCM_CTRL2 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL2_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_VAL_MSK) >> ODU_TFRM_REG_TCM_CTRL2_BIT_DMT_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_DMT_VAL_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void odu_tfrm_lfield_IAE_E_set( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_IAE_E_set( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_IAE_E_set", value[2] , value[1] , value[0] );

    /* (0x0000d658 bits 31:0) bits 0:31 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E */
    odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_MSK,
                                                  ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_OFF,
                                                  value[0]);

    /* (0x0000d65c bits 31:0) bits 32:63 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E */
    odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_MSK,
                                                  ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_OFF,
                                                  value[1]);

    /* (0x0000d660 bits 31:0) bits 64:95 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E */
    odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_MSK,
                                                  ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_OFF,
                                                  value[2]);
}

static INLINE void odu_tfrm_lfield_IAE_E_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_IAE_E_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000d658 bits 31:0) bits 0:31 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E */
    reg_value = odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_MSK) >> ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_OFF;
    value[0] |= field_value;

    /* (0x0000d65c bits 31:0) bits 32:63 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E */
    reg_value = odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_MSK) >> ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_OFF;
    value[1] |= field_value;

    /* (0x0000d660 bits 31:0) bits 64:95 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E */
    reg_value = odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_MSK) >> ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_IAE_E_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_tfrm_lfield_range_IAE_E_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_IAE_E_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_IAE_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_IAE_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_IAE_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d658 bits 31:0) bits 0:31 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E */
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
        /* (0x0000d658 bits 31:0) bits 0:31 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E */
        odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d65c bits 31:0) bits 32:63 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E */
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
        /* (0x0000d65c bits 31:0) bits 32:63 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E */
        odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d660 bits 31:0) bits 64:95 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E */
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
        /* (0x0000d660 bits 31:0) bits 64:95 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E */
        odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_lfield_range_IAE_E_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_IAE_E_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_IAE_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_IAE_E_get", stop_bit, 95 );
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
    /* (0x0000d658 bits 31:0) bits 0:31 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E */
        reg_value = odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_MSK, ODU_TFRM_REG_CHANNEL_INT_EN0_IAE_E_BIT_IAE_E_OFF, field_value );
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
    /* (0x0000d65c bits 31:0) bits 32:63 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E */
        reg_value = odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_MSK, ODU_TFRM_REG_CHANNEL_INT_EN1_IAE_E_BIT_IAE_E_OFF, field_value );
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
    /* (0x0000d660 bits 31:0) bits 64:95 use field IAE_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E */
        reg_value = odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_MSK, ODU_TFRM_REG_CHANNEL_INT_EN2_IAE_E_BIT_IAE_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_IAE_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_lfield_DM_PM_E_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_DM_PM_E_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PM_E_set", value[2] , value[1] , value[0] );

    /* (0x0000d664 bits 31:0) bits 0:31 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E */
    odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_field_set( b_ptr,
                                                    h_ptr,
                                                    ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_MSK,
                                                    ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_OFF,
                                                    value[0]);

    /* (0x0000d668 bits 31:0) bits 32:63 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E */
    odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_field_set( b_ptr,
                                                    h_ptr,
                                                    ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_MSK,
                                                    ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_OFF,
                                                    value[1]);

    /* (0x0000d66c bits 31:0) bits 64:95 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E */
    odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_field_set( b_ptr,
                                                    h_ptr,
                                                    ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_MSK,
                                                    ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_OFF,
                                                    value[2]);
}

static INLINE void odu_tfrm_lfield_DM_PM_E_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_DM_PM_E_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000d664 bits 31:0) bits 0:31 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E */
    reg_value = odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_MSK) >> ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_OFF;
    value[0] |= field_value;

    /* (0x0000d668 bits 31:0) bits 32:63 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E */
    reg_value = odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_MSK) >> ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_OFF;
    value[1] |= field_value;

    /* (0x0000d66c bits 31:0) bits 64:95 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E */
    reg_value = odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_MSK) >> ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PM_E_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_tfrm_lfield_range_DM_PM_E_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_DM_PM_E_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PM_E_set", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PM_E_set", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PM_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d664 bits 31:0) bits 0:31 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E */
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
        /* (0x0000d664 bits 31:0) bits 0:31 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E */
        odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_OFF + subfield_offset),
                                                        ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d668 bits 31:0) bits 32:63 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E */
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
        /* (0x0000d668 bits 31:0) bits 32:63 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E */
        odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_OFF + subfield_offset),
                                                        ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d66c bits 31:0) bits 64:95 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E */
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
        /* (0x0000d66c bits 31:0) bits 64:95 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E */
        odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_OFF + subfield_offset),
                                                        ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_lfield_range_DM_PM_E_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_DM_PM_E_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PM_E_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PM_E_get", stop_bit, 95 );
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
    /* (0x0000d664 bits 31:0) bits 0:31 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E */
        reg_value = odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_MSK, ODU_TFRM_REG_CHANNEL_INT_EN3_DM_PM_E_BIT_DM_PM_E_OFF, field_value );
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
    /* (0x0000d668 bits 31:0) bits 32:63 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E */
        reg_value = odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_MSK, ODU_TFRM_REG_CHANNEL_INT_EN4_DM_PM_E_BIT_DM_PM_E_OFF, field_value );
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
    /* (0x0000d66c bits 31:0) bits 64:95 use field DM_PM_E of register PMC_ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E */
        reg_value = odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_MSK, ODU_TFRM_REG_CHANNEL_INT_EN5_DM_PM_E_BIT_DM_PM_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PM_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_lfield_DM_PID_E_set( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_DM_PID_E_set( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[8] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PID_E_set", value[7] , value[6] , value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

    /* (0x0000d670 bits 31:0) bits 0:31 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E */
    odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_MSK,
                                                  ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_OFF,
                                                  value[0]);

    /* (0x0000d674 bits 31:0) bits 32:63 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E */
    odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_MSK,
                                                  ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_OFF,
                                                  value[1]);

    /* (0x0000d678 bits 31:0) bits 64:95 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E */
    odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_MSK,
                                                  ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_OFF,
                                                  value[2]);

    /* (0x0000d67c bits 31:0) bits 96:127 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E */
    odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_MSK,
                                                  ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_OFF,
                                                  value[3]);

    /* (0x0000d680 bits 31:0) bits 128:159 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E */
    odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_MSK,
                                                  ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_OFF,
                                                  value[4]);

    /* (0x0000d684 bits 31:0) bits 160:191 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E */
    odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_MSK,
                                                  ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_OFF,
                                                  value[5]);

    /* (0x0000d688 bits 31:0) bits 192:223 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E */
    odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_MSK,
                                                  ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_OFF,
                                                  value[6]);

    /* (0x0000d68c bits 31:0) bits 224:255 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E */
    odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_field_set( b_ptr,
                                                  h_ptr,
                                                  ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_MSK,
                                                  ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_OFF,
                                                  value[7]);
}

static INLINE void odu_tfrm_lfield_DM_PID_E_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_DM_PID_E_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[8] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<8;i++)
        value[i] = 0;

    /* (0x0000d670 bits 31:0) bits 0:31 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E */
    reg_value = odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_MSK) >> ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_OFF;
    value[0] |= field_value;

    /* (0x0000d674 bits 31:0) bits 32:63 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E */
    reg_value = odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_MSK) >> ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_OFF;
    value[1] |= field_value;

    /* (0x0000d678 bits 31:0) bits 64:95 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E */
    reg_value = odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_MSK) >> ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_OFF;
    value[2] |= field_value;

    /* (0x0000d67c bits 31:0) bits 96:127 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E */
    reg_value = odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_MSK) >> ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_OFF;
    value[3] |= field_value;

    /* (0x0000d680 bits 31:0) bits 128:159 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E */
    reg_value = odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_MSK) >> ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_OFF;
    value[4] |= field_value;

    /* (0x0000d684 bits 31:0) bits 160:191 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E */
    reg_value = odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_MSK) >> ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_OFF;
    value[5] |= field_value;

    /* (0x0000d688 bits 31:0) bits 192:223 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E */
    reg_value = odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_MSK) >> ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_OFF;
    value[6] |= field_value;

    /* (0x0000d68c bits 31:0) bits 224:255 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E */
    reg_value = odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_MSK) >> ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_OFF;
    value[7] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PID_E_get", value[7] , value[6] , value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

}
static INLINE void odu_tfrm_lfield_range_DM_PID_E_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_DM_PID_E_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PID_E_set", stop_bit, start_bit );
    if (stop_bit > 255) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PID_E_set", stop_bit, 255 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PID_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d670 bits 31:0) bits 0:31 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E */
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
        /* (0x0000d670 bits 31:0) bits 0:31 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E */
        odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d674 bits 31:0) bits 32:63 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E */
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
        /* (0x0000d674 bits 31:0) bits 32:63 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E */
        odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d678 bits 31:0) bits 64:95 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E */
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
        /* (0x0000d678 bits 31:0) bits 64:95 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E */
        odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d67c bits 31:0) bits 96:127 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E */
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
        /* (0x0000d67c bits 31:0) bits 96:127 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E */
        odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d680 bits 31:0) bits 128:159 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E */
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
        /* (0x0000d680 bits 31:0) bits 128:159 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E */
        odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d684 bits 31:0) bits 160:191 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E */
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
        /* (0x0000d684 bits 31:0) bits 160:191 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E */
        odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d688 bits 31:0) bits 192:223 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E */
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
        /* (0x0000d688 bits 31:0) bits 192:223 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E */
        odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d68c bits 31:0) bits 224:255 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E */
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
        /* (0x0000d68c bits 31:0) bits 224:255 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E */
        odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset),
                                                      ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_lfield_range_DM_PID_E_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_DM_PID_E_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PID_E_get", stop_bit, start_bit );
    if (stop_bit > 255) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PID_E_get", stop_bit, 255 );
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
    /* (0x0000d670 bits 31:0) bits 0:31 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E */
        reg_value = odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_MSK)
                      >> ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_MSK, ODU_TFRM_REG_POOL_INT_EN0_DM_PID_E_BIT_DM_PID_E_OFF, field_value );
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
    /* (0x0000d674 bits 31:0) bits 32:63 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E */
        reg_value = odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_MSK)
                      >> ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_MSK, ODU_TFRM_REG_POOL_INT_EN1_DM_PID_E_BIT_DM_PID_E_OFF, field_value );
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
    /* (0x0000d678 bits 31:0) bits 64:95 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E */
        reg_value = odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_MSK)
                      >> ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_MSK, ODU_TFRM_REG_POOL_INT_EN2_DM_PID_E_BIT_DM_PID_E_OFF, field_value );
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
    /* (0x0000d67c bits 31:0) bits 96:127 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E */
        reg_value = odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_MSK)
                      >> ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_MSK, ODU_TFRM_REG_POOL_INT_EN3_DM_PID_E_BIT_DM_PID_E_OFF, field_value );
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
    /* (0x0000d680 bits 31:0) bits 128:159 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E */
        reg_value = odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_MSK)
                      >> ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_MSK, ODU_TFRM_REG_POOL_INT_EN4_DM_PID_E_BIT_DM_PID_E_OFF, field_value );
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
    /* (0x0000d684 bits 31:0) bits 160:191 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E */
        reg_value = odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_MSK)
                      >> ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_MSK, ODU_TFRM_REG_POOL_INT_EN5_DM_PID_E_BIT_DM_PID_E_OFF, field_value );
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
    /* (0x0000d688 bits 31:0) bits 192:223 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E */
        reg_value = odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_MSK)
                      >> ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_MSK, ODU_TFRM_REG_POOL_INT_EN6_DM_PID_E_BIT_DM_PID_E_OFF, field_value );
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
    /* (0x0000d68c bits 31:0) bits 224:255 use field DM_PID_E of register PMC_ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E */
        reg_value = odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_MSK)
                      >> ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_MSK, ODU_TFRM_REG_POOL_INT_EN7_DM_PID_E_BIT_DM_PID_E_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PID_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_DM_FIFO_UDR_E_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DM_FIFO_UDR_E_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DM_FIFO_UDR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_tfrm_field_DM_FIFO_UDR_E_set", value );

    /* (0x0000d698 bits 1) field DM_FIFO_UDR_E of register PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E */
    odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_field_set( b_ptr,
                                                          h_ptr,
                                                          ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_UDR_E_MSK,
                                                          ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_UDR_E_OFF,
                                                          value);
}

static INLINE UINT32 odu_tfrm_field_DM_FIFO_UDR_E_get( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DM_FIFO_UDR_E_get( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000d698 bits 1) field DM_FIFO_UDR_E of register PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E */
    reg_value = odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_UDR_E_MSK) >> ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_UDR_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_tfrm_field_DM_FIFO_UDR_E_get", value );

    return value;
}
static INLINE void odu_tfrm_field_DM_FIFO_OVR_E_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DM_FIFO_OVR_E_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DM_FIFO_OVR_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_tfrm_field_DM_FIFO_OVR_E_set", value );

    /* (0x0000d698 bits 0) field DM_FIFO_OVR_E of register PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E */
    odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_field_set( b_ptr,
                                                          h_ptr,
                                                          ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_OVR_E_MSK,
                                                          ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_OVR_E_OFF,
                                                          value);
}

static INLINE UINT32 odu_tfrm_field_DM_FIFO_OVR_E_get( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DM_FIFO_OVR_E_get( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000d698 bits 0) field DM_FIFO_OVR_E of register PMC_ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E */
    reg_value = odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_OVR_E_MSK) >> ODU_TFRM_REG_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_BIT_DM_FIFO_OVR_E_OFF;
    IOLOG( "%s -> 0x%08x", "odu_tfrm_field_DM_FIFO_OVR_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void odu_tfrm_lfield_IAE_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_IAE_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_IAE_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x0000d50c bits 31:0) bits 0:31 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I */
    odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_MSK,
                                                                    ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF,
                                                                    value[0]);

    /* (0x0000d510 bits 31:0) bits 32:63 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I */
    odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_MSK,
                                                                    ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF,
                                                                    value[1]);

    /* (0x0000d514 bits 31:0) bits 64:95 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I */
    odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_MSK,
                                                                    ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF,
                                                                    value[2]);
}

static INLINE void odu_tfrm_lfield_IAE_I_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_IAE_I_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000d50c bits 31:0) bits 0:31 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I */
    reg_value = odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_MSK) >> ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF;
    value[0] |= field_value;

    /* (0x0000d510 bits 31:0) bits 32:63 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I */
    reg_value = odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_MSK) >> ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF;
    value[1] |= field_value;

    /* (0x0000d514 bits 31:0) bits 64:95 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I */
    reg_value = odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_MSK) >> ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_IAE_I_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_tfrm_lfield_range_IAE_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_IAE_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                             odu_tfrm_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_IAE_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_IAE_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_IAE_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d50c bits 31:0) bits 0:31 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I */
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
        /* (0x0000d50c bits 31:0) bits 0:31 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I */
        odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d510 bits 31:0) bits 32:63 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I */
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
        /* (0x0000d510 bits 31:0) bits 32:63 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I */
        odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d514 bits 31:0) bits 64:95 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I */
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
        /* (0x0000d514 bits 31:0) bits 64:95 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I */
        odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_lfield_range_IAE_I_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_IAE_I_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_IAE_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_IAE_I_get", stop_bit, 95 );
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
    /* (0x0000d50c bits 31:0) bits 0:31 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I */
        reg_value = odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_MSK, ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF, field_value );
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
    /* (0x0000d510 bits 31:0) bits 32:63 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I */
        reg_value = odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_MSK, ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF, field_value );
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
    /* (0x0000d514 bits 31:0) bits 64:95 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I */
        reg_value = odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_MSK, ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_IAE_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                     odu_tfrm_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                     odu_tfrm_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_IAE_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_IAE_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_IAE_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d50c bits 31:0) bits 0:31 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I */
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
        /* (0x0000d50c bits 31:0) bits 0:31 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I */
        return odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d510 bits 31:0) bits 32:63 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I */
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
        /* (0x0000d510 bits 31:0) bits 32:63 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I */
        return odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d514 bits 31:0) bits 64:95 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I */
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
        /* (0x0000d514 bits 31:0) bits 64:95 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I */
        return odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_IAE_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_IAE_I_poll", value[2] , value[1] , value[0] );

    /* (0x0000d50c bits 31:0) bits 0:31 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I */
    return odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_MSK,
                                                      (value[0]<<ODU_TFRM_REG_CHANNEL_INT_STAT0_IAE_I_BIT_IAE_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d510 bits 31:0) bits 32:63 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I */
    return odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_MSK,
                                                      (value[1]<<ODU_TFRM_REG_CHANNEL_INT_STAT1_IAE_I_BIT_IAE_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d514 bits 31:0) bits 64:95 use field IAE_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I */
    return odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_MSK,
                                                      (value[2]<<ODU_TFRM_REG_CHANNEL_INT_STAT2_IAE_I_BIT_IAE_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void odu_tfrm_lfield_DM_PM_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_DM_PM_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 value[3] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PM_I_set_to_clear", value[2] , value[1] , value[0] );

    /* (0x0000d518 bits 31:0) bits 0:31 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I */
    odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_action_on_write_field_set( b_ptr,
                                                                      h_ptr,
                                                                      ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_MSK,
                                                                      ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF,
                                                                      value[0]);

    /* (0x0000d51c bits 31:0) bits 32:63 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I */
    odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_action_on_write_field_set( b_ptr,
                                                                      h_ptr,
                                                                      ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_MSK,
                                                                      ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF,
                                                                      value[1]);

    /* (0x0000d520 bits 31:0) bits 64:95 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I */
    odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_action_on_write_field_set( b_ptr,
                                                                      h_ptr,
                                                                      ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_MSK,
                                                                      ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF,
                                                                      value[2]);
}

static INLINE void odu_tfrm_lfield_DM_PM_I_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_DM_PM_I_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000d518 bits 31:0) bits 0:31 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I */
    reg_value = odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_MSK) >> ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF;
    value[0] |= field_value;

    /* (0x0000d51c bits 31:0) bits 32:63 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I */
    reg_value = odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_MSK) >> ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF;
    value[1] |= field_value;

    /* (0x0000d520 bits 31:0) bits 64:95 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I */
    reg_value = odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_read( b_ptr,
                                                             h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_MSK) >> ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PM_I_get", value[2] , value[1] , value[0] );

}
static INLINE void odu_tfrm_lfield_range_DM_PM_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_DM_PM_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PM_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PM_I_set_to_clear", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PM_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d518 bits 31:0) bits 0:31 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I */
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
        /* (0x0000d518 bits 31:0) bits 0:31 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I */
        odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_action_on_write_field_set( b_ptr,
                                                                          h_ptr,
                                                                          subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                                          ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d51c bits 31:0) bits 32:63 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I */
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
        /* (0x0000d51c bits 31:0) bits 32:63 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I */
        odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_action_on_write_field_set( b_ptr,
                                                                          h_ptr,
                                                                          subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                                          ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d520 bits 31:0) bits 64:95 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I */
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
        /* (0x0000d520 bits 31:0) bits 64:95 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I */
        odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_action_on_write_field_set( b_ptr,
                                                                          h_ptr,
                                                                          subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                                          ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset,
                                                                          value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_lfield_range_DM_PM_I_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_DM_PM_I_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PM_I_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PM_I_get", stop_bit, 95 );
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
    /* (0x0000d518 bits 31:0) bits 0:31 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I */
        reg_value = odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_MSK, ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF, field_value );
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
    /* (0x0000d51c bits 31:0) bits 32:63 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I */
        reg_value = odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_MSK, ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF, field_value );
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
    /* (0x0000d520 bits 31:0) bits 64:95 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I */
        reg_value = odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_read( b_ptr,
                                                                 h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_MSK, ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PM_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                       odu_tfrm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                       odu_tfrm_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PM_I_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PM_I_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PM_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d518 bits 31:0) bits 0:31 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I */
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
        /* (0x0000d518 bits 31:0) bits 0:31 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I */
        return odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                            value << (ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d51c bits 31:0) bits 32:63 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I */
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
        /* (0x0000d51c bits 31:0) bits 32:63 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I */
        return odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                            value << (ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d520 bits 31:0) bits 64:95 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I */
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
        /* (0x0000d520 bits 31:0) bits 64:95 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I */
        return odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_poll( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                            value << (ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF + subfield_offset),
                                                            cmp,
                                                            max_count,
                                                            num_failed_polls,
                                                            delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 value[3],
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_DM_PM_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                 odu_tfrm_handle_t *h_ptr,
                                                                 UINT32 value[3],
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PM_I_poll", value[2] , value[1] , value[0] );

    /* (0x0000d518 bits 31:0) bits 0:31 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I */
    return odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_poll( b_ptr,
                                                        h_ptr,
                                                        ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_MSK,
                                                        (value[0]<<ODU_TFRM_REG_CHANNEL_INT_STAT3_DM_PM_I_BIT_DM_PM_I_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);

    /* (0x0000d51c bits 31:0) bits 32:63 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I */
    return odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_poll( b_ptr,
                                                        h_ptr,
                                                        ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_MSK,
                                                        (value[1]<<ODU_TFRM_REG_CHANNEL_INT_STAT4_DM_PM_I_BIT_DM_PM_I_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);

    /* (0x0000d520 bits 31:0) bits 64:95 use field DM_PM_I of register PMC_ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I */
    return odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_poll( b_ptr,
                                                        h_ptr,
                                                        ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_MSK,
                                                        (value[2]<<ODU_TFRM_REG_CHANNEL_INT_STAT5_DM_PM_I_BIT_DM_PM_I_OFF),
                                                        cmp,
                                                        max_count,
                                                        num_failed_polls,
                                                        delay_between_polls_in_microseconds);
}

static INLINE void odu_tfrm_lfield_DM_PID_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_DM_PID_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32 value[8] )
{
    IOLOG( "%s <= 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PID_I_set_to_clear", value[7] , value[6] , value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

    /* (0x0000d524 bits 31:0) bits 0:31 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I */
    odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_MSK,
                                                                    ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF,
                                                                    value[0]);

    /* (0x0000d528 bits 31:0) bits 32:63 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I */
    odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_MSK,
                                                                    ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF,
                                                                    value[1]);

    /* (0x0000d52c bits 31:0) bits 64:95 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I */
    odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_MSK,
                                                                    ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF,
                                                                    value[2]);

    /* (0x0000d530 bits 31:0) bits 96:127 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I */
    odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_MSK,
                                                                    ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF,
                                                                    value[3]);

    /* (0x0000d534 bits 31:0) bits 128:159 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I */
    odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_MSK,
                                                                    ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF,
                                                                    value[4]);

    /* (0x0000d538 bits 31:0) bits 160:191 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I */
    odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_MSK,
                                                                    ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF,
                                                                    value[5]);

    /* (0x0000d53c bits 31:0) bits 192:223 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I */
    odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_MSK,
                                                                    ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF,
                                                                    value[6]);

    /* (0x0000d540 bits 31:0) bits 224:255 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I */
    odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                    h_ptr,
                                                                    ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_MSK,
                                                                    ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF,
                                                                    value[7]);
}

static INLINE void odu_tfrm_lfield_DM_PID_I_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[8] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_DM_PID_I_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32 value[8] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<8;i++)
        value[i] = 0;

    /* (0x0000d524 bits 31:0) bits 0:31 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I */
    reg_value = odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_MSK) >> ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF;
    value[0] |= field_value;

    /* (0x0000d528 bits 31:0) bits 32:63 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I */
    reg_value = odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_MSK) >> ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF;
    value[1] |= field_value;

    /* (0x0000d52c bits 31:0) bits 64:95 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I */
    reg_value = odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_MSK) >> ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF;
    value[2] |= field_value;

    /* (0x0000d530 bits 31:0) bits 96:127 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I */
    reg_value = odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_MSK) >> ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF;
    value[3] |= field_value;

    /* (0x0000d534 bits 31:0) bits 128:159 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I */
    reg_value = odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_MSK) >> ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF;
    value[4] |= field_value;

    /* (0x0000d538 bits 31:0) bits 160:191 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I */
    reg_value = odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_MSK) >> ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF;
    value[5] |= field_value;

    /* (0x0000d53c bits 31:0) bits 192:223 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I */
    reg_value = odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_MSK) >> ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF;
    value[6] |= field_value;

    /* (0x0000d540 bits 31:0) bits 224:255 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I */
    reg_value = odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_MSK) >> ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF;
    value[7] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PID_I_get", value[7] , value[6] , value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

}
static INLINE void odu_tfrm_lfield_range_DM_PID_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                                odu_tfrm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_DM_PID_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                                odu_tfrm_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PID_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 255) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PID_I_set_to_clear", stop_bit, 255 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PID_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d524 bits 31:0) bits 0:31 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I */
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
        /* (0x0000d524 bits 31:0) bits 0:31 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I */
        odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d528 bits 31:0) bits 32:63 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I */
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
        /* (0x0000d528 bits 31:0) bits 32:63 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I */
        odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d52c bits 31:0) bits 64:95 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I */
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
        /* (0x0000d52c bits 31:0) bits 64:95 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I */
        odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d530 bits 31:0) bits 96:127 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I */
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
        /* (0x0000d530 bits 31:0) bits 96:127 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I */
        odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d534 bits 31:0) bits 128:159 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I */
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
        /* (0x0000d534 bits 31:0) bits 128:159 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I */
        odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d538 bits 31:0) bits 160:191 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I */
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
        /* (0x0000d538 bits 31:0) bits 160:191 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I */
        odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d53c bits 31:0) bits 192:223 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I */
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
        /* (0x0000d53c bits 31:0) bits 192:223 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I */
        odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d540 bits 31:0) bits 224:255 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I */
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
        /* (0x0000d540 bits 31:0) bits 224:255 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I */
        odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_action_on_write_field_set( b_ptr,
                                                                        h_ptr,
                                                                        subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                                        ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset,
                                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_lfield_range_DM_PID_I_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_DM_PID_I_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PID_I_get", stop_bit, start_bit );
    if (stop_bit > 255) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PID_I_get", stop_bit, 255 );
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
    /* (0x0000d524 bits 31:0) bits 0:31 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I */
        reg_value = odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_MSK)
                      >> ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_MSK, ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF, field_value );
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
    /* (0x0000d528 bits 31:0) bits 32:63 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I */
        reg_value = odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_MSK)
                      >> ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_MSK, ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF, field_value );
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
    /* (0x0000d52c bits 31:0) bits 64:95 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I */
        reg_value = odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_MSK)
                      >> ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_MSK, ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF, field_value );
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
    /* (0x0000d530 bits 31:0) bits 96:127 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I */
        reg_value = odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_MSK)
                      >> ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_MSK, ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF, field_value );
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
    /* (0x0000d534 bits 31:0) bits 128:159 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I */
        reg_value = odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_MSK)
                      >> ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_MSK, ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF, field_value );
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
    /* (0x0000d538 bits 31:0) bits 160:191 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I */
        reg_value = odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_MSK)
                      >> ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_MSK, ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF, field_value );
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
    /* (0x0000d53c bits 31:0) bits 192:223 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I */
        reg_value = odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_MSK)
                      >> ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_MSK, ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF, field_value );
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
    /* (0x0000d540 bits 31:0) bits 224:255 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I */
        reg_value = odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_MSK)
                      >> ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_MSK, ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PID_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                        odu_tfrm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                        odu_tfrm_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_DM_PID_I_poll", stop_bit, start_bit );
    if (stop_bit > 255) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_DM_PID_I_poll", stop_bit, 255 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_DM_PID_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d524 bits 31:0) bits 0:31 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I */
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
        /* (0x0000d524 bits 31:0) bits 0:31 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I */
        return odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d528 bits 31:0) bits 32:63 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I */
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
        /* (0x0000d528 bits 31:0) bits 32:63 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I */
        return odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d52c bits 31:0) bits 64:95 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I */
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
        /* (0x0000d52c bits 31:0) bits 64:95 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I */
        return odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d530 bits 31:0) bits 96:127 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I */
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
        /* (0x0000d530 bits 31:0) bits 96:127 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I */
        return odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d534 bits 31:0) bits 128:159 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I */
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
        /* (0x0000d534 bits 31:0) bits 128:159 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I */
        return odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d538 bits 31:0) bits 160:191 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I */
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
        /* (0x0000d538 bits 31:0) bits 160:191 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I */
        return odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d53c bits 31:0) bits 192:223 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I */
    if ((start_bit <= 223) && (stop_bit >= 192)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 192) {
            subfield_offset = start_bit - 192;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 192;
        }
        if (stop_bit < 223) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 223;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000d53c bits 31:0) bits 192:223 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I */
        return odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d540 bits 31:0) bits 224:255 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I */
    if ((start_bit <= 255) && (stop_bit >= 224)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 224) {
            subfield_offset = start_bit - 224;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_start_bit = 224;
        }
        if (stop_bit < 255) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 255;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000d540 bits 31:0) bits 224:255 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I */
        return odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_poll( b_ptr,
                                                          h_ptr,
                                                          subfield_mask << (ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          value << (ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF + subfield_offset),
                                                          cmp,
                                                          max_count,
                                                          num_failed_polls,
                                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                  odu_tfrm_handle_t *h_ptr,
                                                                  UINT32 value[8],
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_DM_PID_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                  odu_tfrm_handle_t *h_ptr,
                                                                  UINT32 value[8],
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_DM_PID_I_poll", value[7] , value[6] , value[5] , value[4] , value[3] , value[2] , value[1] , value[0] );

    /* (0x0000d524 bits 31:0) bits 0:31 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I */
    return odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_MSK,
                                                      (value[0]<<ODU_TFRM_REG_POOL_INT_STAT0_DM_PID_I_BIT_DM_PID_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d528 bits 31:0) bits 32:63 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I */
    return odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_MSK,
                                                      (value[1]<<ODU_TFRM_REG_POOL_INT_STAT1_DM_PID_I_BIT_DM_PID_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d52c bits 31:0) bits 64:95 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I */
    return odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_MSK,
                                                      (value[2]<<ODU_TFRM_REG_POOL_INT_STAT2_DM_PID_I_BIT_DM_PID_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d530 bits 31:0) bits 96:127 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I */
    return odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_MSK,
                                                      (value[3]<<ODU_TFRM_REG_POOL_INT_STAT3_DM_PID_I_BIT_DM_PID_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d534 bits 31:0) bits 128:159 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I */
    return odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_MSK,
                                                      (value[4]<<ODU_TFRM_REG_POOL_INT_STAT4_DM_PID_I_BIT_DM_PID_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d538 bits 31:0) bits 160:191 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I */
    return odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_MSK,
                                                      (value[5]<<ODU_TFRM_REG_POOL_INT_STAT5_DM_PID_I_BIT_DM_PID_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d53c bits 31:0) bits 192:223 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I */
    return odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_MSK,
                                                      (value[6]<<ODU_TFRM_REG_POOL_INT_STAT6_DM_PID_I_BIT_DM_PID_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);

    /* (0x0000d540 bits 31:0) bits 224:255 use field DM_PID_I of register PMC_ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I */
    return odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_poll( b_ptr,
                                                      h_ptr,
                                                      ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_MSK,
                                                      (value[7]<<ODU_TFRM_REG_POOL_INT_STAT7_DM_PID_I_BIT_DM_PID_I_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE void odu_tfrm_field_DM_FIFO_UDR_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DM_FIFO_UDR_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DM_FIFO_UDR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_tfrm_field_DM_FIFO_UDR_I_set_to_clear", value );

    /* (0x0000d54c bits 1) field DM_FIFO_UDR_I of register PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I */
    odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_action_on_write_field_set( b_ptr,
                                                                            h_ptr,
                                                                            ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_UDR_I_MSK,
                                                                            ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_UDR_I_OFF,
                                                                            value);
}

static INLINE UINT32 odu_tfrm_field_DM_FIFO_UDR_I_get( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DM_FIFO_UDR_I_get( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000d54c bits 1) field DM_FIFO_UDR_I of register PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I */
    reg_value = odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_UDR_I_MSK) >> ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_UDR_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_tfrm_field_DM_FIFO_UDR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_field_DM_FIFO_UDR_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                      odu_tfrm_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_field_DM_FIFO_UDR_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                      odu_tfrm_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_tfrm_field_DM_FIFO_UDR_I_poll", value );

    /* (0x0000d54c bits 1) field DM_FIFO_UDR_I of register PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I */
    return odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_poll( b_ptr,
                                                              h_ptr,
                                                              ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_UDR_I_MSK,
                                                              (value<<ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_UDR_I_OFF),
                                                              cmp,
                                                              max_count,
                                                              num_failed_polls,
                                                              delay_between_polls_in_microseconds);
}

static INLINE void odu_tfrm_field_DM_FIFO_OVR_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DM_FIFO_OVR_I_set_to_clear( odu_tfrm_buffer_t *b_ptr,
                                                              odu_tfrm_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DM_FIFO_OVR_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odu_tfrm_field_DM_FIFO_OVR_I_set_to_clear", value );

    /* (0x0000d54c bits 0) field DM_FIFO_OVR_I of register PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I */
    odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_action_on_write_field_set( b_ptr,
                                                                            h_ptr,
                                                                            ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_OVR_I_MSK,
                                                                            ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_OVR_I_OFF,
                                                                            value);
}

static INLINE UINT32 odu_tfrm_field_DM_FIFO_OVR_I_get( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DM_FIFO_OVR_I_get( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000d54c bits 0) field DM_FIFO_OVR_I of register PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I */
    reg_value = odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_OVR_I_MSK) >> ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_OVR_I_OFF;
    IOLOG( "%s -> 0x%08x", "odu_tfrm_field_DM_FIFO_OVR_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_field_DM_FIFO_OVR_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                      odu_tfrm_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_field_DM_FIFO_OVR_I_poll( odu_tfrm_buffer_t *b_ptr,
                                                                      odu_tfrm_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odu_tfrm_field_DM_FIFO_OVR_I_poll", value );

    /* (0x0000d54c bits 0) field DM_FIFO_OVR_I of register PMC_ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I */
    return odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_poll( b_ptr,
                                                              h_ptr,
                                                              ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_OVR_I_MSK,
                                                              (value<<ODU_TFRM_REG_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_BIT_DM_FIFO_OVR_I_OFF),
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
static INLINE void odu_tfrm_lfield_IAE_V_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32 value[3] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_IAE_V_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32 value[3] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<3;i++)
        value[i] = 0;

    /* (0x0000d64c bits 31:0) bits 0:31 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V */
    reg_value = odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_MSK) >> ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_OFF;
    value[0] |= field_value;

    /* (0x0000d650 bits 31:0) bits 32:63 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V */
    reg_value = odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_MSK) >> ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_OFF;
    value[1] |= field_value;

    /* (0x0000d654 bits 31:0) bits 64:95 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V */
    reg_value = odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_MSK) >> ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_OFF;
    value[2] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_IAE_V_get", value[2] , value[1] , value[0] );

}
static INLINE UINT32 odu_tfrm_lfield_range_IAE_V_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_IAE_V_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_IAE_V_get", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_IAE_V_get", stop_bit, 95 );
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
    /* (0x0000d64c bits 31:0) bits 0:31 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V */
        reg_value = odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_MSK, ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_OFF, field_value );
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
    /* (0x0000d650 bits 31:0) bits 32:63 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V */
        reg_value = odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_MSK, ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_OFF, field_value );
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
    /* (0x0000d654 bits 31:0) bits 64:95 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V */
        reg_value = odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_read( b_ptr,
                                                              h_ptr);
        field_value = (reg_value & ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_MSK)
                      >> ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_MSK, ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_IAE_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                                     odu_tfrm_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                                     odu_tfrm_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_lfield_range_IAE_V_poll", stop_bit, start_bit );
    if (stop_bit > 95) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_lfield_range_IAE_V_poll", stop_bit, 95 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_lfield_range_IAE_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d64c bits 31:0) bits 0:31 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V */
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
        /* (0x0000d64c bits 31:0) bits 0:31 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V */
        return odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_OFF + subfield_offset),
                                                         value << (ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d650 bits 31:0) bits 32:63 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V */
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
        /* (0x0000d650 bits 31:0) bits 32:63 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V */
        return odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_OFF + subfield_offset),
                                                         value << (ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000d654 bits 31:0) bits 64:95 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V */
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
        /* (0x0000d654 bits 31:0) bits 64:95 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V */
        return odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_poll( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_OFF + subfield_offset),
                                                         value << (ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_OFF + subfield_offset),
                                                         cmp,
                                                         max_count,
                                                         num_failed_polls,
                                                         delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odu_tfrm_lfield_IAE_V_poll( odu_tfrm_buffer_t *b_ptr,
                                                               odu_tfrm_handle_t *h_ptr,
                                                               UINT32 value[3],
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x 0x%08x", "odu_tfrm_lfield_IAE_V_poll", value[2] , value[1] , value[0] );

    /* (0x0000d64c bits 31:0) bits 0:31 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V */
    return odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_poll( b_ptr,
                                                     h_ptr,
                                                     ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_MSK,
                                                     (value[0]<<ODU_TFRM_REG_CHANNEL_INT_VAL0_IAE_V_BIT_IAE_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);

    /* (0x0000d650 bits 31:0) bits 32:63 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V */
    return odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_poll( b_ptr,
                                                     h_ptr,
                                                     ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_MSK,
                                                     (value[1]<<ODU_TFRM_REG_CHANNEL_INT_VAL1_IAE_V_BIT_IAE_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);

    /* (0x0000d654 bits 31:0) bits 64:95 use field IAE_V of register PMC_ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V */
    return odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_poll( b_ptr,
                                                     h_ptr,
                                                     ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_MSK,
                                                     (value[2]<<ODU_TFRM_REG_CHANNEL_INT_VAL2_IAE_V_BIT_IAE_V_OFF),
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
static INLINE void odu_tfrm_lfield_TTI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32 value[1536] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_TTI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32 value[1536] )
{
    IOLOG( "%s ", "odu_tfrm_lfield_TTI_PM_set");
    odu_tfrm_reg_TTI_PM_array_burst_write( b_ptr,
                                           h_ptr,
                                           0,
                                           1536,
                                           value);
}

static INLINE void odu_tfrm_lfield_TTI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32 value[1536] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_TTI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32 value[1536] )
{
    IOLOG( "%s ", "odu_tfrm_lfield_TTI_PM_get");
    odu_tfrm_reg_TTI_PM_array_burst_read( b_ptr,
                                          h_ptr,
                                          0,
                                          1536,
                                          (UINT32 *)value);
}

static INLINE void odu_tfrm_lfield_range_TTI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_TTI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_tfrm_lfield_range_TTI_PM_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_tfrm_lfield_range_TTI_PM_set", start_bit, stop_bit, value);
    odu_tfrm_reg_TTI_PM_array_field_set( b_ptr,
                                         h_ptr,
                                         word_number,
                                         mask,
                                         field_ofs,
                                         value);
}

static INLINE UINT32 odu_tfrm_lfield_range_TTI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_TTI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_tfrm_lfield_range_TTI_PM_get", start_bit, stop_bit );
    }
    reg_value = odu_tfrm_reg_TTI_PM_array_read( b_ptr,
                                                h_ptr,
                                                word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_tfrm_lfield_range_TTI_PM_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_burstsize4096
 * ==================================================================================
 */
static INLINE void odu_tfrm_lfield_TTI_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32 value[4096] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_TTI_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32 value[4096] )
{
    IOLOG( "%s ", "odu_tfrm_lfield_TTI_set");
    odu_tfrm_reg_TTI_PID_array_burst_write( b_ptr,
                                            h_ptr,
                                            0,
                                            4096,
                                            value);
}

static INLINE void odu_tfrm_lfield_TTI_get( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32 value[4096] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_TTI_get( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32 value[4096] )
{
    IOLOG( "%s ", "odu_tfrm_lfield_TTI_get");
    odu_tfrm_reg_TTI_PID_array_burst_read( b_ptr,
                                           h_ptr,
                                           0,
                                           4096,
                                           (UINT32 *)value);
}

static INLINE void odu_tfrm_lfield_range_TTI_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_TTI_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_tfrm_lfield_range_TTI_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_tfrm_lfield_range_TTI_set", start_bit, stop_bit, value);
    odu_tfrm_reg_TTI_PID_array_field_set( b_ptr,
                                          h_ptr,
                                          word_number,
                                          mask,
                                          field_ofs,
                                          value);
}

static INLINE UINT32 odu_tfrm_lfield_range_TTI_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_TTI_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_tfrm_lfield_range_TTI_get", start_bit, stop_bit );
    }
    reg_value = odu_tfrm_reg_TTI_PID_array_read( b_ptr,
                                                 h_ptr,
                                                 word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_tfrm_lfield_range_TTI_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_burstsize768
 * ==================================================================================
 */
static INLINE void odu_tfrm_lfield_FTFL_set( odu_tfrm_buffer_t *b_ptr,
                                             odu_tfrm_handle_t *h_ptr,
                                             UINT32 value[768] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_FTFL_set( odu_tfrm_buffer_t *b_ptr,
                                             odu_tfrm_handle_t *h_ptr,
                                             UINT32 value[768] )
{
    IOLOG( "%s ", "odu_tfrm_lfield_FTFL_set");
    odu_tfrm_reg_FTFL_array_burst_write( b_ptr,
                                         h_ptr,
                                         0,
                                         768,
                                         value);
}

static INLINE void odu_tfrm_lfield_FTFL_get( odu_tfrm_buffer_t *b_ptr,
                                             odu_tfrm_handle_t *h_ptr,
                                             UINT32 value[768] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_FTFL_get( odu_tfrm_buffer_t *b_ptr,
                                             odu_tfrm_handle_t *h_ptr,
                                             UINT32 value[768] )
{
    IOLOG( "%s ", "odu_tfrm_lfield_FTFL_get");
    odu_tfrm_reg_FTFL_array_burst_read( b_ptr,
                                        h_ptr,
                                        0,
                                        768,
                                        (UINT32 *)value);
}

static INLINE void odu_tfrm_lfield_range_FTFL_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_FTFL_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_tfrm_lfield_range_FTFL_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_tfrm_lfield_range_FTFL_set", start_bit, stop_bit, value);
    odu_tfrm_reg_FTFL_array_field_set( b_ptr,
                                       h_ptr,
                                       word_number,
                                       mask,
                                       field_ofs,
                                       value);
}

static INLINE UINT32 odu_tfrm_lfield_range_FTFL_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_FTFL_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_tfrm_lfield_range_FTFL_get", start_bit, stop_bit );
    }
    reg_value = odu_tfrm_reg_FTFL_array_read( b_ptr,
                                              h_ptr,
                                              word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_tfrm_lfield_range_FTFL_get", start_bit, stop_bit, value );
    return value;
}

static INLINE void odu_tfrm_lfield_APS_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32 value[768] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_APS_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32 value[768] )
{
    IOLOG( "%s ", "odu_tfrm_lfield_APS_set");
    odu_tfrm_reg_APS_PCC_array_burst_write( b_ptr,
                                            h_ptr,
                                            0,
                                            768,
                                            value);
}

static INLINE void odu_tfrm_lfield_APS_get( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32 value[768] ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_APS_get( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32 value[768] )
{
    IOLOG( "%s ", "odu_tfrm_lfield_APS_get");
    odu_tfrm_reg_APS_PCC_array_burst_read( b_ptr,
                                           h_ptr,
                                           0,
                                           768,
                                           (UINT32 *)value);
}

static INLINE void odu_tfrm_lfield_range_APS_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_lfield_range_APS_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_tfrm_lfield_range_APS_set", start_bit, stop_bit );
    }
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_tfrm_lfield_range_APS_set", start_bit, stop_bit, value);
    odu_tfrm_reg_APS_PCC_array_field_set( b_ptr,
                                          h_ptr,
                                          word_number,
                                          mask,
                                          field_ofs,
                                          value);
}

static INLINE UINT32 odu_tfrm_lfield_range_APS_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_lfield_range_APS_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
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
        IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odu_tfrm_lfield_range_APS_get", start_bit, stop_bit );
    }
    reg_value = odu_tfrm_reg_APS_PCC_array_read( b_ptr,
                                                 h_ptr,
                                                 word_number);
    value = (reg_value >> field_ofs) & mask;
    IOLOG( "%s -> start_bit=%d stop_bit=%d value=0x%08x", "odu_tfrm_lfield_range_APS_get", start_bit, stop_bit, value );
    return value;
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size256
 * ==================================================================================
 */
static INLINE void odu_tfrm_field_DM_STATE_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DM_STATE_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DM_STATE_set", N, 255);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DM_STATE_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_DM_STATE_set", N, value );

    /* ((0x00007c00 + (N) * 0x40) bits 26:24) field DM_STATE of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
    odu_tfrm_reg_DM_PID_array_field_set( b_ptr,
                                         h_ptr,
                                         N,
                                         ODU_TFRM_REG_DM_PID_BIT_DM_STATE_MSK,
                                         ODU_TFRM_REG_DM_PID_BIT_DM_STATE_OFF,
                                         value);
}

static INLINE UINT32 odu_tfrm_field_DM_STATE_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DM_STATE_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DM_STATE_get", N, 255);
    /* ((0x00007c00 + (N) * 0x40) bits 26:24) field DM_STATE of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
    reg_value = odu_tfrm_reg_DM_PID_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_DM_PID_BIT_DM_STATE_MSK) >> ODU_TFRM_REG_DM_PID_BIT_DM_STATE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_DM_STATE_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_DM_STATE_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_DM_STATE_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_DM_STATE_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_DM_STATE_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_DM_STATE_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_DM_STATE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00007c00 + (N) * 0x40) bits 26:24) field DM_STATE of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
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
        /* ((0x00007c00 + (N) * 0x40) bits 26:24) field DM_STATE of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
        odu_tfrm_reg_DM_PID_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             subfield_mask << (ODU_TFRM_REG_DM_PID_BIT_DM_STATE_OFF + subfield_offset),
                                             ODU_TFRM_REG_DM_PID_BIT_DM_STATE_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_DM_STATE_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_DM_STATE_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_DM_STATE_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_DM_STATE_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_DM_STATE_get", stop_bit, 2 );
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
    /* ((0x00007c00 + (N) * 0x40) bits 26:24) field DM_STATE of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
    reg_value = odu_tfrm_reg_DM_PID_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_DM_PID_BIT_DM_STATE_MSK)
                  >> ODU_TFRM_REG_DM_PID_BIT_DM_STATE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_DM_PID_BIT_DM_STATE_MSK, ODU_TFRM_REG_DM_PID_BIT_DM_STATE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_DM_STATE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_DM_COUNT_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DM_COUNT_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DM_COUNT_set", N, 255);
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DM_COUNT_set", value, 1048575);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_DM_COUNT_set", N, value );

    /* ((0x00007c00 + (N) * 0x40) bits 19:0) field DM_COUNT of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
    odu_tfrm_reg_DM_PID_array_field_set( b_ptr,
                                         h_ptr,
                                         N,
                                         ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_MSK,
                                         ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_OFF,
                                         value);
}

static INLINE UINT32 odu_tfrm_field_DM_COUNT_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DM_COUNT_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DM_COUNT_get", N, 255);
    /* ((0x00007c00 + (N) * 0x40) bits 19:0) field DM_COUNT of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
    reg_value = odu_tfrm_reg_DM_PID_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_MSK) >> ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_DM_COUNT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_DM_COUNT_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_DM_COUNT_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_DM_COUNT_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_DM_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_DM_COUNT_set", stop_bit, 19 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_DM_COUNT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00007c00 + (N) * 0x40) bits 19:0) field DM_COUNT of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
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
        /* ((0x00007c00 + (N) * 0x40) bits 19:0) field DM_COUNT of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
        odu_tfrm_reg_DM_PID_array_field_set( b_ptr,
                                             h_ptr,
                                             N,
                                             subfield_mask << (ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_OFF + subfield_offset),
                                             ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_OFF + subfield_offset,
                                             value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_DM_COUNT_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_DM_COUNT_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_DM_COUNT_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_DM_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_DM_COUNT_get", stop_bit, 19 );
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
    /* ((0x00007c00 + (N) * 0x40) bits 19:0) field DM_COUNT of register PMC_ODU_TFRM_REG_DM_PID index N=0..255 */
    reg_value = odu_tfrm_reg_DM_PID_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_MSK)
                  >> ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_MSK, ODU_TFRM_REG_DM_PID_BIT_DM_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_DM_COUNT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_STAT_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_STAT_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_STAT_set", N, 255);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_STAT_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_STAT_set", N, value );

    /* ((0x00007c04 + (N) * 0x40) bits 2:0) field STAT of register PMC_ODU_TFRM_REG_TCM_CTRL1 index N=0..255 */
    odu_tfrm_reg_TCM_CTRL1_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_MSK,
                                            ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_STAT_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_STAT_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_STAT_get", N, 255);
    /* ((0x00007c04 + (N) * 0x40) bits 2:0) field STAT of register PMC_ODU_TFRM_REG_TCM_CTRL1 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL1_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_MSK) >> ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_STAT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_STAT_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_STAT_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_STAT_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_STAT_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_STAT_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_STAT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00007c04 + (N) * 0x40) bits 2:0) field STAT of register PMC_ODU_TFRM_REG_TCM_CTRL1 index N=0..255 */
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
        /* ((0x00007c04 + (N) * 0x40) bits 2:0) field STAT of register PMC_ODU_TFRM_REG_TCM_CTRL1 index N=0..255 */
        odu_tfrm_reg_TCM_CTRL1_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                subfield_mask << (ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_OFF + subfield_offset),
                                                ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_STAT_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_STAT_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_STAT_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_STAT_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_STAT_get", stop_bit, 2 );
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
    /* ((0x00007c04 + (N) * 0x40) bits 2:0) field STAT of register PMC_ODU_TFRM_REG_TCM_CTRL1 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL1_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    field_value = (reg_value & ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_MSK)
                  >> ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_MSK, ODU_TFRM_REG_TCM_CTRL1_BIT_STAT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_STAT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_BEI_BIAE_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_BEI_BIAE_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_BEI_BIAE_set", N, 255);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_BEI_BIAE_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_BEI_BIAE_set", N, value );

    /* ((0x00007c0c + (N) * 0x40) bits 7:4) field BEI_BIAE of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    odu_tfrm_reg_TCM_CTRL3_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_MSK,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_BEI_BIAE_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_BEI_BIAE_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_BEI_BIAE_get", N, 255);
    /* ((0x00007c0c + (N) * 0x40) bits 7:4) field BEI_BIAE of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL3_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_MSK) >> ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_BEI_BIAE_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_BEI_BIAE_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_BEI_BIAE_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_BEI_BIAE_set", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_BEI_BIAE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_BEI_BIAE_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_BEI_BIAE_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00007c0c + (N) * 0x40) bits 7:4) field BEI_BIAE of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
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
        /* ((0x00007c0c + (N) * 0x40) bits 7:4) field BEI_BIAE of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
        odu_tfrm_reg_TCM_CTRL3_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                subfield_mask << (ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_OFF + subfield_offset),
                                                ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_BEI_BIAE_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_BEI_BIAE_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_BEI_BIAE_get", N, 255);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_BEI_BIAE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_BEI_BIAE_get", stop_bit, 3 );
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
    /* ((0x00007c0c + (N) * 0x40) bits 7:4) field BEI_BIAE of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL3_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    field_value = (reg_value & ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_MSK)
                  >> ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_MSK, ODU_TFRM_REG_TCM_CTRL3_BIT_BEI_BIAE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_BEI_BIAE_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_BDI_set( odu_tfrm_buffer_t *b_ptr,
                                           odu_tfrm_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_BDI_set( odu_tfrm_buffer_t *b_ptr,
                                           odu_tfrm_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_BDI_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_BDI_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_BDI_set", N, value );

    /* ((0x00007c0c + (N) * 0x40) bits 3) field BDI of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    odu_tfrm_reg_TCM_CTRL3_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_BDI_MSK,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_BDI_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_BDI_get( odu_tfrm_buffer_t *b_ptr,
                                             odu_tfrm_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_BDI_get( odu_tfrm_buffer_t *b_ptr,
                                             odu_tfrm_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_BDI_get", N, 255);
    /* ((0x00007c0c + (N) * 0x40) bits 3) field BDI of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL3_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_TCM_CTRL3_BIT_BDI_MSK) >> ODU_TFRM_REG_TCM_CTRL3_BIT_BDI_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_BDI_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TTI_OS_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TTI_OS_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_OS_INS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TTI_OS_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TTI_OS_INS_set", N, value );

    /* ((0x00007c0c + (N) * 0x40) bits 2) field TTI_OS_INS of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    odu_tfrm_reg_TCM_CTRL3_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_OS_INS_MSK,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_OS_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_TTI_OS_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TTI_OS_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_OS_INS_get", N, 255);
    /* ((0x00007c0c + (N) * 0x40) bits 2) field TTI_OS_INS of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL3_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_OS_INS_MSK) >> ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_OS_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TTI_OS_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TTI_DAPI_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TTI_DAPI_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_DAPI_INS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TTI_DAPI_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TTI_DAPI_INS_set", N, value );

    /* ((0x00007c0c + (N) * 0x40) bits 1) field TTI_DAPI_INS of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    odu_tfrm_reg_TCM_CTRL3_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_DAPI_INS_MSK,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_DAPI_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_TTI_DAPI_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TTI_DAPI_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_DAPI_INS_get", N, 255);
    /* ((0x00007c0c + (N) * 0x40) bits 1) field TTI_DAPI_INS of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL3_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_DAPI_INS_MSK) >> ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_DAPI_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TTI_DAPI_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TTI_SAPI_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TTI_SAPI_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_SAPI_INS_set", N, 255);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TTI_SAPI_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TTI_SAPI_INS_set", N, value );

    /* ((0x00007c0c + (N) * 0x40) bits 0) field TTI_SAPI_INS of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    odu_tfrm_reg_TCM_CTRL3_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_SAPI_INS_MSK,
                                            ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_SAPI_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_TTI_SAPI_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TTI_SAPI_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 255)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_SAPI_INS_get", N, 255);
    /* ((0x00007c0c + (N) * 0x40) bits 0) field TTI_SAPI_INS of register PMC_ODU_TFRM_REG_TCM_CTRL3 index N=0..255 */
    reg_value = odu_tfrm_reg_TCM_CTRL3_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_SAPI_INS_MSK) >> ODU_TFRM_REG_TCM_CTRL3_BIT_TTI_SAPI_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TTI_SAPI_INS_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset ram_N_size96
 * ==================================================================================
 */
static INLINE void odu_tfrm_field_DM_STATE_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DM_STATE_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DM_STATE_PM_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DM_STATE_PM_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_DM_STATE_PM_set", N, value );

    /* ((0x0000bc00 + (N) * 0x40) bits 26:24) field DM_STATE_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
    odu_tfrm_reg_DM_PM_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_MSK,
                                        ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_OFF,
                                        value);
}

static INLINE UINT32 odu_tfrm_field_DM_STATE_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DM_STATE_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DM_STATE_PM_get", N, 95);
    /* ((0x0000bc00 + (N) * 0x40) bits 26:24) field DM_STATE_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
    reg_value = odu_tfrm_reg_DM_PM_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_MSK) >> ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_DM_STATE_PM_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_DM_STATE_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_DM_STATE_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_DM_STATE_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_DM_STATE_PM_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_DM_STATE_PM_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_DM_STATE_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc00 + (N) * 0x40) bits 26:24) field DM_STATE_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
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
        /* ((0x0000bc00 + (N) * 0x40) bits 26:24) field DM_STATE_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
        odu_tfrm_reg_DM_PM_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_OFF + subfield_offset),
                                            ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_DM_STATE_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_DM_STATE_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_DM_STATE_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_DM_STATE_PM_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_DM_STATE_PM_get", stop_bit, 2 );
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
    /* ((0x0000bc00 + (N) * 0x40) bits 26:24) field DM_STATE_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
    reg_value = odu_tfrm_reg_DM_PM_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_MSK)
                  >> ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_MSK, ODU_TFRM_REG_DM_PM_BIT_DM_STATE_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_DM_STATE_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_DM_COUNT_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DM_COUNT_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DM_COUNT_PM_set", N, 95);
    if (value > 1048575)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DM_COUNT_PM_set", value, 1048575);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_DM_COUNT_PM_set", N, value );

    /* ((0x0000bc00 + (N) * 0x40) bits 19:0) field DM_COUNT_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
    odu_tfrm_reg_DM_PM_array_field_set( b_ptr,
                                        h_ptr,
                                        N,
                                        ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_MSK,
                                        ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_OFF,
                                        value);
}

static INLINE UINT32 odu_tfrm_field_DM_COUNT_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DM_COUNT_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DM_COUNT_PM_get", N, 95);
    /* ((0x0000bc00 + (N) * 0x40) bits 19:0) field DM_COUNT_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
    reg_value = odu_tfrm_reg_DM_PM_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_MSK) >> ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_DM_COUNT_PM_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_DM_COUNT_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_DM_COUNT_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_DM_COUNT_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_DM_COUNT_PM_set", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_DM_COUNT_PM_set", stop_bit, 19 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_DM_COUNT_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc00 + (N) * 0x40) bits 19:0) field DM_COUNT_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
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
        /* ((0x0000bc00 + (N) * 0x40) bits 19:0) field DM_COUNT_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
        odu_tfrm_reg_DM_PM_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            subfield_mask << (ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_OFF + subfield_offset),
                                            ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_DM_COUNT_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_DM_COUNT_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_DM_COUNT_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_DM_COUNT_PM_get", stop_bit, start_bit );
    if (stop_bit > 19) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_DM_COUNT_PM_get", stop_bit, 19 );
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
    /* ((0x0000bc00 + (N) * 0x40) bits 19:0) field DM_COUNT_PM of register PMC_ODU_TFRM_REG_DM_PM index N=0..95 */
    reg_value = odu_tfrm_reg_DM_PM_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_MSK)
                  >> ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_MSK, ODU_TFRM_REG_DM_PM_BIT_DM_COUNT_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_DM_COUNT_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_STAT_PM_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_STAT_PM_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_STAT_PM_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_STAT_PM_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_STAT_PM_set", N, value );

    /* ((0x0000bc04 + (N) * 0x40) bits 2:0) field STAT_PM of register PMC_ODU_TFRM_REG_PM_CTRL1 index N=0..95 */
    odu_tfrm_reg_PM_CTRL1_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_MSK,
                                           ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_OFF,
                                           value);
}

static INLINE UINT32 odu_tfrm_field_STAT_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_STAT_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_STAT_PM_get", N, 95);
    /* ((0x0000bc04 + (N) * 0x40) bits 2:0) field STAT_PM of register PMC_ODU_TFRM_REG_PM_CTRL1 index N=0..95 */
    reg_value = odu_tfrm_reg_PM_CTRL1_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_MSK) >> ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_STAT_PM_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_STAT_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_STAT_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_STAT_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_STAT_PM_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_STAT_PM_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_STAT_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc04 + (N) * 0x40) bits 2:0) field STAT_PM of register PMC_ODU_TFRM_REG_PM_CTRL1 index N=0..95 */
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
        /* ((0x0000bc04 + (N) * 0x40) bits 2:0) field STAT_PM of register PMC_ODU_TFRM_REG_PM_CTRL1 index N=0..95 */
        odu_tfrm_reg_PM_CTRL1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_OFF + subfield_offset),
                                               ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_STAT_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_STAT_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_STAT_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_STAT_PM_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_STAT_PM_get", stop_bit, 2 );
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
    /* ((0x0000bc04 + (N) * 0x40) bits 2:0) field STAT_PM of register PMC_ODU_TFRM_REG_PM_CTRL1 index N=0..95 */
    reg_value = odu_tfrm_reg_PM_CTRL1_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_MSK)
                  >> ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_MSK, ODU_TFRM_REG_PM_CTRL1_BIT_STAT_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_STAT_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_PM_TCM_RES_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_PM_TCM_RES_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_PM_TCM_RES_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_PM_TCM_RES_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_PM_TCM_RES_set", N, value );

    /* ((0x0000bc08 + (N) * 0x40) bits 24) field PM_TCM_RES of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
    odu_tfrm_reg_ROW2_OH_array_field_set( b_ptr,
                                          h_ptr,
                                          N,
                                          ODU_TFRM_REG_ROW2_OH_BIT_PM_TCM_RES_MSK,
                                          ODU_TFRM_REG_ROW2_OH_BIT_PM_TCM_RES_OFF,
                                          value);
}

static INLINE UINT32 odu_tfrm_field_PM_TCM_RES_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_PM_TCM_RES_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_PM_TCM_RES_get", N, 95);
    /* ((0x0000bc08 + (N) * 0x40) bits 24) field PM_TCM_RES of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_ROW2_OH_BIT_PM_TCM_RES_MSK) >> ODU_TFRM_REG_ROW2_OH_BIT_PM_TCM_RES_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_PM_TCM_RES_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM_ACT_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM_ACT_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM_ACT_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM_ACT_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM_ACT_set", N, value );

    /* ((0x0000bc08 + (N) * 0x40) bits 23:16) field TCM_ACT of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
    odu_tfrm_reg_ROW2_OH_array_field_set( b_ptr,
                                          h_ptr,
                                          N,
                                          ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK,
                                          ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF,
                                          value);
}

static INLINE UINT32 odu_tfrm_field_TCM_ACT_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM_ACT_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM_ACT_get", N, 95);
    /* ((0x0000bc08 + (N) * 0x40) bits 23:16) field TCM_ACT of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK) >> ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM_ACT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_TCM_ACT_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_TCM_ACT_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM_ACT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM_ACT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM_ACT_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM_ACT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc08 + (N) * 0x40) bits 23:16) field TCM_ACT of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
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
        /* ((0x0000bc08 + (N) * 0x40) bits 23:16) field TCM_ACT of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
        odu_tfrm_reg_ROW2_OH_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              subfield_mask << (ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF + subfield_offset),
                                              ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_TCM_ACT_get( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_TCM_ACT_get( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM_ACT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM_ACT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM_ACT_get", stop_bit, 7 );
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
    /* ((0x0000bc08 + (N) * 0x40) bits 23:16) field TCM_ACT of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK)
                  >> ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_MSK, ODU_TFRM_REG_ROW2_OH_BIT_TCM_ACT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM_ACT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_RES1_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_RES1_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES1_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_RES1_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_RES1_set", N, value );

    /* ((0x0000bc08 + (N) * 0x40) bits 15:0) field RES1 of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
    odu_tfrm_reg_ROW2_OH_array_field_set( b_ptr,
                                          h_ptr,
                                          N,
                                          ODU_TFRM_REG_ROW2_OH_BIT_RES1_MSK,
                                          ODU_TFRM_REG_ROW2_OH_BIT_RES1_OFF,
                                          value);
}

static INLINE UINT32 odu_tfrm_field_RES1_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_RES1_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES1_get", N, 95);
    /* ((0x0000bc08 + (N) * 0x40) bits 15:0) field RES1 of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_ROW2_OH_BIT_RES1_MSK) >> ODU_TFRM_REG_ROW2_OH_BIT_RES1_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_RES1_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_RES1_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_RES1_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_RES1_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_RES1_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_RES1_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_RES1_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc08 + (N) * 0x40) bits 15:0) field RES1 of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
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
        /* ((0x0000bc08 + (N) * 0x40) bits 15:0) field RES1 of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
        odu_tfrm_reg_ROW2_OH_array_field_set( b_ptr,
                                              h_ptr,
                                              N,
                                              subfield_mask << (ODU_TFRM_REG_ROW2_OH_BIT_RES1_OFF + subfield_offset),
                                              ODU_TFRM_REG_ROW2_OH_BIT_RES1_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_RES1_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_RES1_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_RES1_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_RES1_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_RES1_get", stop_bit, 15 );
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
    /* ((0x0000bc08 + (N) * 0x40) bits 15:0) field RES1 of register PMC_ODU_TFRM_REG_ROW2_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW2_OH_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_ROW2_OH_BIT_RES1_MSK)
                  >> ODU_TFRM_REG_ROW2_OH_BIT_RES1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_ROW2_OH_BIT_RES1_MSK, ODU_TFRM_REG_ROW2_OH_BIT_RES1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_RES1_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_RES2_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_RES2_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES2_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_RES2_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_RES2_set", N, value );

    /* ((0x0000bc0c + (N) * 0x40) bits 31:16) field RES2 of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
    odu_tfrm_reg_ROW3_4_OH_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_MSK,
                                            ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_RES2_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_RES2_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES2_get", N, 95);
    /* ((0x0000bc0c + (N) * 0x40) bits 31:16) field RES2 of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW3_4_OH_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_MSK) >> ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_RES2_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_RES2_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_RES2_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_RES2_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_RES2_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_RES2_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_RES2_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc0c + (N) * 0x40) bits 31:16) field RES2 of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
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
        /* ((0x0000bc0c + (N) * 0x40) bits 31:16) field RES2 of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
        odu_tfrm_reg_ROW3_4_OH_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                subfield_mask << (ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_OFF + subfield_offset),
                                                ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_RES2_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_RES2_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_RES2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_RES2_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_RES2_get", stop_bit, 15 );
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
    /* ((0x0000bc0c + (N) * 0x40) bits 31:16) field RES2 of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW3_4_OH_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    field_value = (reg_value & ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_MSK)
                  >> ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_MSK, ODU_TFRM_REG_ROW3_4_OH_BIT_RES2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_RES2_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_EXP_set( odu_tfrm_buffer_t *b_ptr,
                                           odu_tfrm_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_EXP_set( odu_tfrm_buffer_t *b_ptr,
                                           odu_tfrm_handle_t *h_ptr,
                                           UINT32  N,
                                           UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_EXP_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_EXP_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_EXP_set", N, value );

    /* ((0x0000bc0c + (N) * 0x40) bits 15:0) field EXP of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
    odu_tfrm_reg_ROW3_4_OH_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_MSK,
                                            ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_EXP_get( odu_tfrm_buffer_t *b_ptr,
                                             odu_tfrm_handle_t *h_ptr,
                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_EXP_get( odu_tfrm_buffer_t *b_ptr,
                                             odu_tfrm_handle_t *h_ptr,
                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_EXP_get", N, 95);
    /* ((0x0000bc0c + (N) * 0x40) bits 15:0) field EXP of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW3_4_OH_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_MSK) >> ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_EXP_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_EXP_set( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_EXP_set( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_EXP_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_EXP_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_EXP_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_EXP_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc0c + (N) * 0x40) bits 15:0) field EXP of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
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
        /* ((0x0000bc0c + (N) * 0x40) bits 15:0) field EXP of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
        odu_tfrm_reg_ROW3_4_OH_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                subfield_mask << (ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_OFF + subfield_offset),
                                                ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_EXP_get( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_EXP_get( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_EXP_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_EXP_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_EXP_get", stop_bit, 15 );
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
    /* ((0x0000bc0c + (N) * 0x40) bits 15:0) field EXP of register PMC_ODU_TFRM_REG_ROW3_4_OH index N=0..95 */
    reg_value = odu_tfrm_reg_ROW3_4_OH_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    field_value = (reg_value & ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_MSK)
                  >> ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_MSK, ODU_TFRM_REG_ROW3_4_OH_BIT_EXP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_EXP_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_RES3_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_RES3_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES3_set", N, 95);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_RES3_set", N, value );

    /* ((0x0000bc10 + (N) * 0x40) bits 31:0) field RES3 of register PMC_ODU_TFRM_REG_ROW4_OH0 index N=0..95 */
    odu_tfrm_reg_ROW4_OH0_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODU_TFRM_REG_ROW4_OH0_BIT_RES3_MSK,
                                           ODU_TFRM_REG_ROW4_OH0_BIT_RES3_OFF,
                                           value);
}

static INLINE UINT32 odu_tfrm_field_RES3_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_RES3_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES3_get", N, 95);
    /* ((0x0000bc10 + (N) * 0x40) bits 31:0) field RES3 of register PMC_ODU_TFRM_REG_ROW4_OH0 index N=0..95 */
    reg_value = odu_tfrm_reg_ROW4_OH0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_ROW4_OH0_BIT_RES3_MSK) >> ODU_TFRM_REG_ROW4_OH0_BIT_RES3_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_RES3_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_RES3_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_RES3_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_RES3_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_RES3_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_RES3_set", stop_bit, 31 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_RES3_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc10 + (N) * 0x40) bits 31:0) field RES3 of register PMC_ODU_TFRM_REG_ROW4_OH0 index N=0..95 */
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
        /* ((0x0000bc10 + (N) * 0x40) bits 31:0) field RES3 of register PMC_ODU_TFRM_REG_ROW4_OH0 index N=0..95 */
        odu_tfrm_reg_ROW4_OH0_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODU_TFRM_REG_ROW4_OH0_BIT_RES3_OFF + subfield_offset),
                                               ODU_TFRM_REG_ROW4_OH0_BIT_RES3_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_RES3_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_RES3_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_RES3_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_RES3_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_RES3_get", stop_bit, 31 );
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
    /* ((0x0000bc10 + (N) * 0x40) bits 31:0) field RES3 of register PMC_ODU_TFRM_REG_ROW4_OH0 index N=0..95 */
    reg_value = odu_tfrm_reg_ROW4_OH0_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_ROW4_OH0_BIT_RES3_MSK)
                  >> ODU_TFRM_REG_ROW4_OH0_BIT_RES3_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_ROW4_OH0_BIT_RES3_MSK, ODU_TFRM_REG_ROW4_OH0_BIT_RES3_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_RES3_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_GCC2_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_GCC2_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_GCC2_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_GCC2_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_GCC2_set", N, value );

    /* ((0x0000bc14 + (N) * 0x40) bits 31:16) field GCC2 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
    odu_tfrm_reg_ROW4_OH1_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_MSK,
                                           ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_OFF,
                                           value);
}

static INLINE UINT32 odu_tfrm_field_GCC2_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_GCC2_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_GCC2_get", N, 95);
    /* ((0x0000bc14 + (N) * 0x40) bits 31:16) field GCC2 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
    reg_value = odu_tfrm_reg_ROW4_OH1_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_MSK) >> ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_GCC2_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_GCC2_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_GCC2_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_GCC2_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_GCC2_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_GCC2_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_GCC2_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc14 + (N) * 0x40) bits 31:16) field GCC2 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
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
        /* ((0x0000bc14 + (N) * 0x40) bits 31:16) field GCC2 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
        odu_tfrm_reg_ROW4_OH1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_OFF + subfield_offset),
                                               ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_GCC2_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_GCC2_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_GCC2_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_GCC2_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_GCC2_get", stop_bit, 15 );
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
    /* ((0x0000bc14 + (N) * 0x40) bits 31:16) field GCC2 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
    reg_value = odu_tfrm_reg_ROW4_OH1_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_MSK)
                  >> ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_MSK, ODU_TFRM_REG_ROW4_OH1_BIT_GCC2_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_GCC2_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_GCC1_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_GCC1_set( odu_tfrm_buffer_t *b_ptr,
                                            odu_tfrm_handle_t *h_ptr,
                                            UINT32  N,
                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_GCC1_set", N, 95);
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_GCC1_set", value, 65535);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_GCC1_set", N, value );

    /* ((0x0000bc14 + (N) * 0x40) bits 15:0) field GCC1 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
    odu_tfrm_reg_ROW4_OH1_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_MSK,
                                           ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_OFF,
                                           value);
}

static INLINE UINT32 odu_tfrm_field_GCC1_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_GCC1_get( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_GCC1_get", N, 95);
    /* ((0x0000bc14 + (N) * 0x40) bits 15:0) field GCC1 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
    reg_value = odu_tfrm_reg_ROW4_OH1_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_MSK) >> ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_GCC1_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_GCC1_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_GCC1_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_GCC1_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_GCC1_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_GCC1_set", stop_bit, 15 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_GCC1_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc14 + (N) * 0x40) bits 15:0) field GCC1 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
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
        /* ((0x0000bc14 + (N) * 0x40) bits 15:0) field GCC1 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
        odu_tfrm_reg_ROW4_OH1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_OFF + subfield_offset),
                                               ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_GCC1_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_GCC1_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_GCC1_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_GCC1_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_GCC1_get", stop_bit, 15 );
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
    /* ((0x0000bc14 + (N) * 0x40) bits 15:0) field GCC1 of register PMC_ODU_TFRM_REG_ROW4_OH1 index N=0..95 */
    reg_value = odu_tfrm_reg_ROW4_OH1_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_MSK)
                  >> ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_MSK, ODU_TFRM_REG_ROW4_OH1_BIT_GCC1_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_GCC1_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM1_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM1_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM1_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM1_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM1_POOL_ID_set", N, value );

    /* ((0x0000bc18 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_TFRM_REG_TCM1_POOL_ID index N=0..95 */
    odu_tfrm_reg_TCM1_POOL_ID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_MSK,
                                               ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM1_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM1_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM1_POOL_ID_get", N, 95);
    /* ((0x0000bc18 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_TFRM_REG_TCM1_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM1_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_MSK) >> ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM1_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_TCM1_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_TCM1_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM1_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM1_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM1_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM1_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc18 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_TFRM_REG_TCM1_POOL_ID index N=0..95 */
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
        /* ((0x0000bc18 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_TFRM_REG_TCM1_POOL_ID index N=0..95 */
        odu_tfrm_reg_TCM1_POOL_ID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_OFF + subfield_offset),
                                                   ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_TCM1_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_TCM1_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM1_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM1_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM1_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x0000bc18 + (N) * 0x40) bits 7:0) field TCM1_POOL_ID of register PMC_ODU_TFRM_REG_TCM1_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM1_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_MSK)
                  >> ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_MSK, ODU_TFRM_REG_TCM1_POOL_ID_BIT_TCM1_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM1_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM2_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM2_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM2_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM2_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM2_POOL_ID_set", N, value );

    /* ((0x0000bc1c + (N) * 0x40) bits 7:0) field TCM2_POOL_ID of register PMC_ODU_TFRM_REG_TCM2_POOL_ID index N=0..95 */
    odu_tfrm_reg_TCM2_POOL_ID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_MSK,
                                               ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM2_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM2_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM2_POOL_ID_get", N, 95);
    /* ((0x0000bc1c + (N) * 0x40) bits 7:0) field TCM2_POOL_ID of register PMC_ODU_TFRM_REG_TCM2_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM2_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_MSK) >> ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM2_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_TCM2_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_TCM2_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM2_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM2_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM2_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM2_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc1c + (N) * 0x40) bits 7:0) field TCM2_POOL_ID of register PMC_ODU_TFRM_REG_TCM2_POOL_ID index N=0..95 */
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
        /* ((0x0000bc1c + (N) * 0x40) bits 7:0) field TCM2_POOL_ID of register PMC_ODU_TFRM_REG_TCM2_POOL_ID index N=0..95 */
        odu_tfrm_reg_TCM2_POOL_ID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_OFF + subfield_offset),
                                                   ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_TCM2_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_TCM2_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM2_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM2_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM2_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x0000bc1c + (N) * 0x40) bits 7:0) field TCM2_POOL_ID of register PMC_ODU_TFRM_REG_TCM2_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM2_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_MSK)
                  >> ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_MSK, ODU_TFRM_REG_TCM2_POOL_ID_BIT_TCM2_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM2_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM3_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM3_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM3_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM3_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM3_POOL_ID_set", N, value );

    /* ((0x0000bc20 + (N) * 0x40) bits 7:0) field TCM3_POOL_ID of register PMC_ODU_TFRM_REG_TCM3_POOL_ID index N=0..95 */
    odu_tfrm_reg_TCM3_POOL_ID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_MSK,
                                               ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM3_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM3_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM3_POOL_ID_get", N, 95);
    /* ((0x0000bc20 + (N) * 0x40) bits 7:0) field TCM3_POOL_ID of register PMC_ODU_TFRM_REG_TCM3_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM3_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_MSK) >> ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM3_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_TCM3_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_TCM3_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM3_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM3_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM3_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM3_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc20 + (N) * 0x40) bits 7:0) field TCM3_POOL_ID of register PMC_ODU_TFRM_REG_TCM3_POOL_ID index N=0..95 */
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
        /* ((0x0000bc20 + (N) * 0x40) bits 7:0) field TCM3_POOL_ID of register PMC_ODU_TFRM_REG_TCM3_POOL_ID index N=0..95 */
        odu_tfrm_reg_TCM3_POOL_ID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_OFF + subfield_offset),
                                                   ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_TCM3_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_TCM3_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM3_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM3_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM3_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x0000bc20 + (N) * 0x40) bits 7:0) field TCM3_POOL_ID of register PMC_ODU_TFRM_REG_TCM3_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM3_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_MSK)
                  >> ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_MSK, ODU_TFRM_REG_TCM3_POOL_ID_BIT_TCM3_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM3_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM4_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM4_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM4_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM4_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM4_POOL_ID_set", N, value );

    /* ((0x0000bc24 + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_TFRM_REG_TCM4_POOL_ID index N=0..95 */
    odu_tfrm_reg_TCM4_POOL_ID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_MSK,
                                               ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM4_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM4_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM4_POOL_ID_get", N, 95);
    /* ((0x0000bc24 + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_TFRM_REG_TCM4_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM4_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_MSK) >> ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM4_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_TCM4_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_TCM4_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM4_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM4_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM4_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM4_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc24 + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_TFRM_REG_TCM4_POOL_ID index N=0..95 */
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
        /* ((0x0000bc24 + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_TFRM_REG_TCM4_POOL_ID index N=0..95 */
        odu_tfrm_reg_TCM4_POOL_ID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_OFF + subfield_offset),
                                                   ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_TCM4_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_TCM4_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM4_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM4_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM4_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x0000bc24 + (N) * 0x40) bits 7:0) field TCM4_POOL_ID of register PMC_ODU_TFRM_REG_TCM4_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM4_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_MSK)
                  >> ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_MSK, ODU_TFRM_REG_TCM4_POOL_ID_BIT_TCM4_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM4_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM5_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM5_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM5_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM5_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM5_POOL_ID_set", N, value );

    /* ((0x0000bc28 + (N) * 0x40) bits 7:0) field TCM5_POOL_ID of register PMC_ODU_TFRM_REG_TCM5_POOL_ID index N=0..95 */
    odu_tfrm_reg_TCM5_POOL_ID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_MSK,
                                               ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM5_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM5_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM5_POOL_ID_get", N, 95);
    /* ((0x0000bc28 + (N) * 0x40) bits 7:0) field TCM5_POOL_ID of register PMC_ODU_TFRM_REG_TCM5_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM5_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_MSK) >> ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM5_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_TCM5_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_TCM5_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM5_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM5_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM5_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM5_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc28 + (N) * 0x40) bits 7:0) field TCM5_POOL_ID of register PMC_ODU_TFRM_REG_TCM5_POOL_ID index N=0..95 */
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
        /* ((0x0000bc28 + (N) * 0x40) bits 7:0) field TCM5_POOL_ID of register PMC_ODU_TFRM_REG_TCM5_POOL_ID index N=0..95 */
        odu_tfrm_reg_TCM5_POOL_ID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_OFF + subfield_offset),
                                                   ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_TCM5_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_TCM5_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM5_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM5_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM5_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x0000bc28 + (N) * 0x40) bits 7:0) field TCM5_POOL_ID of register PMC_ODU_TFRM_REG_TCM5_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM5_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_MSK)
                  >> ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_MSK, ODU_TFRM_REG_TCM5_POOL_ID_BIT_TCM5_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM5_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM6_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM6_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM6_POOL_ID_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM6_POOL_ID_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM6_POOL_ID_set", N, value );

    /* ((0x0000bc2c + (N) * 0x40) bits 7:0) field TCM6_POOL_ID of register PMC_ODU_TFRM_REG_TCM6_POOL_ID index N=0..95 */
    odu_tfrm_reg_TCM6_POOL_ID_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_MSK,
                                               ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM6_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM6_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM6_POOL_ID_get", N, 95);
    /* ((0x0000bc2c + (N) * 0x40) bits 7:0) field TCM6_POOL_ID of register PMC_ODU_TFRM_REG_TCM6_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM6_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_MSK) >> ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM6_POOL_ID_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_TCM6_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_TCM6_POOL_ID_set( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM6_POOL_ID_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM6_POOL_ID_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM6_POOL_ID_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM6_POOL_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc2c + (N) * 0x40) bits 7:0) field TCM6_POOL_ID of register PMC_ODU_TFRM_REG_TCM6_POOL_ID index N=0..95 */
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
        /* ((0x0000bc2c + (N) * 0x40) bits 7:0) field TCM6_POOL_ID of register PMC_ODU_TFRM_REG_TCM6_POOL_ID index N=0..95 */
        odu_tfrm_reg_TCM6_POOL_ID_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_OFF + subfield_offset),
                                                   ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_TCM6_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_TCM6_POOL_ID_get( odu_tfrm_buffer_t *b_ptr,
                                                            odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_TCM6_POOL_ID_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_TCM6_POOL_ID_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_TCM6_POOL_ID_get", stop_bit, 7 );
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
    /* ((0x0000bc2c + (N) * 0x40) bits 7:0) field TCM6_POOL_ID of register PMC_ODU_TFRM_REG_TCM6_POOL_ID index N=0..95 */
    reg_value = odu_tfrm_reg_TCM6_POOL_ID_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_MSK)
                  >> ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_MSK, ODU_TFRM_REG_TCM6_POOL_ID_BIT_TCM6_POOL_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_TCM6_POOL_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_RCP_EN_set( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_RCP_EN_set( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RCP_EN_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_RCP_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_RCP_EN_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 18) field RCP_EN of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_RCP_EN_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_RCP_EN_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_RCP_EN_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_RCP_EN_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RCP_EN_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 18) field RCP_EN of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_RCP_EN_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_RCP_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_RCP_EN_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_GCC1_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_GCC1_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_GCC1_INS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_GCC1_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_GCC1_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 17) field GCC1_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_GCC1_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_GCC1_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_GCC1_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_GCC1_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_GCC1_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 17) field GCC1_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_GCC1_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_GCC1_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_GCC1_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_GCC2_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_GCC2_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_GCC2_INS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_GCC2_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_GCC2_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 16) field GCC2_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_GCC2_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_GCC2_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_GCC2_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_GCC2_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_GCC2_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 16) field GCC2_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_GCC2_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_GCC2_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_GCC2_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_APS_PCC_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_APS_PCC_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_APS_PCC_INS_set", N, 95);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_APS_PCC_INS_set", value, 255);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_APS_PCC_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 15:8) field APS_PCC_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_APS_PCC_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_APS_PCC_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_APS_PCC_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 15:8) field APS_PCC_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_APS_PCC_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_APS_PCC_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_APS_PCC_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_APS_PCC_INS_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_APS_PCC_INS_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_APS_PCC_INS_set", stop_bit, 7 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_APS_PCC_INS_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc30 + (N) * 0x40) bits 15:8) field APS_PCC_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
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
        /* ((0x0000bc30 + (N) * 0x40) bits 15:8) field APS_PCC_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
        odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                subfield_mask << (ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_OFF + subfield_offset),
                                                ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_APS_PCC_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_APS_PCC_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                           odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_APS_PCC_INS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_APS_PCC_INS_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_APS_PCC_INS_get", stop_bit, 7 );
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
    /* ((0x0000bc30 + (N) * 0x40) bits 15:8) field APS_PCC_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    field_value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_MSK)
                  >> ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_MSK, ODU_TFRM_REG_OH_INSERT_BIT_APS_PCC_INS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_APS_PCC_INS_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_RES2_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_RES2_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES2_INS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_RES2_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_RES2_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 6) field RES2_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_RES2_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_RES2_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_RES2_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_RES2_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES2_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 6) field RES2_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_RES2_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_RES2_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_RES2_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_EXP_INS_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_EXP_INS_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_EXP_INS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_EXP_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_EXP_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 5) field EXP_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_EXP_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_EXP_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_EXP_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_EXP_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_EXP_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 5) field EXP_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_EXP_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_EXP_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_EXP_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_RES1_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_RES1_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES1_INS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_RES1_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_RES1_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 4) field RES1_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_RES1_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_RES1_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_RES1_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_RES1_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_RES1_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 4) field RES1_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_RES1_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_RES1_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_RES1_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_PM_TCM_RES_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_PM_TCM_RES_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_PM_TCM_RES_INS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_PM_TCM_RES_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_PM_TCM_RES_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 3) field PM_TCM_RES_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_PM_TCM_RES_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_PM_TCM_RES_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_PM_TCM_RES_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_PM_TCM_RES_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_PM_TCM_RES_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 3) field PM_TCM_RES_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_PM_TCM_RES_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_PM_TCM_RES_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_PM_TCM_RES_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM_ACT_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM_ACT_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM_ACT_INS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM_ACT_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM_ACT_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 2) field TCM_ACT_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_TCM_ACT_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_TCM_ACT_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_TCM_ACT_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM_ACT_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM_ACT_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 2) field TCM_ACT_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_TCM_ACT_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_TCM_ACT_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM_ACT_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_FTFL_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_FTFL_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_FTFL_INS_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_FTFL_INS_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_FTFL_INS_set", N, value );

    /* ((0x0000bc30 + (N) * 0x40) bits 1:0) field FTFL_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                            h_ptr,
                                            N,
                                            ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_MSK,
                                            ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_OFF,
                                            value);
}

static INLINE UINT32 odu_tfrm_field_FTFL_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_FTFL_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_FTFL_INS_get", N, 95);
    /* ((0x0000bc30 + (N) * 0x40) bits 1:0) field FTFL_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_MSK) >> ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_FTFL_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_FTFL_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_FTFL_INS_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_FTFL_INS_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_FTFL_INS_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_FTFL_INS_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_FTFL_INS_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc30 + (N) * 0x40) bits 1:0) field FTFL_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
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
        /* ((0x0000bc30 + (N) * 0x40) bits 1:0) field FTFL_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
        odu_tfrm_reg_OH_INSERT_array_field_set( b_ptr,
                                                h_ptr,
                                                N,
                                                subfield_mask << (ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_OFF + subfield_offset),
                                                ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_FTFL_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_FTFL_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_FTFL_INS_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_FTFL_INS_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_FTFL_INS_get", stop_bit, 1 );
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
    /* ((0x0000bc30 + (N) * 0x40) bits 1:0) field FTFL_INS of register PMC_ODU_TFRM_REG_OH_INSERT index N=0..95 */
    reg_value = odu_tfrm_reg_OH_INSERT_array_read( b_ptr,
                                                   h_ptr,
                                                   N);
    field_value = (reg_value & ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_MSK)
                  >> ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_MSK, ODU_TFRM_REG_OH_INSERT_BIT_FTFL_INS_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_FTFL_INS_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_IAE_INS_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_IAE_INS_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_IAE_INS_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_IAE_INS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_IAE_INS_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 28) field IAE_INS of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_IAE_INS_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_IAE_INS_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_IAE_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_IAE_INS_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_IAE_INS_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 28) field IAE_INS of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_IAE_INS_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_IAE_INS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_IAE_INS_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TTI_OS_INS_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TTI_OS_INS_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                     odu_tfrm_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_OS_INS_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TTI_OS_INS_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TTI_OS_INS_PM_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 26) field TTI_OS_INS_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_OS_INS_PM_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_OS_INS_PM_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TTI_OS_INS_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TTI_OS_INS_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_OS_INS_PM_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 26) field TTI_OS_INS_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_OS_INS_PM_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_OS_INS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TTI_OS_INS_PM_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TTI_DAPI_INS_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TTI_DAPI_INS_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_DAPI_INS_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TTI_DAPI_INS_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TTI_DAPI_INS_PM_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 25) field TTI_DAPI_INS_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_DAPI_INS_PM_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_DAPI_INS_PM_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TTI_DAPI_INS_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TTI_DAPI_INS_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_DAPI_INS_PM_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 25) field TTI_DAPI_INS_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_DAPI_INS_PM_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_DAPI_INS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TTI_DAPI_INS_PM_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TTI_SAPI_INS_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TTI_SAPI_INS_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_SAPI_INS_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TTI_SAPI_INS_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TTI_SAPI_INS_PM_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 24) field TTI_SAPI_INS_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_SAPI_INS_PM_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_SAPI_INS_PM_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TTI_SAPI_INS_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TTI_SAPI_INS_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TTI_SAPI_INS_PM_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 24) field TTI_SAPI_INS_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_SAPI_INS_PM_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TTI_SAPI_INS_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TTI_SAPI_INS_PM_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_BEI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_BEI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_BEI_PM_set", N, 95);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_BEI_PM_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_BEI_PM_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 23:20) field BEI_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_BEI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_BEI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_BEI_PM_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 23:20) field BEI_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_BEI_PM_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_BEI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_BEI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_BEI_PM_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_BEI_PM_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_BEI_PM_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_BEI_PM_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc34 + (N) * 0x40) bits 23:20) field BEI_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
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
        /* ((0x0000bc34 + (N) * 0x40) bits 23:20) field BEI_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
        odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_OFF + subfield_offset),
                                                   ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_BEI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_BEI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_BEI_PM_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_BEI_PM_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_BEI_PM_get", stop_bit, 3 );
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
    /* ((0x0000bc34 + (N) * 0x40) bits 23:20) field BEI_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_MSK)
                  >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_MSK, ODU_TFRM_REG_CHANNEL_CTRL_BIT_BEI_PM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_BEI_PM_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_BDI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_BDI_PM_set( odu_tfrm_buffer_t *b_ptr,
                                              odu_tfrm_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_BDI_PM_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_BDI_PM_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_BDI_PM_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 19) field BDI_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_BDI_PM_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_BDI_PM_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_BDI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_BDI_PM_get( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_BDI_PM_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 19) field BDI_PM of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_BDI_PM_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_BDI_PM_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_BDI_PM_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_SF_SD_PROP_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_SF_SD_PROP_set( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_SF_SD_PROP_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_SF_SD_PROP_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_SF_SD_PROP_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 17) field SF_SD_PROP of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_SF_SD_PROP_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_SF_SD_PROP_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_SF_SD_PROP_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_SF_SD_PROP_get( odu_tfrm_buffer_t *b_ptr,
                                                    odu_tfrm_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_SF_SD_PROP_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 17) field SF_SD_PROP of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_SF_SD_PROP_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_SF_SD_PROP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_SF_SD_PROP_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_SSF_PROP_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_SSF_PROP_set( odu_tfrm_buffer_t *b_ptr,
                                                odu_tfrm_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_SSF_PROP_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_SSF_PROP_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_SSF_PROP_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 16) field SSF_PROP of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_SSF_PROP_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_SSF_PROP_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_SSF_PROP_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_SSF_PROP_get( odu_tfrm_buffer_t *b_ptr,
                                                  odu_tfrm_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_SSF_PROP_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 16) field SSF_PROP of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_SSF_PROP_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_SSF_PROP_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_SSF_PROP_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM6_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM6_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM6_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM6_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM6_TRANSPARENT_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 13) field TCM6_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM6_TRANSPARENT_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM6_TRANSPARENT_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM6_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM6_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM6_TRANSPARENT_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 13) field TCM6_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM6_TRANSPARENT_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM6_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM6_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM5_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM5_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM5_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM5_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM5_TRANSPARENT_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 12) field TCM5_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM5_TRANSPARENT_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM5_TRANSPARENT_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM5_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM5_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM5_TRANSPARENT_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 12) field TCM5_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM5_TRANSPARENT_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM5_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM5_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM4_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM4_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM4_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM4_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM4_TRANSPARENT_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 11) field TCM4_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM4_TRANSPARENT_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM4_TRANSPARENT_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM4_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM4_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM4_TRANSPARENT_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 11) field TCM4_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM4_TRANSPARENT_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM4_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM4_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM3_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM3_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM3_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM3_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM3_TRANSPARENT_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 10) field TCM3_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM3_TRANSPARENT_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM3_TRANSPARENT_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM3_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM3_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM3_TRANSPARENT_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 10) field TCM3_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM3_TRANSPARENT_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM3_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM3_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM2_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM2_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM2_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM2_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM2_TRANSPARENT_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 9) field TCM2_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM2_TRANSPARENT_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM2_TRANSPARENT_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM2_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM2_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM2_TRANSPARENT_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 9) field TCM2_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM2_TRANSPARENT_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM2_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM2_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_TCM1_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_TCM1_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM1_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_TCM1_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_TCM1_TRANSPARENT_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 8) field TCM1_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM1_TRANSPARENT_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM1_TRANSPARENT_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_TCM1_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_TCM1_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                          odu_tfrm_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_TCM1_TRANSPARENT_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 8) field TCM1_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM1_TRANSPARENT_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_TCM1_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_TCM1_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_PM_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_PM_TRANSPARENT_set( odu_tfrm_buffer_t *b_ptr,
                                                      odu_tfrm_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_PM_TRANSPARENT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_PM_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_PM_TRANSPARENT_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 7) field PM_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_PM_TRANSPARENT_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_PM_TRANSPARENT_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_PM_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_PM_TRANSPARENT_get( odu_tfrm_buffer_t *b_ptr,
                                                        odu_tfrm_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_PM_TRANSPARENT_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 7) field PM_TRANSPARENT of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_PM_TRANSPARENT_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_PM_TRANSPARENT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_PM_TRANSPARENT_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_MAINT_SIG_set( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_MAINT_SIG_set( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_MAINT_SIG_set", N, 95);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_MAINT_SIG_set", value, 3);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_MAINT_SIG_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 5:4) field MAINT_SIG of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_MAINT_SIG_get( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_MAINT_SIG_get( odu_tfrm_buffer_t *b_ptr,
                                                   odu_tfrm_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_MAINT_SIG_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 5:4) field MAINT_SIG of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_MAINT_SIG_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_range_MAINT_SIG_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_range_MAINT_SIG_set( odu_tfrm_buffer_t *b_ptr,
                                                       odu_tfrm_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_MAINT_SIG_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_MAINT_SIG_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_MAINT_SIG_set", stop_bit, 1 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_MAINT_SIG_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000bc34 + (N) * 0x40) bits 5:4) field MAINT_SIG of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
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
        /* ((0x0000bc34 + (N) * 0x40) bits 5:4) field MAINT_SIG of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
        odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                                   h_ptr,
                                                   N,
                                                   subfield_mask << (ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_OFF + subfield_offset),
                                                   ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 odu_tfrm_field_range_MAINT_SIG_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_range_MAINT_SIG_get( odu_tfrm_buffer_t *b_ptr,
                                                         odu_tfrm_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_range_MAINT_SIG_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odu_tfrm_field_range_MAINT_SIG_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odu_tfrm_field_range_MAINT_SIG_get", stop_bit, 1 );
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
    /* ((0x0000bc34 + (N) * 0x40) bits 5:4) field MAINT_SIG of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    field_value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_MSK)
                  >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_MSK, ODU_TFRM_REG_CHANNEL_CTRL_BIT_MAINT_SIG_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odu_tfrm_field_range_MAINT_SIG_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odu_tfrm_field_DMP_SRC_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DMP_SRC_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DMP_SRC_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DMP_SRC_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_DMP_SRC_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 1) field DMP_SRC of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_SRC_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_SRC_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_DMP_SRC_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DMP_SRC_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DMP_SRC_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 1) field DMP_SRC of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_SRC_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_SRC_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_DMP_SRC_get", N, value );

    return value;
}
static INLINE void odu_tfrm_field_DMP_VAL_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odu_tfrm_field_DMP_VAL_set( odu_tfrm_buffer_t *b_ptr,
                                               odu_tfrm_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DMP_VAL_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odu_tfrm_field_DMP_VAL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odu_tfrm_field_DMP_VAL_set", N, value );

    /* ((0x0000bc34 + (N) * 0x40) bits 0) field DMP_VAL of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    odu_tfrm_reg_CHANNEL_CTRL_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_VAL_MSK,
                                               ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_VAL_OFF,
                                               value);
}

static INLINE UINT32 odu_tfrm_field_DMP_VAL_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odu_tfrm_field_DMP_VAL_get( odu_tfrm_buffer_t *b_ptr,
                                                 odu_tfrm_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odu_tfrm_field_DMP_VAL_get", N, 95);
    /* ((0x0000bc34 + (N) * 0x40) bits 0) field DMP_VAL of register PMC_ODU_TFRM_REG_CHANNEL_CTRL index N=0..95 */
    reg_value = odu_tfrm_reg_CHANNEL_CTRL_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_VAL_MSK) >> ODU_TFRM_REG_CHANNEL_CTRL_BIT_DMP_VAL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odu_tfrm_field_DMP_VAL_get", N, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODU_TFRM_IO_INLINE_H */
