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
 *     and register accessor functions for the odujat block
 *****************************************************************************/
#ifndef _ODUJAT_IO_INLINE_H
#define _ODUJAT_IO_INLINE_H

#include "odujat_loc.h"
#include "odujat_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ODUJAT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for odujat
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
    odujat_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} odujat_buffer_t;
static INLINE void odujat_buffer_init( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void odujat_buffer_init( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "odujat_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void odujat_buffer_flush( odujat_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void odujat_buffer_flush( odujat_buffer_t *b_ptr )
{
    IOLOG( "odujat_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 odujat_reg_read( odujat_buffer_t *b_ptr,
                                      odujat_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 odujat_reg_read( odujat_buffer_t *b_ptr,
                                      odujat_handle_t *h_ptr,
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
static INLINE void odujat_reg_write( odujat_buffer_t *b_ptr,
                                     odujat_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_write( odujat_buffer_t *b_ptr,
                                     odujat_handle_t *h_ptr,
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

static INLINE void odujat_field_set( odujat_buffer_t *b_ptr,
                                     odujat_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_set( odujat_buffer_t *b_ptr,
                                     odujat_handle_t *h_ptr,
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

static INLINE void odujat_action_on_write_field_set( odujat_buffer_t *b_ptr,
                                                     odujat_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_action_on_write_field_set( odujat_buffer_t *b_ptr,
                                                     odujat_handle_t *h_ptr,
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

static INLINE void odujat_burst_read( odujat_buffer_t *b_ptr,
                                      odujat_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void odujat_burst_read( odujat_buffer_t *b_ptr,
                                      odujat_handle_t *h_ptr,
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

static INLINE void odujat_burst_write( odujat_buffer_t *b_ptr,
                                       odujat_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void odujat_burst_write( odujat_buffer_t *b_ptr,
                                       odujat_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE odujat_poll( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odujat_poll( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
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
 *  register access functions for odujat
 * ==================================================================================
 */

static INLINE void odujat_reg_CFG_write( odujat_buffer_t *b_ptr,
                                         odujat_handle_t *h_ptr,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_CFG_write( odujat_buffer_t *b_ptr,
                                         odujat_handle_t *h_ptr,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odujat_reg_CFG_write", value );
    odujat_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ODUJAT_CORE_REG_CFG,
                      value);
}

static INLINE void odujat_reg_CFG_field_set( odujat_buffer_t *b_ptr,
                                             odujat_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_CFG_field_set( odujat_buffer_t *b_ptr,
                                             odujat_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odujat_reg_CFG_field_set", mask, ofs, value );
    odujat_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ODUJAT_CORE_REG_CFG,
                      mask,
                      PMC_ODUJAT_CORE_REG_CFG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 odujat_reg_CFG_read( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_reg_CFG_read( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odujat_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ODUJAT_CORE_REG_CFG);

    IOLOG( "%s -> 0x%08x;", "odujat_reg_CFG_read", reg_value);
    return reg_value;
}

static INLINE void odujat_reg_CHAN_CFG_0_array_write( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_CHAN_CFG_0_array_write( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odujat_reg_CHAN_CFG_0_array_write", value );
    odujat_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_0(N),
                      value);
}

static INLINE void odujat_reg_CHAN_CFG_0_array_field_set( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_CHAN_CFG_0_array_field_set( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odujat_reg_CHAN_CFG_0_array_field_set", N, mask, ofs, value );
    odujat_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_0(N),
                      mask,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_0_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 odujat_reg_CHAN_CFG_0_array_read( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_reg_CHAN_CFG_0_array_read( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = odujat_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ODUJAT_CORE_REG_CHAN_CFG_0(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odujat_reg_CHAN_CFG_0_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odujat_reg_CHAN_CFG_1_array_write( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_CHAN_CFG_1_array_write( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odujat_reg_CHAN_CFG_1_array_write", value );
    odujat_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_1(N),
                      value);
}

static INLINE void odujat_reg_CHAN_CFG_1_array_field_set( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_CHAN_CFG_1_array_field_set( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odujat_reg_CHAN_CFG_1_array_field_set", N, mask, ofs, value );
    odujat_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_1(N),
                      mask,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 odujat_reg_CHAN_CFG_1_array_read( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_reg_CHAN_CFG_1_array_read( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = odujat_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ODUJAT_CORE_REG_CHAN_CFG_1(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odujat_reg_CHAN_CFG_1_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odujat_reg_CHAN_CFG_2_array_write( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_CHAN_CFG_2_array_write( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odujat_reg_CHAN_CFG_2_array_write", value );
    odujat_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_2(N),
                      value);
}

static INLINE void odujat_reg_CHAN_CFG_2_array_field_set( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_CHAN_CFG_2_array_field_set( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "odujat_reg_CHAN_CFG_2_array_field_set", N, mask, ofs, value );
    odujat_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_2(N),
                      mask,
                      PMC_ODUJAT_CORE_REG_CHAN_CFG_2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 odujat_reg_CHAN_CFG_2_array_read( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_reg_CHAN_CFG_2_array_read( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 reg_value;

    reg_value = odujat_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ODUJAT_CORE_REG_CHAN_CFG_2(N));

    IOLOG( "%s -> 0x%08x; N=%d", "odujat_reg_CHAN_CFG_2_array_read", reg_value, N);
    return reg_value;
}

static INLINE void odujat_reg_ERR_INT_EN_write( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_ERR_INT_EN_write( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odujat_reg_ERR_INT_EN_write", value );
    odujat_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ODUJAT_CORE_REG_ERR_INT_EN,
                      value);
}

static INLINE void odujat_reg_ERR_INT_EN_field_set( odujat_buffer_t *b_ptr,
                                                    odujat_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_ERR_INT_EN_field_set( odujat_buffer_t *b_ptr,
                                                    odujat_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odujat_reg_ERR_INT_EN_field_set", mask, ofs, value );
    odujat_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ODUJAT_CORE_REG_ERR_INT_EN,
                      mask,
                      PMC_ODUJAT_CORE_REG_ERR_INT_EN_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 odujat_reg_ERR_INT_EN_read( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_reg_ERR_INT_EN_read( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odujat_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ODUJAT_CORE_REG_ERR_INT_EN);

    IOLOG( "%s -> 0x%08x;", "odujat_reg_ERR_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void odujat_reg_ERR_INTS_write( odujat_buffer_t *b_ptr,
                                              odujat_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_ERR_INTS_write( odujat_buffer_t *b_ptr,
                                              odujat_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "odujat_reg_ERR_INTS_write", value );
    odujat_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ODUJAT_CORE_REG_ERR_INTS,
                      value);
}

static INLINE void odujat_reg_ERR_INTS_action_on_write_field_set( odujat_buffer_t *b_ptr,
                                                                  odujat_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_reg_ERR_INTS_action_on_write_field_set( odujat_buffer_t *b_ptr,
                                                                  odujat_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "odujat_reg_ERR_INTS_action_on_write_field_set", mask, ofs, value );
    odujat_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_ODUJAT_CORE_REG_ERR_INTS,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 odujat_reg_ERR_INTS_read( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_reg_ERR_INTS_read( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odujat_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ODUJAT_CORE_REG_ERR_INTS);

    IOLOG( "%s -> 0x%08x;", "odujat_reg_ERR_INTS_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE odujat_reg_ERR_INTS_poll( odujat_buffer_t *b_ptr,
                                                             odujat_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odujat_reg_ERR_INTS_poll( odujat_buffer_t *b_ptr,
                                                             odujat_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 value,
                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                             UINT32 max_count,
                                                             UINT32 *num_failed_polls,
                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "odujat_reg_ERR_INTS_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return odujat_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ODUJAT_CORE_REG_ERR_INTS,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 odujat_reg_ERR_CHAN_read( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_reg_ERR_CHAN_read( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = odujat_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ODUJAT_CORE_REG_ERR_CHAN);

    IOLOG( "%s -> 0x%08x;", "odujat_reg_ERR_CHAN_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void odujat_field_FIFO_LIMIT_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_FIFO_LIMIT_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_FIFO_LIMIT_set", value, 3);
    IOLOG( "%s <= 0x%08x", "odujat_field_FIFO_LIMIT_set", value );

    /* (0x00000000 bits 17:16) field FIFO_LIMIT of register PMC_ODUJAT_CORE_REG_CFG */
    odujat_reg_CFG_field_set( b_ptr,
                              h_ptr,
                              ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_MSK,
                              ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_OFF,
                              value);
}

static INLINE UINT32 odujat_field_FIFO_LIMIT_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_FIFO_LIMIT_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 17:16) field FIFO_LIMIT of register PMC_ODUJAT_CORE_REG_CFG */
    reg_value = odujat_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_MSK) >> ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_FIFO_LIMIT_get", value );

    return value;
}
static INLINE void odujat_field_range_FIFO_LIMIT_set( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_range_FIFO_LIMIT_set( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_FIFO_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_FIFO_LIMIT_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_FIFO_LIMIT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000000 bits 17:16) field FIFO_LIMIT of register PMC_ODUJAT_CORE_REG_CFG */
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
        /* (0x00000000 bits 17:16) field FIFO_LIMIT of register PMC_ODUJAT_CORE_REG_CFG */
        odujat_reg_CFG_field_set( b_ptr,
                                  h_ptr,
                                  subfield_mask << (ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_OFF + subfield_offset),
                                  ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_OFF + subfield_offset,
                                  value >> subfield_shift);
    }
}

static INLINE UINT32 odujat_field_range_FIFO_LIMIT_get( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_FIFO_LIMIT_get( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_FIFO_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_FIFO_LIMIT_get", stop_bit, 1 );
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
    /* (0x00000000 bits 17:16) field FIFO_LIMIT of register PMC_ODUJAT_CORE_REG_CFG */
    reg_value = odujat_reg_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_MSK)
                  >> ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_MSK, ODUJAT_CORE_REG_CFG_BIT_FIFO_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_FIFO_LIMIT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void odujat_field_RAM_LS_set( odujat_buffer_t *b_ptr,
                                            odujat_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_RAM_LS_set( odujat_buffer_t *b_ptr,
                                            odujat_handle_t *h_ptr,
                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_RAM_LS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odujat_field_RAM_LS_set", value );

    /* (0x00000000 bits 9) field RAM_LS of register PMC_ODUJAT_CORE_REG_CFG */
    odujat_reg_CFG_field_set( b_ptr,
                              h_ptr,
                              ODUJAT_CORE_REG_CFG_BIT_RAM_LS_MSK,
                              ODUJAT_CORE_REG_CFG_BIT_RAM_LS_OFF,
                              value);
}

static INLINE UINT32 odujat_field_RAM_LS_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_RAM_LS_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 9) field RAM_LS of register PMC_ODUJAT_CORE_REG_CFG */
    reg_value = odujat_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_CFG_BIT_RAM_LS_MSK) >> ODUJAT_CORE_REG_CFG_BIT_RAM_LS_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_RAM_LS_get", value );

    return value;
}
static INLINE void odujat_field_PG_OVERRIDE_set( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_PG_OVERRIDE_set( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_PG_OVERRIDE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odujat_field_PG_OVERRIDE_set", value );

    /* (0x00000000 bits 8) field PG_OVERRIDE of register PMC_ODUJAT_CORE_REG_CFG */
    odujat_reg_CFG_field_set( b_ptr,
                              h_ptr,
                              ODUJAT_CORE_REG_CFG_BIT_PG_OVERRIDE_MSK,
                              ODUJAT_CORE_REG_CFG_BIT_PG_OVERRIDE_OFF,
                              value);
}

static INLINE UINT32 odujat_field_PG_OVERRIDE_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_PG_OVERRIDE_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 8) field PG_OVERRIDE of register PMC_ODUJAT_CORE_REG_CFG */
    reg_value = odujat_reg_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_CFG_BIT_PG_OVERRIDE_MSK) >> ODUJAT_CORE_REG_CFG_BIT_PG_OVERRIDE_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_PG_OVERRIDE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size96
 * ==================================================================================
 */
static INLINE void odujat_field_JAT_ENABLE_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_JAT_ENABLE_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_JAT_ENABLE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_JAT_ENABLE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_JAT_ENABLE_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 31) field JAT_ENABLE of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_JAT_ENABLE_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_JAT_ENABLE_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_JAT_ENABLE_get( odujat_buffer_t *b_ptr,
                                                  odujat_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_JAT_ENABLE_get( odujat_buffer_t *b_ptr,
                                                  odujat_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_JAT_ENABLE_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 31) field JAT_ENABLE of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_JAT_ENABLE_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_JAT_ENABLE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_JAT_ENABLE_get", N, value );

    return value;
}
static INLINE void odujat_field_DQ_AT_LIMIT_set( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_DQ_AT_LIMIT_set( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_DQ_AT_LIMIT_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_DQ_AT_LIMIT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_DQ_AT_LIMIT_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 24) field DQ_AT_LIMIT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_DQ_AT_LIMIT_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_DQ_AT_LIMIT_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_DQ_AT_LIMIT_get( odujat_buffer_t *b_ptr,
                                                   odujat_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_DQ_AT_LIMIT_get( odujat_buffer_t *b_ptr,
                                                   odujat_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_DQ_AT_LIMIT_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 24) field DQ_AT_LIMIT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_DQ_AT_LIMIT_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_DQ_AT_LIMIT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_DQ_AT_LIMIT_get", N, value );

    return value;
}
static INLINE void odujat_field_CND_SELECT_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_CND_SELECT_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_CND_SELECT_set", N, 95);
    if (value > 7)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_CND_SELECT_set", value, 7);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_CND_SELECT_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 20:18) bits 0:2 use field CND_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_CND_SELECT_get( odujat_buffer_t *b_ptr,
                                                  odujat_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_CND_SELECT_get( odujat_buffer_t *b_ptr,
                                                  odujat_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_CND_SELECT_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 20:18) bits 0:2 use field CND_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_CND_SELECT_get", N, value );

    return value;
}
static INLINE void odujat_field_range_CND_SELECT_set( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_range_CND_SELECT_set( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_CND_SELECT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_CND_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_CND_SELECT_set", stop_bit, 2 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_CND_SELECT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000100 + (N) * 0x10) bits 20:18) bits 0:2 use field CND_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
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
        /* ((0x00000100 + (N) * 0x10) bits 20:18) bits 0:2 use field CND_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
        odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_OFF + subfield_offset),
                                               ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odujat_field_range_CND_SELECT_get( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_CND_SELECT_get( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_CND_SELECT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_CND_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 2) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_CND_SELECT_get", stop_bit, 2 );
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
    /* ((0x00000100 + (N) * 0x10) bits 20:18) bits 0:2 use field CND_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_MSK)
                  >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_MSK, ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CND_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_CND_SELECT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odujat_field_ADD_RESIDUE_set( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_ADD_RESIDUE_set( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_ADD_RESIDUE_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_ADD_RESIDUE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_ADD_RESIDUE_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 17) field ADD_RESIDUE of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_RESIDUE_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_RESIDUE_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_ADD_RESIDUE_get( odujat_buffer_t *b_ptr,
                                                   odujat_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_ADD_RESIDUE_get( odujat_buffer_t *b_ptr,
                                                   odujat_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_ADD_RESIDUE_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 17) field ADD_RESIDUE of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_RESIDUE_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_RESIDUE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_ADD_RESIDUE_get", N, value );

    return value;
}
static INLINE void odujat_field_ADD_CND_set( odujat_buffer_t *b_ptr,
                                             odujat_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_ADD_CND_set( odujat_buffer_t *b_ptr,
                                             odujat_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_ADD_CND_set", N, 95);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_ADD_CND_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_ADD_CND_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 16) field ADD_CND of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_CND_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_CND_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_ADD_CND_get( odujat_buffer_t *b_ptr,
                                               odujat_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_ADD_CND_get( odujat_buffer_t *b_ptr,
                                               odujat_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_ADD_CND_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 16) field ADD_CND of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_CND_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_ADD_CND_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_ADD_CND_get", N, value );

    return value;
}
static INLINE void odujat_field_LOOP_ALPHA_SELECT_set( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_LOOP_ALPHA_SELECT_set( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_LOOP_ALPHA_SELECT_set", N, 95);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_LOOP_ALPHA_SELECT_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_LOOP_ALPHA_SELECT_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 11:8) bits 0:3 use field LOOP_ALPHA_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_LOOP_ALPHA_SELECT_get( odujat_buffer_t *b_ptr,
                                                         odujat_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_LOOP_ALPHA_SELECT_get( odujat_buffer_t *b_ptr,
                                                         odujat_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_LOOP_ALPHA_SELECT_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 11:8) bits 0:3 use field LOOP_ALPHA_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_LOOP_ALPHA_SELECT_get", N, value );

    return value;
}
static INLINE void odujat_field_range_LOOP_ALPHA_SELECT_set( odujat_buffer_t *b_ptr,
                                                             odujat_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_range_LOOP_ALPHA_SELECT_set( odujat_buffer_t *b_ptr,
                                                             odujat_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_LOOP_ALPHA_SELECT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_LOOP_ALPHA_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_LOOP_ALPHA_SELECT_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_LOOP_ALPHA_SELECT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000100 + (N) * 0x10) bits 11:8) bits 0:3 use field LOOP_ALPHA_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
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
        /* ((0x00000100 + (N) * 0x10) bits 11:8) bits 0:3 use field LOOP_ALPHA_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
        odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_OFF + subfield_offset),
                                               ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odujat_field_range_LOOP_ALPHA_SELECT_get( odujat_buffer_t *b_ptr,
                                                               odujat_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_LOOP_ALPHA_SELECT_get( odujat_buffer_t *b_ptr,
                                                               odujat_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_LOOP_ALPHA_SELECT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_LOOP_ALPHA_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_LOOP_ALPHA_SELECT_get", stop_bit, 3 );
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
    /* ((0x00000100 + (N) * 0x10) bits 11:8) bits 0:3 use field LOOP_ALPHA_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_MSK)
                  >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_MSK, ODUJAT_CORE_REG_CHAN_CFG_0_BIT_LOOP_ALPHA_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_LOOP_ALPHA_SELECT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odujat_field_CONV_GAIN_SELECT_set( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_CONV_GAIN_SELECT_set( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_CONV_GAIN_SELECT_set", N, 95);
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_CONV_GAIN_SELECT_set", value, 31);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_CONV_GAIN_SELECT_set", N, value );

    /* ((0x00000100 + (N) * 0x10) bits 4:0) bits 0:4 use field CONV_GAIN_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_CONV_GAIN_SELECT_get( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_CONV_GAIN_SELECT_get( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_CONV_GAIN_SELECT_get", N, 95);
    /* ((0x00000100 + (N) * 0x10) bits 4:0) bits 0:4 use field CONV_GAIN_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_CONV_GAIN_SELECT_get", N, value );

    return value;
}
static INLINE void odujat_field_range_CONV_GAIN_SELECT_set( odujat_buffer_t *b_ptr,
                                                            odujat_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_range_CONV_GAIN_SELECT_set( odujat_buffer_t *b_ptr,
                                                            odujat_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_CONV_GAIN_SELECT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_CONV_GAIN_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_CONV_GAIN_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_CONV_GAIN_SELECT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000100 + (N) * 0x10) bits 4:0) bits 0:4 use field CONV_GAIN_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
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
        /* ((0x00000100 + (N) * 0x10) bits 4:0) bits 0:4 use field CONV_GAIN_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
        odujat_reg_CHAN_CFG_0_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_OFF + subfield_offset),
                                               ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odujat_field_range_CONV_GAIN_SELECT_get( odujat_buffer_t *b_ptr,
                                                              odujat_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_CONV_GAIN_SELECT_get( odujat_buffer_t *b_ptr,
                                                              odujat_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_CONV_GAIN_SELECT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_CONV_GAIN_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_CONV_GAIN_SELECT_get", stop_bit, 4 );
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
    /* ((0x00000100 + (N) * 0x10) bits 4:0) bits 0:4 use field CONV_GAIN_SELECT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_0 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_0_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_MSK)
                  >> ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_MSK, ODUJAT_CORE_REG_CHAN_CFG_0_BIT_CONV_GAIN_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_CONV_GAIN_SELECT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odujat_field_MC_NUMERATOR_set( odujat_buffer_t *b_ptr,
                                                  odujat_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_MC_NUMERATOR_set( odujat_buffer_t *b_ptr,
                                                  odujat_handle_t *h_ptr,
                                                  UINT32  N,
                                                  UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_MC_NUMERATOR_set", N, 95);
    if (value > 67108863)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_MC_NUMERATOR_set", value, 67108863);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_MC_NUMERATOR_set", N, value );

    /* ((0x00000104 + (N) * 0x10) bits 25:0) bits 0:25 use field MC_NUMERATOR of register PMC_ODUJAT_CORE_REG_CHAN_CFG_1 index N=0..95 */
    odujat_reg_CHAN_CFG_1_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_MC_NUMERATOR_get( odujat_buffer_t *b_ptr,
                                                    odujat_handle_t *h_ptr,
                                                    UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_MC_NUMERATOR_get( odujat_buffer_t *b_ptr,
                                                    odujat_handle_t *h_ptr,
                                                    UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_MC_NUMERATOR_get", N, 95);
    /* ((0x00000104 + (N) * 0x10) bits 25:0) bits 0:25 use field MC_NUMERATOR of register PMC_ODUJAT_CORE_REG_CHAN_CFG_1 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_1_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_MC_NUMERATOR_get", N, value );

    return value;
}
static INLINE void odujat_field_range_MC_NUMERATOR_set( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_range_MC_NUMERATOR_set( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_MC_NUMERATOR_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_MC_NUMERATOR_set", stop_bit, start_bit );
    if (stop_bit > 25) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_MC_NUMERATOR_set", stop_bit, 25 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_MC_NUMERATOR_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000104 + (N) * 0x10) bits 25:0) bits 0:25 use field MC_NUMERATOR of register PMC_ODUJAT_CORE_REG_CHAN_CFG_1 index N=0..95 */
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
        if (stop_bit < 25) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 25;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* ((0x00000104 + (N) * 0x10) bits 25:0) bits 0:25 use field MC_NUMERATOR of register PMC_ODUJAT_CORE_REG_CHAN_CFG_1 index N=0..95 */
        odujat_reg_CHAN_CFG_1_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_OFF + subfield_offset),
                                               ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odujat_field_range_MC_NUMERATOR_get( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_MC_NUMERATOR_get( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_MC_NUMERATOR_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_MC_NUMERATOR_get", stop_bit, start_bit );
    if (stop_bit > 25) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_MC_NUMERATOR_get", stop_bit, 25 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 25) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 25;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* ((0x00000104 + (N) * 0x10) bits 25:0) bits 0:25 use field MC_NUMERATOR of register PMC_ODUJAT_CORE_REG_CHAN_CFG_1 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_1_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_MSK)
                  >> ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_MSK, ODUJAT_CORE_REG_CHAN_CFG_1_BIT_MC_NUMERATOR_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_MC_NUMERATOR_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odujat_field_FIFO_LEVEL_LIMIT_set( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_FIFO_LEVEL_LIMIT_set( odujat_buffer_t *b_ptr,
                                                      odujat_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_FIFO_LEVEL_LIMIT_set", N, 95);
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_FIFO_LEVEL_LIMIT_set", value, 511);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_FIFO_LEVEL_LIMIT_set", N, value );

    /* ((0x00000108 + (N) * 0x10) bits 24:16) bits 0:8 use field FIFO_LEVEL_LIMIT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
    odujat_reg_CHAN_CFG_2_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_FIFO_LEVEL_LIMIT_get( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_FIFO_LEVEL_LIMIT_get( odujat_buffer_t *b_ptr,
                                                        odujat_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_FIFO_LEVEL_LIMIT_get", N, 95);
    /* ((0x00000108 + (N) * 0x10) bits 24:16) bits 0:8 use field FIFO_LEVEL_LIMIT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_2_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_FIFO_LEVEL_LIMIT_get", N, value );

    return value;
}
static INLINE void odujat_field_range_FIFO_LEVEL_LIMIT_set( odujat_buffer_t *b_ptr,
                                                            odujat_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_range_FIFO_LEVEL_LIMIT_set( odujat_buffer_t *b_ptr,
                                                            odujat_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_FIFO_LEVEL_LIMIT_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_FIFO_LEVEL_LIMIT_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_FIFO_LEVEL_LIMIT_set", stop_bit, 8 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_FIFO_LEVEL_LIMIT_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000108 + (N) * 0x10) bits 24:16) bits 0:8 use field FIFO_LEVEL_LIMIT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
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
        /* ((0x00000108 + (N) * 0x10) bits 24:16) bits 0:8 use field FIFO_LEVEL_LIMIT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
        odujat_reg_CHAN_CFG_2_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_OFF + subfield_offset),
                                               ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odujat_field_range_FIFO_LEVEL_LIMIT_get( odujat_buffer_t *b_ptr,
                                                              odujat_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_FIFO_LEVEL_LIMIT_get( odujat_buffer_t *b_ptr,
                                                              odujat_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_FIFO_LEVEL_LIMIT_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_FIFO_LEVEL_LIMIT_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_FIFO_LEVEL_LIMIT_get", stop_bit, 8 );
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
    /* ((0x00000108 + (N) * 0x10) bits 24:16) bits 0:8 use field FIFO_LEVEL_LIMIT of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_2_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_MSK)
                  >> ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_MSK, ODUJAT_CORE_REG_CHAN_CFG_2_BIT_FIFO_LEVEL_LIMIT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_FIFO_LEVEL_LIMIT_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void odujat_field_CND_DELAY_set( odujat_buffer_t *b_ptr,
                                               odujat_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_CND_DELAY_set( odujat_buffer_t *b_ptr,
                                               odujat_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_CND_DELAY_set", N, 95);
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_CND_DELAY_set", value, 1023);
    IOLOG( "%s <= N=%d 0x%08x", "odujat_field_CND_DELAY_set", N, value );

    /* ((0x00000108 + (N) * 0x10) bits 9:0) bits 0:9 use field CND_DELAY of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
    odujat_reg_CHAN_CFG_2_array_field_set( b_ptr,
                                           h_ptr,
                                           N,
                                           ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_MSK,
                                           ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_OFF,
                                           value);
}

static INLINE UINT32 odujat_field_CND_DELAY_get( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_CND_DELAY_get( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_CND_DELAY_get", N, 95);
    /* ((0x00000108 + (N) * 0x10) bits 9:0) bits 0:9 use field CND_DELAY of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_2_array_read(  b_ptr, h_ptr, N);
    value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_MSK) >> ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "odujat_field_CND_DELAY_get", N, value );

    return value;
}
static INLINE void odujat_field_range_CND_DELAY_set( odujat_buffer_t *b_ptr,
                                                     odujat_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_range_CND_DELAY_set( odujat_buffer_t *b_ptr,
                                                     odujat_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (N > 95)
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_CND_DELAY_set", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_CND_DELAY_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_CND_DELAY_set", stop_bit, 9 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_CND_DELAY_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000108 + (N) * 0x10) bits 9:0) bits 0:9 use field CND_DELAY of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
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
        /* ((0x00000108 + (N) * 0x10) bits 9:0) bits 0:9 use field CND_DELAY of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
        odujat_reg_CHAN_CFG_2_array_field_set( b_ptr,
                                               h_ptr,
                                               N,
                                               subfield_mask << (ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_OFF + subfield_offset),
                                               ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 odujat_field_range_CND_DELAY_get( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_CND_DELAY_get( odujat_buffer_t *b_ptr,
                                                       odujat_handle_t *h_ptr,
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
        IO_RANGE_CHECK("%s N is %d but max is %d", "odujat_field_range_CND_DELAY_get", N, 95);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_CND_DELAY_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_CND_DELAY_get", stop_bit, 9 );
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
    /* ((0x00000108 + (N) * 0x10) bits 9:0) bits 0:9 use field CND_DELAY of register PMC_ODUJAT_CORE_REG_CHAN_CFG_2 index N=0..95 */
    reg_value = odujat_reg_CHAN_CFG_2_array_read(  b_ptr, h_ptr, N);
    field_value = (reg_value & ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_MSK)
                  >> ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_MSK, ODUJAT_CORE_REG_CHAN_CFG_2_BIT_CND_DELAY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_CND_DELAY_get", N, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void odujat_field_DQ_LIMIT_E_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_DQ_LIMIT_E_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_DQ_LIMIT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odujat_field_DQ_LIMIT_E_set", value );

    /* (0x00000004 bits 2) field DQ_LIMIT_E of register PMC_ODUJAT_CORE_REG_ERR_INT_EN */
    odujat_reg_ERR_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     ODUJAT_CORE_REG_ERR_INT_EN_BIT_DQ_LIMIT_E_MSK,
                                     ODUJAT_CORE_REG_ERR_INT_EN_BIT_DQ_LIMIT_E_OFF,
                                     value);
}

static INLINE UINT32 odujat_field_DQ_LIMIT_E_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_DQ_LIMIT_E_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 2) field DQ_LIMIT_E of register PMC_ODUJAT_CORE_REG_ERR_INT_EN */
    reg_value = odujat_reg_ERR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_ERR_INT_EN_BIT_DQ_LIMIT_E_MSK) >> ODUJAT_CORE_REG_ERR_INT_EN_BIT_DQ_LIMIT_E_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_DQ_LIMIT_E_get", value );

    return value;
}
static INLINE void odujat_field_GAIN_UNDF_E_set( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_GAIN_UNDF_E_set( odujat_buffer_t *b_ptr,
                                                 odujat_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_GAIN_UNDF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odujat_field_GAIN_UNDF_E_set", value );

    /* (0x00000004 bits 1) field GAIN_UNDF_E of register PMC_ODUJAT_CORE_REG_ERR_INT_EN */
    odujat_reg_ERR_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_UNDF_E_MSK,
                                     ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_UNDF_E_OFF,
                                     value);
}

static INLINE UINT32 odujat_field_GAIN_UNDF_E_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_GAIN_UNDF_E_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 1) field GAIN_UNDF_E of register PMC_ODUJAT_CORE_REG_ERR_INT_EN */
    reg_value = odujat_reg_ERR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_UNDF_E_MSK) >> ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_UNDF_E_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_GAIN_UNDF_E_get", value );

    return value;
}
static INLINE void odujat_field_GAIN_OVF_E_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_GAIN_OVF_E_set( odujat_buffer_t *b_ptr,
                                                odujat_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_GAIN_OVF_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "odujat_field_GAIN_OVF_E_set", value );

    /* (0x00000004 bits 0) field GAIN_OVF_E of register PMC_ODUJAT_CORE_REG_ERR_INT_EN */
    odujat_reg_ERR_INT_EN_field_set( b_ptr,
                                     h_ptr,
                                     ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_OVF_E_MSK,
                                     ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_OVF_E_OFF,
                                     value);
}

static INLINE UINT32 odujat_field_GAIN_OVF_E_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_GAIN_OVF_E_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field GAIN_OVF_E of register PMC_ODUJAT_CORE_REG_ERR_INT_EN */
    reg_value = odujat_reg_ERR_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_OVF_E_MSK) >> ODUJAT_CORE_REG_ERR_INT_EN_BIT_GAIN_OVF_E_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_GAIN_OVF_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void odujat_field_DQ_LIMIT_I_set_to_clear( odujat_buffer_t *b_ptr,
                                                         odujat_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_DQ_LIMIT_I_set_to_clear( odujat_buffer_t *b_ptr,
                                                         odujat_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_DQ_LIMIT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odujat_field_DQ_LIMIT_I_set_to_clear", value );

    /* (0x00000008 bits 2) field DQ_LIMIT_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    odujat_reg_ERR_INTS_action_on_write_field_set( b_ptr,
                                                   h_ptr,
                                                   ODUJAT_CORE_REG_ERR_INTS_BIT_DQ_LIMIT_I_MSK,
                                                   ODUJAT_CORE_REG_ERR_INTS_BIT_DQ_LIMIT_I_OFF,
                                                   value);
}

static INLINE UINT32 odujat_field_DQ_LIMIT_I_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_DQ_LIMIT_I_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 2) field DQ_LIMIT_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    reg_value = odujat_reg_ERR_INTS_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_ERR_INTS_BIT_DQ_LIMIT_I_MSK) >> ODUJAT_CORE_REG_ERR_INTS_BIT_DQ_LIMIT_I_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_DQ_LIMIT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odujat_field_DQ_LIMIT_I_poll( odujat_buffer_t *b_ptr,
                                                                 odujat_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odujat_field_DQ_LIMIT_I_poll( odujat_buffer_t *b_ptr,
                                                                 odujat_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odujat_field_DQ_LIMIT_I_poll", value );

    /* (0x00000008 bits 2) field DQ_LIMIT_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    return odujat_reg_ERR_INTS_poll( b_ptr,
                                     h_ptr,
                                     ODUJAT_CORE_REG_ERR_INTS_BIT_DQ_LIMIT_I_MSK,
                                     (value<<ODUJAT_CORE_REG_ERR_INTS_BIT_DQ_LIMIT_I_OFF),
                                     cmp,
                                     max_count,
                                     num_failed_polls,
                                     delay_between_polls_in_microseconds);
}

static INLINE void odujat_field_GAIN_UNDF_I_set_to_clear( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_GAIN_UNDF_I_set_to_clear( odujat_buffer_t *b_ptr,
                                                          odujat_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_GAIN_UNDF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odujat_field_GAIN_UNDF_I_set_to_clear", value );

    /* (0x00000008 bits 1) field GAIN_UNDF_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    odujat_reg_ERR_INTS_action_on_write_field_set( b_ptr,
                                                   h_ptr,
                                                   ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_UNDF_I_MSK,
                                                   ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_UNDF_I_OFF,
                                                   value);
}

static INLINE UINT32 odujat_field_GAIN_UNDF_I_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_GAIN_UNDF_I_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 1) field GAIN_UNDF_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    reg_value = odujat_reg_ERR_INTS_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_UNDF_I_MSK) >> ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_UNDF_I_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_GAIN_UNDF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odujat_field_GAIN_UNDF_I_poll( odujat_buffer_t *b_ptr,
                                                                  odujat_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odujat_field_GAIN_UNDF_I_poll( odujat_buffer_t *b_ptr,
                                                                  odujat_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odujat_field_GAIN_UNDF_I_poll", value );

    /* (0x00000008 bits 1) field GAIN_UNDF_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    return odujat_reg_ERR_INTS_poll( b_ptr,
                                     h_ptr,
                                     ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_UNDF_I_MSK,
                                     (value<<ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_UNDF_I_OFF),
                                     cmp,
                                     max_count,
                                     num_failed_polls,
                                     delay_between_polls_in_microseconds);
}

static INLINE void odujat_field_GAIN_OVF_I_set_to_clear( odujat_buffer_t *b_ptr,
                                                         odujat_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void odujat_field_GAIN_OVF_I_set_to_clear( odujat_buffer_t *b_ptr,
                                                         odujat_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "odujat_field_GAIN_OVF_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "odujat_field_GAIN_OVF_I_set_to_clear", value );

    /* (0x00000008 bits 0) field GAIN_OVF_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    odujat_reg_ERR_INTS_action_on_write_field_set( b_ptr,
                                                   h_ptr,
                                                   ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_OVF_I_MSK,
                                                   ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_OVF_I_OFF,
                                                   value);
}

static INLINE UINT32 odujat_field_GAIN_OVF_I_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_GAIN_OVF_I_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field GAIN_OVF_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    reg_value = odujat_reg_ERR_INTS_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_OVF_I_MSK) >> ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_OVF_I_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_GAIN_OVF_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE odujat_field_GAIN_OVF_I_poll( odujat_buffer_t *b_ptr,
                                                                 odujat_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE odujat_field_GAIN_OVF_I_poll( odujat_buffer_t *b_ptr,
                                                                 odujat_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "odujat_field_GAIN_OVF_I_poll", value );

    /* (0x00000008 bits 0) field GAIN_OVF_I of register PMC_ODUJAT_CORE_REG_ERR_INTS */
    return odujat_reg_ERR_INTS_poll( b_ptr,
                                     h_ptr,
                                     ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_OVF_I_MSK,
                                     (value<<ODUJAT_CORE_REG_ERR_INTS_BIT_GAIN_OVF_I_OFF),
                                     cmp,
                                     max_count,
                                     num_failed_polls,
                                     delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 odujat_field_LIMIT_ERR_CHAN_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_LIMIT_ERR_CHAN_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 14:8) field LIMIT_ERR_CHAN of register PMC_ODUJAT_CORE_REG_ERR_CHAN */
    reg_value = odujat_reg_ERR_CHAN_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_ERR_CHAN_BIT_LIMIT_ERR_CHAN_MSK) >> ODUJAT_CORE_REG_ERR_CHAN_BIT_LIMIT_ERR_CHAN_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_LIMIT_ERR_CHAN_get", value );

    return value;
}
static INLINE UINT32 odujat_field_range_LIMIT_ERR_CHAN_get( odujat_buffer_t *b_ptr,
                                                            odujat_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_LIMIT_ERR_CHAN_get( odujat_buffer_t *b_ptr,
                                                            odujat_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_LIMIT_ERR_CHAN_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_LIMIT_ERR_CHAN_get", stop_bit, 6 );
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
    /* (0x0000000c bits 14:8) field LIMIT_ERR_CHAN of register PMC_ODUJAT_CORE_REG_ERR_CHAN */
    reg_value = odujat_reg_ERR_CHAN_read(  b_ptr, h_ptr);
    field_value = (reg_value & ODUJAT_CORE_REG_ERR_CHAN_BIT_LIMIT_ERR_CHAN_MSK)
                  >> ODUJAT_CORE_REG_ERR_CHAN_BIT_LIMIT_ERR_CHAN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_ERR_CHAN_BIT_LIMIT_ERR_CHAN_MSK, ODUJAT_CORE_REG_ERR_CHAN_BIT_LIMIT_ERR_CHAN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_LIMIT_ERR_CHAN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 odujat_field_GAIN_ERR_CHAN_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_GAIN_ERR_CHAN_get( odujat_buffer_t *b_ptr, odujat_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 6:0) field GAIN_ERR_CHAN of register PMC_ODUJAT_CORE_REG_ERR_CHAN */
    reg_value = odujat_reg_ERR_CHAN_read(  b_ptr, h_ptr);
    value = (reg_value & ODUJAT_CORE_REG_ERR_CHAN_BIT_GAIN_ERR_CHAN_MSK) >> ODUJAT_CORE_REG_ERR_CHAN_BIT_GAIN_ERR_CHAN_OFF;
    IOLOG( "%s -> 0x%08x", "odujat_field_GAIN_ERR_CHAN_get", value );

    return value;
}
static INLINE UINT32 odujat_field_range_GAIN_ERR_CHAN_get( odujat_buffer_t *b_ptr,
                                                           odujat_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 odujat_field_range_GAIN_ERR_CHAN_get( odujat_buffer_t *b_ptr,
                                                           odujat_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "odujat_field_range_GAIN_ERR_CHAN_get", stop_bit, start_bit );
    if (stop_bit > 6) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "odujat_field_range_GAIN_ERR_CHAN_get", stop_bit, 6 );
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
    /* (0x0000000c bits 6:0) field GAIN_ERR_CHAN of register PMC_ODUJAT_CORE_REG_ERR_CHAN */
    reg_value = odujat_reg_ERR_CHAN_read(  b_ptr, h_ptr);
    field_value = (reg_value & ODUJAT_CORE_REG_ERR_CHAN_BIT_GAIN_ERR_CHAN_MSK)
                  >> ODUJAT_CORE_REG_ERR_CHAN_BIT_GAIN_ERR_CHAN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ODUJAT_CORE_REG_ERR_CHAN_BIT_GAIN_ERR_CHAN_MSK, ODUJAT_CORE_REG_ERR_CHAN_BIT_GAIN_ERR_CHAN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "odujat_field_range_GAIN_ERR_CHAN_get", start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ODUJAT_IO_INLINE_H */
