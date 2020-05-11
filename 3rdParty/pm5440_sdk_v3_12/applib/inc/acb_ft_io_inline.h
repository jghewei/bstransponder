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
 *     and register accessor functions for the acb_ft block
 *****************************************************************************/
#ifndef _ACB_FT_IO_INLINE_H
#define _ACB_FT_IO_INLINE_H

#include "acb_ft.h"
#include "acb_ft_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ACB_FT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for acb_ft
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
    acb_ft_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} acb_ft_buffer_t;
static INLINE void acb_ft_buffer_init( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void acb_ft_buffer_init( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "acb_ft_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void acb_ft_buffer_flush( acb_ft_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void acb_ft_buffer_flush( acb_ft_buffer_t *b_ptr )
{
    IOLOG( "acb_ft_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 acb_ft_reg_read( acb_ft_buffer_t *b_ptr,
                                      acb_ft_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_read( acb_ft_buffer_t *b_ptr,
                                      acb_ft_handle_t *h_ptr,
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
static INLINE void acb_ft_reg_write( acb_ft_buffer_t *b_ptr,
                                     acb_ft_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_write( acb_ft_buffer_t *b_ptr,
                                     acb_ft_handle_t *h_ptr,
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

static INLINE void acb_ft_field_set( acb_ft_buffer_t *b_ptr,
                                     acb_ft_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_set( acb_ft_buffer_t *b_ptr,
                                     acb_ft_handle_t *h_ptr,
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

static INLINE void acb_ft_action_on_write_field_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_action_on_write_field_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
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

static INLINE void acb_ft_burst_read( acb_ft_buffer_t *b_ptr,
                                      acb_ft_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void acb_ft_burst_read( acb_ft_buffer_t *b_ptr,
                                      acb_ft_handle_t *h_ptr,
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

static INLINE void acb_ft_burst_write( acb_ft_buffer_t *b_ptr,
                                       acb_ft_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void acb_ft_burst_write( acb_ft_buffer_t *b_ptr,
                                       acb_ft_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE acb_ft_poll( acb_ft_buffer_t *b_ptr,
                                                acb_ft_handle_t *h_ptr,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE acb_ft_poll( acb_ft_buffer_t *b_ptr,
                                                acb_ft_handle_t *h_ptr,
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
 *  register access functions for acb_ft
 * ==================================================================================
 */

static INLINE void acb_ft_reg_ACB_FT_EN_REG_write( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_EN_REG_write( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_EN_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_EN_REG,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_EN_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_EN_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_EN_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_EN_REG,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_EN_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_EN_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_EN_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_HOLDOVER_REG_write( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_HOLDOVER_REG_write( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_HOLDOVER_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_HOLDOVER_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                             acb_ft_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_HOLDOVER_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                             acb_ft_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_HOLDOVER_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_HOLDOVER_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_HOLDOVER_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_HOLDOVER_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_GAIN_REG_1_write( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_GAIN_REG_1_write( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_GAIN_REG_1_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_GAIN_REG_1_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_GAIN_REG_1_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_GAIN_REG_1_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_GAIN_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_GAIN_REG_2_write( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_GAIN_REG_2_write( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_GAIN_REG_2_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_GAIN_REG_2_field_set( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_GAIN_REG_2_field_set( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_GAIN_REG_2_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_GAIN_REG_2_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_GAIN_REG_2_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_GAIN_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_write( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_write( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG,
                      value);
}

static INLINE void acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG,
                      mask,
                      PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_write( acb_ft_buffer_t *b_ptr,
                                                                     acb_ft_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_write( acb_ft_buffer_t *b_ptr,
                                                                     acb_ft_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG,
                      value);
}

static INLINE void acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                         acb_ft_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                         acb_ft_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG,
                      mask,
                      PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_write( acb_ft_buffer_t *b_ptr,
                                                                     acb_ft_handle_t *h_ptr,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_write( acb_ft_buffer_t *b_ptr,
                                                                     acb_ft_handle_t *h_ptr,
                                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG,
                      value);
}

static INLINE void acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                         acb_ft_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                         acb_ft_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG,
                      mask,
                      PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_write( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_write( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG,
                      value);
}

static INLINE void acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                           acb_ft_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                           acb_ft_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG,
                      mask,
                      PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_write( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_write( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG,
                      value);
}

static INLINE void acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                           acb_ft_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                           acb_ft_handle_t *h_ptr,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG,
                      mask,
                      PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_write( acb_ft_buffer_t *b_ptr,
                                                                acb_ft_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_write( acb_ft_buffer_t *b_ptr,
                                                                acb_ft_handle_t *h_ptr,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                    acb_ft_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                    acb_ft_handle_t *h_ptr,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_write( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_write( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_field_set( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_field_set( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_write( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_write( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_field_set( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_field_set( acb_ft_buffer_t *b_ptr,
                                                                       acb_ft_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_write( acb_ft_buffer_t *b_ptr,
                                                               acb_ft_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_write( acb_ft_buffer_t *b_ptr,
                                                               acb_ft_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_write( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_field_set( acb_ft_buffer_t *b_ptr,
                                                                      acb_ft_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 ofs,
                                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_CONFIG,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_CONFIG,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_ACB_FT_INT_REG_write( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_INT_REG_write( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_INT_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ACB_FT_REG_ACB_FT_INT_REG,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_INT_REG_action_on_write_field_set( acb_ft_buffer_t *b_ptr,
                                                                        acb_ft_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_INT_REG_action_on_write_field_set( acb_ft_buffer_t *b_ptr,
                                                                        acb_ft_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_INT_REG_action_on_write_field_set", mask, ofs, value );
    acb_ft_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      MEM_TYPE_STATUS,
                                      PMC_ACB_FT_REG_ACB_FT_INT_REG,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_INT_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_INT_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_INT_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE acb_ft_reg_ACB_FT_INT_REG_poll( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE acb_ft_reg_ACB_FT_INT_REG_poll( acb_ft_buffer_t *b_ptr,
                                                                   acb_ft_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "acb_ft_reg_ACB_FT_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return acb_ft_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ACB_FT_REG_ACB_FT_INT_REG,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void acb_ft_reg_ACB_FT_INT_EN_REG_write( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_INT_EN_REG_write( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_ACB_FT_INT_EN_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ACB_FT_REG_ACB_FT_INT_EN_REG,
                      value);
}

static INLINE void acb_ft_reg_ACB_FT_INT_EN_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_ACB_FT_INT_EN_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_ACB_FT_INT_EN_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ACB_FT_REG_ACB_FT_INT_EN_REG,
                      mask,
                      PMC_ACB_FT_REG_ACB_FT_INT_EN_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_ACB_FT_INT_EN_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_INT_EN_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 acb_ft_reg_ACB_FT_INT_STAT_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_INT_STAT_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_INT_STAT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE acb_ft_reg_ACB_FT_INT_STAT_REG_poll( acb_ft_buffer_t *b_ptr,
                                                                        acb_ft_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE acb_ft_reg_ACB_FT_INT_STAT_REG_poll( acb_ft_buffer_t *b_ptr,
                                                                        acb_ft_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "acb_ft_reg_ACB_FT_INT_STAT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return acb_ft_poll( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_ACB_FT_REG_ACB_FT_INT_STAT_REG,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_1_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_1_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_1_read", reg_value);
    return reg_value;
}

static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_2_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_2_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_2_read", reg_value);
    return reg_value;
}

static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_3_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_3_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_3_read", reg_value);
    return reg_value;
}

static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_4_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_4_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_4_read", reg_value);
    return reg_value;
}

static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_5_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_5_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_5_read", reg_value);
    return reg_value;
}

static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_8_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_8_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_8_read", reg_value);
    return reg_value;
}

static INLINE void acb_ft_reg_SW_RESET_REG_write( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_SW_RESET_REG_write( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "acb_ft_reg_SW_RESET_REG_write", value );
    acb_ft_reg_write( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ACB_FT_REG_SW_RESET_REG,
                      value);
}

static INLINE void acb_ft_reg_SW_RESET_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_reg_SW_RESET_REG_field_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "acb_ft_reg_SW_RESET_REG_field_set", mask, ofs, value );
    acb_ft_field_set( b_ptr,
                      h_ptr,
                      MEM_TYPE_STATUS,
                      PMC_ACB_FT_REG_SW_RESET_REG,
                      mask,
                      PMC_ACB_FT_REG_SW_RESET_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 acb_ft_reg_SW_RESET_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_reg_SW_RESET_REG_read( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = acb_ft_reg_read( b_ptr,
                                 h_ptr,
                                 MEM_TYPE_STATUS,
                                 PMC_ACB_FT_REG_SW_RESET_REG);

    IOLOG( "%s -> 0x%08x;", "acb_ft_reg_SW_RESET_REG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void acb_ft_field_FIFO_CENT_ACCEL_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FIFO_CENT_ACCEL_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FIFO_CENT_ACCEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FIFO_CENT_ACCEL_set", value );

    /* (0x00000004 bits 7) field FIFO_CENT_ACCEL of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    acb_ft_reg_ACB_FT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ACCEL_MSK,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ACCEL_OFF,
                                        value);
}

static INLINE UINT32 acb_ft_field_FIFO_CENT_ACCEL_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FIFO_CENT_ACCEL_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 7) field FIFO_CENT_ACCEL of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ACCEL_MSK) >> ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ACCEL_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FIFO_CENT_ACCEL_get", value );

    return value;
}
static INLINE void acb_ft_field_B_CKCTL_MODE_set( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_B_CKCTL_MODE_set( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_B_CKCTL_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_B_CKCTL_MODE_set", value );

    /* (0x00000004 bits 6) field B_CKCTL_MODE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    acb_ft_reg_ACB_FT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_B_CKCTL_MODE_MSK,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_B_CKCTL_MODE_OFF,
                                        value);
}

static INLINE UINT32 acb_ft_field_B_CKCTL_MODE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_B_CKCTL_MODE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 6) field B_CKCTL_MODE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_EN_REG_BIT_B_CKCTL_MODE_MSK) >> ACB_FT_REG_ACB_FT_EN_REG_BIT_B_CKCTL_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_B_CKCTL_MODE_get", value );

    return value;
}
static INLINE void acb_ft_field_C_CKCTL_MODE_set( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_C_CKCTL_MODE_set( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_C_CKCTL_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_C_CKCTL_MODE_set", value );

    /* (0x00000004 bits 5) field C_CKCTL_MODE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    acb_ft_reg_ACB_FT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_C_CKCTL_MODE_MSK,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_C_CKCTL_MODE_OFF,
                                        value);
}

static INLINE UINT32 acb_ft_field_C_CKCTL_MODE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_C_CKCTL_MODE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 5) field C_CKCTL_MODE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_EN_REG_BIT_C_CKCTL_MODE_MSK) >> ACB_FT_REG_ACB_FT_EN_REG_BIT_C_CKCTL_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_C_CKCTL_MODE_get", value );

    return value;
}
static INLINE void acb_ft_field_FIFO_CENT_TRIGGER_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FIFO_CENT_TRIGGER_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FIFO_CENT_TRIGGER_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FIFO_CENT_TRIGGER_set", value );

    /* (0x00000004 bits 4) field FIFO_CENT_TRIGGER of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    acb_ft_reg_ACB_FT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_TRIGGER_MSK,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_TRIGGER_OFF,
                                        value);
}

static INLINE UINT32 acb_ft_field_FIFO_CENT_TRIGGER_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FIFO_CENT_TRIGGER_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 4) field FIFO_CENT_TRIGGER of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_TRIGGER_MSK) >> ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_TRIGGER_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FIFO_CENT_TRIGGER_get", value );

    return value;
}
static INLINE void acb_ft_field_FIFO_CENT_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FIFO_CENT_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FIFO_CENT_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FIFO_CENT_ENABLE_set", value );

    /* (0x00000004 bits 3) field FIFO_CENT_ENABLE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    acb_ft_reg_ACB_FT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ENABLE_MSK,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ENABLE_OFF,
                                        value);
}

static INLINE UINT32 acb_ft_field_FIFO_CENT_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FIFO_CENT_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 3) field FIFO_CENT_ENABLE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ENABLE_MSK) >> ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_CENT_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FIFO_CENT_ENABLE_get", value );

    return value;
}
static INLINE void acb_ft_field_B_INTERFACE_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                        acb_ft_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_B_INTERFACE_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                        acb_ft_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_B_INTERFACE_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_B_INTERFACE_ENABLE_set", value );

    /* (0x00000004 bits 2) field B_INTERFACE_ENABLE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    acb_ft_reg_ACB_FT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_B_INTERFACE_ENABLE_MSK,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_B_INTERFACE_ENABLE_OFF,
                                        value);
}

static INLINE UINT32 acb_ft_field_B_INTERFACE_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_B_INTERFACE_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 2) field B_INTERFACE_ENABLE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_EN_REG_BIT_B_INTERFACE_ENABLE_MSK) >> ACB_FT_REG_ACB_FT_EN_REG_BIT_B_INTERFACE_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_B_INTERFACE_ENABLE_get", value );

    return value;
}
static INLINE void acb_ft_field_FIFO_ADAPT_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FIFO_ADAPT_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FIFO_ADAPT_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FIFO_ADAPT_ENABLE_set", value );

    /* (0x00000004 bits 1) field FIFO_ADAPT_ENABLE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    acb_ft_reg_ACB_FT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_ADAPT_ENABLE_MSK,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_ADAPT_ENABLE_OFF,
                                        value);
}

static INLINE UINT32 acb_ft_field_FIFO_ADAPT_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FIFO_ADAPT_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 1) field FIFO_ADAPT_ENABLE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_ADAPT_ENABLE_MSK) >> ACB_FT_REG_ACB_FT_EN_REG_BIT_FIFO_ADAPT_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FIFO_ADAPT_ENABLE_get", value );

    return value;
}
static INLINE void acb_ft_field_PMM_ADAPT_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PMM_ADAPT_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PMM_ADAPT_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PMM_ADAPT_ENABLE_set", value );

    /* (0x00000004 bits 0) field PMM_ADAPT_ENABLE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    acb_ft_reg_ACB_FT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_PMM_ADAPT_ENABLE_MSK,
                                        ACB_FT_REG_ACB_FT_EN_REG_BIT_PMM_ADAPT_ENABLE_OFF,
                                        value);
}

static INLINE UINT32 acb_ft_field_PMM_ADAPT_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PMM_ADAPT_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field PMM_ADAPT_ENABLE of register PMC_ACB_FT_REG_ACB_FT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_EN_REG_BIT_PMM_ADAPT_ENABLE_MSK) >> ACB_FT_REG_ACB_FT_EN_REG_BIT_PMM_ADAPT_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PMM_ADAPT_ENABLE_get", value );

    return value;
}
static INLINE void acb_ft_field_HOLDOVER_set( acb_ft_buffer_t *b_ptr,
                                              acb_ft_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_HOLDOVER_set( acb_ft_buffer_t *b_ptr,
                                              acb_ft_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_HOLDOVER_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_HOLDOVER_set", value );

    /* (0x00000008 bits 1) field HOLDOVER of register PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG */
    acb_ft_reg_ACB_FT_HOLDOVER_REG_field_set( b_ptr,
                                              h_ptr,
                                              ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_HOLDOVER_MSK,
                                              ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_HOLDOVER_OFF,
                                              value);
}

static INLINE UINT32 acb_ft_field_HOLDOVER_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_HOLDOVER_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 1) field HOLDOVER of register PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG */
    reg_value = acb_ft_reg_ACB_FT_HOLDOVER_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_HOLDOVER_MSK) >> ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_HOLDOVER_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_HOLDOVER_get", value );

    return value;
}
static INLINE void acb_ft_field_FORCE_NOMINAL_set( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FORCE_NOMINAL_set( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FORCE_NOMINAL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FORCE_NOMINAL_set", value );

    /* (0x00000008 bits 0) field FORCE_NOMINAL of register PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG */
    acb_ft_reg_ACB_FT_HOLDOVER_REG_field_set( b_ptr,
                                              h_ptr,
                                              ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_FORCE_NOMINAL_MSK,
                                              ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_FORCE_NOMINAL_OFF,
                                              value);
}

static INLINE UINT32 acb_ft_field_FORCE_NOMINAL_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FORCE_NOMINAL_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000008 bits 0) field FORCE_NOMINAL of register PMC_ACB_FT_REG_ACB_FT_HOLDOVER_REG */
    reg_value = acb_ft_reg_ACB_FT_HOLDOVER_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_FORCE_NOMINAL_MSK) >> ACB_FT_REG_ACB_FT_HOLDOVER_REG_BIT_FORCE_NOMINAL_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FORCE_NOMINAL_get", value );

    return value;
}
static INLINE void acb_ft_field_PLL_S_MIN_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PLL_S_MIN_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PLL_S_MIN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PLL_S_MIN_set", value );

    /* (0x0000000c bits 31:28) field PLL_S_MIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_PLL_S_MIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PLL_S_MIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 31:28) field PLL_S_MIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PLL_S_MIN_get", value );

    return value;
}
static INLINE void acb_ft_field_range_PLL_S_MIN_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_PLL_S_MIN_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PLL_S_MIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PLL_S_MIN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PLL_S_MIN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 31:28) field PLL_S_MIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
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
        /* (0x0000000c bits 31:28) field PLL_S_MIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
        acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_PLL_S_MIN_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PLL_S_MIN_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PLL_S_MIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PLL_S_MIN_get", stop_bit, 3 );
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
    /* (0x0000000c bits 31:28) field PLL_S_MIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_PLL_S_MIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PLL_S_MIN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_DWELL_MAX_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_DWELL_MAX_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_DWELL_MAX_set", value, 15);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_DWELL_MAX_set", value );

    /* (0x0000000c bits 27:24) field DWELL_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_DWELL_MAX_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_DWELL_MAX_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 27:24) field DWELL_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_DWELL_MAX_get", value );

    return value;
}
static INLINE void acb_ft_field_range_DWELL_MAX_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_DWELL_MAX_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_DWELL_MAX_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_DWELL_MAX_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_DWELL_MAX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 27:24) field DWELL_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
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
        /* (0x0000000c bits 27:24) field DWELL_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
        acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_DWELL_MAX_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_DWELL_MAX_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_DWELL_MAX_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_DWELL_MAX_get", stop_bit, 3 );
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
    /* (0x0000000c bits 27:24) field DWELL_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DWELL_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_DWELL_MAX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_FREQ_GAIN_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FREQ_GAIN_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FREQ_GAIN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FREQ_GAIN_set", value );

    /* (0x0000000c bits 23:20) field FREQ_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_FREQ_GAIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FREQ_GAIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 23:20) field FREQ_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FREQ_GAIN_get", value );

    return value;
}
static INLINE void acb_ft_field_range_FREQ_GAIN_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_FREQ_GAIN_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FREQ_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FREQ_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FREQ_GAIN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 23:20) field FREQ_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
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
        /* (0x0000000c bits 23:20) field FREQ_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
        acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_FREQ_GAIN_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_FREQ_GAIN_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FREQ_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FREQ_GAIN_get", stop_bit, 3 );
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
    /* (0x0000000c bits 23:20) field FREQ_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FREQ_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FREQ_GAIN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_DITHER_SEL_set( acb_ft_buffer_t *b_ptr,
                                                acb_ft_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_DITHER_SEL_set( acb_ft_buffer_t *b_ptr,
                                                acb_ft_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_DITHER_SEL_set", value, 3);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_DITHER_SEL_set", value );

    /* (0x0000000c bits 19:18) field DITHER_SEL of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_DITHER_SEL_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_DITHER_SEL_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 19:18) field DITHER_SEL of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_DITHER_SEL_get", value );

    return value;
}
static INLINE void acb_ft_field_range_DITHER_SEL_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_DITHER_SEL_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit,
                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_DITHER_SEL_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_DITHER_SEL_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_DITHER_SEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 19:18) field DITHER_SEL of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
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
        /* (0x0000000c bits 19:18) field DITHER_SEL of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
        acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_DITHER_SEL_get( acb_ft_buffer_t *b_ptr,
                                                        acb_ft_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_DITHER_SEL_get( acb_ft_buffer_t *b_ptr,
                                                        acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_DITHER_SEL_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_DITHER_SEL_get", stop_bit, 1 );
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
    /* (0x0000000c bits 19:18) field DITHER_SEL of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_DITHER_SEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_DITHER_SEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_SEED_set( acb_ft_buffer_t *b_ptr,
                                          acb_ft_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_SEED_set( acb_ft_buffer_t *b_ptr,
                                          acb_ft_handle_t *h_ptr,
                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_SEED_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_SEED_set", value );

    /* (0x0000000c bits 16) field SEED of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_SEED_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_SEED_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_SEED_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_SEED_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 16) field SEED of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_SEED_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_SEED_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_SEED_get", value );

    return value;
}
static INLINE void acb_ft_field_FINAL_SHIFT_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FINAL_SHIFT_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FINAL_SHIFT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FINAL_SHIFT_set", value );

    /* (0x0000000c bits 11:8) field FINAL_SHIFT of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_FINAL_SHIFT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FINAL_SHIFT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 11:8) field FINAL_SHIFT of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FINAL_SHIFT_get", value );

    return value;
}
static INLINE void acb_ft_field_range_FINAL_SHIFT_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_FINAL_SHIFT_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FINAL_SHIFT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FINAL_SHIFT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FINAL_SHIFT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 11:8) field FINAL_SHIFT of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
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
        /* (0x0000000c bits 11:8) field FINAL_SHIFT of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
        acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_FINAL_SHIFT_get( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_FINAL_SHIFT_get( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FINAL_SHIFT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FINAL_SHIFT_get", stop_bit, 3 );
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
    /* (0x0000000c bits 11:8) field FINAL_SHIFT of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FINAL_SHIFT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FINAL_SHIFT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_INT_GAIN_set( acb_ft_buffer_t *b_ptr,
                                              acb_ft_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_INT_GAIN_set( acb_ft_buffer_t *b_ptr,
                                              acb_ft_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_INT_GAIN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_INT_GAIN_set", value );

    /* (0x0000000c bits 7:4) field INT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_INT_GAIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_INT_GAIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 7:4) field INT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_INT_GAIN_get", value );

    return value;
}
static INLINE void acb_ft_field_range_INT_GAIN_set( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_INT_GAIN_set( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_INT_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_INT_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_INT_GAIN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 7:4) field INT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
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
        /* (0x0000000c bits 7:4) field INT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
        acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_INT_GAIN_get( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_INT_GAIN_get( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_INT_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_INT_GAIN_get", stop_bit, 3 );
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
    /* (0x0000000c bits 7:4) field INT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_INT_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_INT_GAIN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_FILT_GAIN_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FILT_GAIN_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FILT_GAIN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FILT_GAIN_set", value );

    /* (0x0000000c bits 3:0) field FILT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_FILT_GAIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FILT_GAIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000000c bits 3:0) field FILT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FILT_GAIN_get", value );

    return value;
}
static INLINE void acb_ft_field_range_FILT_GAIN_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_FILT_GAIN_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FILT_GAIN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FILT_GAIN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FILT_GAIN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000000c bits 3:0) field FILT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
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
        /* (0x0000000c bits 3:0) field FILT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
        acb_ft_reg_ACB_FT_GAIN_REG_1_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_FILT_GAIN_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_FILT_GAIN_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FILT_GAIN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FILT_GAIN_get", stop_bit, 3 );
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
    /* (0x0000000c bits 3:0) field FILT_GAIN of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_1_BIT_FILT_GAIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FILT_GAIN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_OUTPUT_DSM_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_OUTPUT_DSM_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_OUTPUT_DSM_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_OUTPUT_DSM_ENABLE_set", value );

    /* (0x00000010 bits 17) field OUTPUT_DSM_ENABLE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    acb_ft_reg_ACB_FT_GAIN_REG_2_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_DSM_ENABLE_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_DSM_ENABLE_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_OUTPUT_DSM_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_OUTPUT_DSM_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 17) field OUTPUT_DSM_ENABLE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_2_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_DSM_ENABLE_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_DSM_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_OUTPUT_DSM_ENABLE_get", value );

    return value;
}
static INLINE void acb_ft_field_PHASE_FILT_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PHASE_FILT_ENABLE_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PHASE_FILT_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PHASE_FILT_ENABLE_set", value );

    /* (0x00000010 bits 16) field PHASE_FILT_ENABLE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    acb_ft_reg_ACB_FT_GAIN_REG_2_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PHASE_FILT_ENABLE_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PHASE_FILT_ENABLE_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_PHASE_FILT_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PHASE_FILT_ENABLE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 16) field PHASE_FILT_ENABLE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_2_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PHASE_FILT_ENABLE_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PHASE_FILT_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PHASE_FILT_ENABLE_get", value );

    return value;
}
static INLINE void acb_ft_field_OUTPUT_UPDATE_RATE_set( acb_ft_buffer_t *b_ptr,
                                                        acb_ft_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_OUTPUT_UPDATE_RATE_set( acb_ft_buffer_t *b_ptr,
                                                        acb_ft_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_OUTPUT_UPDATE_RATE_set", value, 255);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_OUTPUT_UPDATE_RATE_set", value );

    /* (0x00000010 bits 15:8) field OUTPUT_UPDATE_RATE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    acb_ft_reg_ACB_FT_GAIN_REG_2_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_OUTPUT_UPDATE_RATE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_OUTPUT_UPDATE_RATE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 15:8) field OUTPUT_UPDATE_RATE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_2_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_OUTPUT_UPDATE_RATE_get", value );

    return value;
}
static INLINE void acb_ft_field_range_OUTPUT_UPDATE_RATE_set( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_OUTPUT_UPDATE_RATE_set( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_OUTPUT_UPDATE_RATE_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_OUTPUT_UPDATE_RATE_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_OUTPUT_UPDATE_RATE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 15:8) field OUTPUT_UPDATE_RATE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
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
        /* (0x00000010 bits 15:8) field OUTPUT_UPDATE_RATE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
        acb_ft_reg_ACB_FT_GAIN_REG_2_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_OUTPUT_UPDATE_RATE_get( acb_ft_buffer_t *b_ptr,
                                                                acb_ft_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_OUTPUT_UPDATE_RATE_get( acb_ft_buffer_t *b_ptr,
                                                                acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_OUTPUT_UPDATE_RATE_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_OUTPUT_UPDATE_RATE_get", stop_bit, 7 );
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
    /* (0x00000010 bits 15:8) field OUTPUT_UPDATE_RATE of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_OUTPUT_UPDATE_RATE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_OUTPUT_UPDATE_RATE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_PLL_S_MAX_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PLL_S_MAX_set( acb_ft_buffer_t *b_ptr,
                                               acb_ft_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PLL_S_MAX_set", value, 31);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PLL_S_MAX_set", value );

    /* (0x00000010 bits 4:0) field PLL_S_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    acb_ft_reg_ACB_FT_GAIN_REG_2_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_MSK,
                                            ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_PLL_S_MAX_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PLL_S_MAX_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000010 bits 4:0) field PLL_S_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_2_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_MSK) >> ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PLL_S_MAX_get", value );

    return value;
}
static INLINE void acb_ft_field_range_PLL_S_MAX_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_PLL_S_MAX_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PLL_S_MAX_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PLL_S_MAX_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PLL_S_MAX_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000010 bits 4:0) field PLL_S_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
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
        /* (0x00000010 bits 4:0) field PLL_S_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
        acb_ft_reg_ACB_FT_GAIN_REG_2_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_OFF + subfield_offset),
                                                ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_PLL_S_MAX_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PLL_S_MAX_get( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PLL_S_MAX_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PLL_S_MAX_get", stop_bit, 4 );
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
    /* (0x00000010 bits 4:0) field PLL_S_MAX of register PMC_ACB_FT_REG_ACB_FT_GAIN_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_GAIN_REG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_MSK)
                  >> ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_MSK, ACB_FT_REG_ACB_FT_GAIN_REG_2_BIT_PLL_S_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PLL_S_MAX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_PKT_PERIOD_INT_set( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PKT_PERIOD_INT_set( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PKT_PERIOD_INT_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PKT_PERIOD_INT_set", value );

    /* (0x00000014 bits 15:0) field PKT_PERIOD_INT of register PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG */
    acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_MSK,
                                                        ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_OFF,
                                                        value);
}

static INLINE UINT32 acb_ft_field_PKT_PERIOD_INT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PKT_PERIOD_INT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000014 bits 15:0) field PKT_PERIOD_INT of register PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_MSK) >> ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PKT_PERIOD_INT_get", value );

    return value;
}
static INLINE void acb_ft_field_range_PKT_PERIOD_INT_set( acb_ft_buffer_t *b_ptr,
                                                          acb_ft_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_PKT_PERIOD_INT_set( acb_ft_buffer_t *b_ptr,
                                                          acb_ft_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_INT_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_INT_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_INT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000014 bits 15:0) field PKT_PERIOD_INT of register PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG */
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
        /* (0x00000014 bits 15:0) field PKT_PERIOD_INT of register PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG */
        acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_OFF + subfield_offset),
                                                            ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_INT_get( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_INT_get( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_INT_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_INT_get", stop_bit, 15 );
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
    /* (0x00000014 bits 15:0) field PKT_PERIOD_INT of register PMC_ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_MSK)
                  >> ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_MSK, ACB_FT_REG_PACKET_PERIOD_INTEGER_CFG_REG_BIT_PKT_PERIOD_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_PKT_PERIOD_N_MSB_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PKT_PERIOD_N_MSB_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PKT_PERIOD_N_MSB_set", value, 511);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PKT_PERIOD_N_MSB_set", value );

    /* (0x00000018 bits 8:0) field PKT_PERIOD_N_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG */
    acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_MSK,
                                                          ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_OFF,
                                                          value);
}

static INLINE UINT32 acb_ft_field_PKT_PERIOD_N_MSB_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PKT_PERIOD_N_MSB_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000018 bits 8:0) field PKT_PERIOD_N_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_MSK) >> ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PKT_PERIOD_N_MSB_get", value );

    return value;
}
static INLINE void acb_ft_field_range_PKT_PERIOD_N_MSB_set( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_PKT_PERIOD_N_MSB_set( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_N_MSB_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_N_MSB_set", stop_bit, 8 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_N_MSB_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000018 bits 8:0) field PKT_PERIOD_N_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG */
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
        /* (0x00000018 bits 8:0) field PKT_PERIOD_N_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG */
        acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_OFF + subfield_offset),
                                                              ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_N_MSB_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_N_MSB_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_N_MSB_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_N_MSB_get", stop_bit, 8 );
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
    /* (0x00000018 bits 8:0) field PKT_PERIOD_N_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_read( b_ptr,
                                                                 h_ptr);
    field_value = (reg_value & ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_MSK)
                  >> ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_MSK, ACB_FT_REG_PACKET_PERIOD_NUMERATOR_MSB_REG_BIT_PKT_PERIOD_N_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_N_MSB_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_PKT_PERIOD_N_LSB_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PKT_PERIOD_N_LSB_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PKT_PERIOD_N_LSB_set", value );

    /* (0x0000001c bits 31:0) field PKT_PERIOD_N_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG */
    acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_field_set( b_ptr,
                                                          h_ptr,
                                                          ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_MSK,
                                                          ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_OFF,
                                                          value);
}

static INLINE UINT32 acb_ft_field_PKT_PERIOD_N_LSB_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PKT_PERIOD_N_LSB_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000001c bits 31:0) field PKT_PERIOD_N_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_read( b_ptr,
                                                                 h_ptr);
    value = (reg_value & ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_MSK) >> ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PKT_PERIOD_N_LSB_get", value );

    return value;
}
static INLINE void acb_ft_field_range_PKT_PERIOD_N_LSB_set( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_PKT_PERIOD_N_LSB_set( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_N_LSB_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_N_LSB_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_N_LSB_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000001c bits 31:0) field PKT_PERIOD_N_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG */
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
        /* (0x0000001c bits 31:0) field PKT_PERIOD_N_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG */
        acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              subfield_mask << (ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_OFF + subfield_offset),
                                                              ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_OFF + subfield_offset,
                                                              value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_N_LSB_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_N_LSB_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_N_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_N_LSB_get", stop_bit, 31 );
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
    /* (0x0000001c bits 31:0) field PKT_PERIOD_N_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_read( b_ptr,
                                                                 h_ptr);
    field_value = (reg_value & ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_MSK)
                  >> ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_MSK, ACB_FT_REG_PACKET_PERIOD_NUMERATOR_LSB_REG_BIT_PKT_PERIOD_N_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_N_LSB_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_PKT_PERIOD_D_MSB_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PKT_PERIOD_D_MSB_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PKT_PERIOD_D_MSB_set", value, 255);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PKT_PERIOD_D_MSB_set", value );

    /* (0x00000020 bits 7:0) field PKT_PERIOD_D_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG */
    acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_MSK,
                                                            ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_OFF,
                                                            value);
}

static INLINE UINT32 acb_ft_field_PKT_PERIOD_D_MSB_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PKT_PERIOD_D_MSB_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000020 bits 7:0) field PKT_PERIOD_D_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_MSK) >> ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PKT_PERIOD_D_MSB_get", value );

    return value;
}
static INLINE void acb_ft_field_range_PKT_PERIOD_D_MSB_set( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_PKT_PERIOD_D_MSB_set( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_D_MSB_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_D_MSB_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_D_MSB_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000020 bits 7:0) field PKT_PERIOD_D_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG */
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
        /* (0x00000020 bits 7:0) field PKT_PERIOD_D_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG */
        acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_OFF + subfield_offset),
                                                                ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_D_MSB_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_D_MSB_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_D_MSB_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_D_MSB_get", stop_bit, 7 );
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
    /* (0x00000020 bits 7:0) field PKT_PERIOD_D_MSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_MSK)
                  >> ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_MSK, ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_MSB_REG_BIT_PKT_PERIOD_D_MSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_D_MSB_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_PKT_PERIOD_D_LSB_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PKT_PERIOD_D_LSB_set( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PKT_PERIOD_D_LSB_set", value );

    /* (0x00000024 bits 31:0) field PKT_PERIOD_D_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG */
    acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_MSK,
                                                            ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_OFF,
                                                            value);
}

static INLINE UINT32 acb_ft_field_PKT_PERIOD_D_LSB_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PKT_PERIOD_D_LSB_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000024 bits 31:0) field PKT_PERIOD_D_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_read( b_ptr,
                                                                   h_ptr);
    value = (reg_value & ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_MSK) >> ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PKT_PERIOD_D_LSB_get", value );

    return value;
}
static INLINE void acb_ft_field_range_PKT_PERIOD_D_LSB_set( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_PKT_PERIOD_D_LSB_set( acb_ft_buffer_t *b_ptr,
                                                            acb_ft_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_D_LSB_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_D_LSB_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_D_LSB_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000024 bits 31:0) field PKT_PERIOD_D_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG */
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
        /* (0x00000024 bits 31:0) field PKT_PERIOD_D_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG */
        acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_field_set( b_ptr,
                                                                h_ptr,
                                                                subfield_mask << (ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_OFF + subfield_offset),
                                                                ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_OFF + subfield_offset,
                                                                value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_D_LSB_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PKT_PERIOD_D_LSB_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_PERIOD_D_LSB_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_PERIOD_D_LSB_get", stop_bit, 31 );
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
    /* (0x00000024 bits 31:0) field PKT_PERIOD_D_LSB of register PMC_ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG */
    reg_value = acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_read( b_ptr,
                                                                   h_ptr);
    field_value = (reg_value & ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_MSK)
                  >> ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_MSK, ACB_FT_REG_PACKET_PERIOD_DENOMINATOR_LSB_REG_BIT_PKT_PERIOD_D_LSB_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_PERIOD_D_LSB_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_FB_WORDSIZE_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FB_WORDSIZE_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FB_WORDSIZE_set", value, 63);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FB_WORDSIZE_set", value );

    /* (0x00000028 bits 29:24) field FB_WORDSIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
    acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_MSK,
                                                     ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_OFF,
                                                     value);
}

static INLINE UINT32 acb_ft_field_FB_WORDSIZE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FB_WORDSIZE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 29:24) field FB_WORDSIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_MSK) >> ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FB_WORDSIZE_get", value );

    return value;
}
static INLINE void acb_ft_field_range_FB_WORDSIZE_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_FB_WORDSIZE_set( acb_ft_buffer_t *b_ptr,
                                                       acb_ft_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit,
                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FB_WORDSIZE_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FB_WORDSIZE_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FB_WORDSIZE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000028 bits 29:24) field FB_WORDSIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
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
        /* (0x00000028 bits 29:24) field FB_WORDSIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
        acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_OFF + subfield_offset),
                                                         ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_FB_WORDSIZE_get( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_FB_WORDSIZE_get( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FB_WORDSIZE_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FB_WORDSIZE_get", stop_bit, 5 );
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
    /* (0x00000028 bits 29:24) field FB_WORDSIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_MSK)
                  >> ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_MSK, ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_FB_WORDSIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FB_WORDSIZE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_PKT_SIZE_VAR_set( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PKT_SIZE_VAR_set( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PKT_SIZE_VAR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PKT_SIZE_VAR_set", value );

    /* (0x00000028 bits 16) field PKT_SIZE_VAR of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
    acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_MSK,
                                                     ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_OFF,
                                                     value);
}

static INLINE UINT32 acb_ft_field_PKT_SIZE_VAR_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PKT_SIZE_VAR_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 16) field PKT_SIZE_VAR of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_MSK) >> ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_VAR_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PKT_SIZE_VAR_get", value );

    return value;
}
static INLINE void acb_ft_field_PKT_SIZE_set( acb_ft_buffer_t *b_ptr,
                                              acb_ft_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_PKT_SIZE_set( acb_ft_buffer_t *b_ptr,
                                              acb_ft_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 511)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_PKT_SIZE_set", value, 511);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_PKT_SIZE_set", value );

    /* (0x00000028 bits 8:0) field PKT_SIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
    acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set( b_ptr,
                                                     h_ptr,
                                                     ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK,
                                                     ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF,
                                                     value);
}

static INLINE UINT32 acb_ft_field_PKT_SIZE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PKT_SIZE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 8:0) field PKT_SIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read( b_ptr,
                                                            h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK) >> ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PKT_SIZE_get", value );

    return value;
}
static INLINE void acb_ft_field_range_PKT_SIZE_set( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_PKT_SIZE_set( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_SIZE_set", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_SIZE_set", stop_bit, 8 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_SIZE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000028 bits 8:0) field PKT_SIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
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
        /* (0x00000028 bits 8:0) field PKT_SIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
        acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set( b_ptr,
                                                         h_ptr,
                                                         subfield_mask << (ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF + subfield_offset),
                                                         ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_PKT_SIZE_get( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PKT_SIZE_get( acb_ft_buffer_t *b_ptr,
                                                      acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PKT_SIZE_get", stop_bit, start_bit );
    if (stop_bit > 8) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PKT_SIZE_get", stop_bit, 8 );
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
    /* (0x00000028 bits 8:0) field PKT_SIZE of register PMC_ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read( b_ptr,
                                                            h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK)
                  >> ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_MSK, ACB_FT_REG_ACB_FT_PACKET_SIZE_CFG_REG_BIT_PKT_SIZE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PKT_SIZE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_THERMOMETER_INT_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_THERMOMETER_INT_set( acb_ft_buffer_t *b_ptr,
                                                     acb_ft_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_THERMOMETER_INT_set", value, 15);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_THERMOMETER_INT_set", value );

    /* (0x0000002c bits 3:0) field THERMOMETER_INT of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1 */
    acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_field_set( b_ptr,
                                                       h_ptr,
                                                       ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_MSK,
                                                       ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_OFF,
                                                       value);
}

static INLINE UINT32 acb_ft_field_THERMOMETER_INT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_THERMOMETER_INT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 3:0) field THERMOMETER_INT of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_MSK) >> ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_THERMOMETER_INT_get", value );

    return value;
}
static INLINE void acb_ft_field_range_THERMOMETER_INT_set( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_THERMOMETER_INT_set( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_THERMOMETER_INT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_THERMOMETER_INT_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_THERMOMETER_INT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000002c bits 3:0) field THERMOMETER_INT of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1 */
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
        /* (0x0000002c bits 3:0) field THERMOMETER_INT of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1 */
        acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_OFF + subfield_offset),
                                                           ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_THERMOMETER_INT_get( acb_ft_buffer_t *b_ptr,
                                                             acb_ft_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_THERMOMETER_INT_get( acb_ft_buffer_t *b_ptr,
                                                             acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_THERMOMETER_INT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_THERMOMETER_INT_get", stop_bit, 3 );
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
    /* (0x0000002c bits 3:0) field THERMOMETER_INT of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_MSK)
                  >> ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_MSK, ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_1_BIT_THERMOMETER_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_THERMOMETER_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_THERMOMETER_N_set( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_THERMOMETER_N_set( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "acb_ft_field_THERMOMETER_N_set", value );

    /* (0x00000030 bits 31:0) field THERMOMETER_N of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2 */
    acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_field_set( b_ptr,
                                                       h_ptr,
                                                       ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_MSK,
                                                       ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_OFF,
                                                       value);
}

static INLINE UINT32 acb_ft_field_THERMOMETER_N_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_THERMOMETER_N_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 31:0) field THERMOMETER_N of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_MSK) >> ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_THERMOMETER_N_get", value );

    return value;
}
static INLINE void acb_ft_field_range_THERMOMETER_N_set( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_THERMOMETER_N_set( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_THERMOMETER_N_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_THERMOMETER_N_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_THERMOMETER_N_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000030 bits 31:0) field THERMOMETER_N of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2 */
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
        /* (0x00000030 bits 31:0) field THERMOMETER_N of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2 */
        acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_OFF + subfield_offset),
                                                           ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_THERMOMETER_N_get( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_THERMOMETER_N_get( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_THERMOMETER_N_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_THERMOMETER_N_get", stop_bit, 31 );
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
    /* (0x00000030 bits 31:0) field THERMOMETER_N of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_MSK)
                  >> ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_MSK, ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_2_BIT_THERMOMETER_N_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_THERMOMETER_N_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_THERMOMETER_D_set( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_THERMOMETER_D_set( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "acb_ft_field_THERMOMETER_D_set", value );

    /* (0x00000034 bits 31:0) field THERMOMETER_D of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3 */
    acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_field_set( b_ptr,
                                                       h_ptr,
                                                       ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_MSK,
                                                       ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_OFF,
                                                       value);
}

static INLINE UINT32 acb_ft_field_THERMOMETER_D_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_THERMOMETER_D_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 31:0) field THERMOMETER_D of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3 */
    reg_value = acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_MSK) >> ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_THERMOMETER_D_get", value );

    return value;
}
static INLINE void acb_ft_field_range_THERMOMETER_D_set( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_THERMOMETER_D_set( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit,
                                                         UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_THERMOMETER_D_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_THERMOMETER_D_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_THERMOMETER_D_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000034 bits 31:0) field THERMOMETER_D of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3 */
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
        /* (0x00000034 bits 31:0) field THERMOMETER_D of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3 */
        acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_OFF + subfield_offset),
                                                           ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_THERMOMETER_D_get( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_THERMOMETER_D_get( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_THERMOMETER_D_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_THERMOMETER_D_get", stop_bit, 31 );
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
    /* (0x00000034 bits 31:0) field THERMOMETER_D of register PMC_ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3 */
    reg_value = acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_MSK)
                  >> ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_MSK, ACB_FT_REG_ACB_FT_THERMOMETER_CFG_REG_3_BIT_THERMOMETER_D_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_THERMOMETER_D_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_CNT_N_set( acb_ft_buffer_t *b_ptr,
                                           acb_ft_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_CNT_N_set( acb_ft_buffer_t *b_ptr,
                                           acb_ft_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "acb_ft_field_CNT_N_set", value );

    /* (0x00000038 bits 31:0) field CNT_N of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1 */
    acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_field_set( b_ptr,
                                                        h_ptr,
                                                        ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_MSK,
                                                        ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_OFF,
                                                        value);
}

static INLINE UINT32 acb_ft_field_CNT_N_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_CNT_N_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 31:0) field CNT_N of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_MSK) >> ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_CNT_N_get", value );

    return value;
}
static INLINE void acb_ft_field_range_CNT_N_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_CNT_N_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_CNT_N_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_CNT_N_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_CNT_N_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000038 bits 31:0) field CNT_N of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1 */
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
        /* (0x00000038 bits 31:0) field CNT_N of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1 */
        acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_OFF + subfield_offset),
                                                            ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_CNT_N_get( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_CNT_N_get( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_CNT_N_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_CNT_N_get", stop_bit, 31 );
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
    /* (0x00000038 bits 31:0) field CNT_N of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_MSK)
                  >> ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_MSK, ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_1_BIT_CNT_N_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_CNT_N_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_CNT_D_set( acb_ft_buffer_t *b_ptr,
                                           acb_ft_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_CNT_D_set( acb_ft_buffer_t *b_ptr,
                                           acb_ft_handle_t *h_ptr,
                                           UINT32 value )
{
    IOLOG( "%s <= 0x%08x", "acb_ft_field_CNT_D_set", value );

    /* (0x0000003c bits 31:0) field CNT_D of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2 */
    acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_field_set( b_ptr,
                                                        h_ptr,
                                                        ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_MSK,
                                                        ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_OFF,
                                                        value);
}

static INLINE UINT32 acb_ft_field_CNT_D_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_CNT_D_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 31:0) field CNT_D of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_read( b_ptr,
                                                               h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_MSK) >> ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_CNT_D_get", value );

    return value;
}
static INLINE void acb_ft_field_range_CNT_D_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_CNT_D_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit,
                                                 UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_CNT_D_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_CNT_D_set", stop_bit, 31 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_CNT_D_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000003c bits 31:0) field CNT_D of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2 */
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
        /* (0x0000003c bits 31:0) field CNT_D of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2 */
        acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_field_set( b_ptr,
                                                            h_ptr,
                                                            subfield_mask << (ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_OFF + subfield_offset),
                                                            ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_CNT_D_get( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_CNT_D_get( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_CNT_D_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_CNT_D_get", stop_bit, 31 );
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
    /* (0x0000003c bits 31:0) field CNT_D of register PMC_ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_read( b_ptr,
                                                               h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_MSK)
                  >> ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_MSK, ACB_FT_REG_ACB_FT_FEEDBACK_CNT_CFG_REG_2_BIT_CNT_D_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_CNT_D_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_OVERRIDE_FILT_set( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_OVERRIDE_FILT_set( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_OVERRIDE_FILT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_OVERRIDE_FILT_set", value );

    /* (0x00000044 bits 3) field OVERRIDE_FILT of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG */
    acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_OVERRIDE_FILT_MSK,
                                                    ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_OVERRIDE_FILT_OFF,
                                                    value);
}

static INLINE UINT32 acb_ft_field_OVERRIDE_FILT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_OVERRIDE_FILT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 3) field OVERRIDE_FILT of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_OVERRIDE_FILT_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_OVERRIDE_FILT_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_OVERRIDE_FILT_get", value );

    return value;
}
static INLINE void acb_ft_field_UPDATE_FILT_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_UPDATE_FILT_set( acb_ft_buffer_t *b_ptr,
                                                 acb_ft_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_UPDATE_FILT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_UPDATE_FILT_set", value );

    /* (0x00000044 bits 2) field UPDATE_FILT of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG */
    acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_FILT_MSK,
                                                    ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_FILT_OFF,
                                                    value);
}

static INLINE UINT32 acb_ft_field_UPDATE_FILT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_UPDATE_FILT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 2) field UPDATE_FILT of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_FILT_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_FILT_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_UPDATE_FILT_get", value );

    return value;
}
static INLINE void acb_ft_field_UPDATE_INT_set( acb_ft_buffer_t *b_ptr,
                                                acb_ft_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_UPDATE_INT_set( acb_ft_buffer_t *b_ptr,
                                                acb_ft_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_UPDATE_INT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_UPDATE_INT_set", value );

    /* (0x00000044 bits 1) field UPDATE_INT of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG */
    acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_INT_MSK,
                                                    ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_INT_OFF,
                                                    value);
}

static INLINE UINT32 acb_ft_field_UPDATE_INT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_UPDATE_INT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 1) field UPDATE_INT of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_INT_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_UPDATE_INT_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_UPDATE_INT_get", value );

    return value;
}
static INLINE void acb_ft_field_RESET_MINMAX_set( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_RESET_MINMAX_set( acb_ft_buffer_t *b_ptr,
                                                  acb_ft_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_RESET_MINMAX_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_RESET_MINMAX_set", value );

    /* (0x00000044 bits 0) field RESET_MINMAX of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG */
    acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_field_set( b_ptr,
                                                    h_ptr,
                                                    ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_RESET_MINMAX_MSK,
                                                    ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_RESET_MINMAX_OFF,
                                                    value);
}

static INLINE UINT32 acb_ft_field_RESET_MINMAX_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_RESET_MINMAX_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000044 bits 0) field RESET_MINMAX of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_read( b_ptr,
                                                           h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_RESET_MINMAX_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_CFG_REG_BIT_RESET_MINMAX_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_RESET_MINMAX_get", value );

    return value;
}
static INLINE void acb_ft_field_FILTER_INT_OVERRIDE_set( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FILTER_INT_OVERRIDE_set( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 33554431)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FILTER_INT_OVERRIDE_set", value, 33554431);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FILTER_INT_OVERRIDE_set", value );

    /* (0x0000005c bits 24:0) field FILTER_INT_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6 */
    acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_field_set( b_ptr,
                                                       h_ptr,
                                                       ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_MSK,
                                                       ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_OFF,
                                                       value);
}

static INLINE UINT32 acb_ft_field_FILTER_INT_OVERRIDE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FILTER_INT_OVERRIDE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000005c bits 24:0) field FILTER_INT_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FILTER_INT_OVERRIDE_get", value );

    return value;
}
static INLINE void acb_ft_field_range_FILTER_INT_OVERRIDE_set( acb_ft_buffer_t *b_ptr,
                                                               acb_ft_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_FILTER_INT_OVERRIDE_set( acb_ft_buffer_t *b_ptr,
                                                               acb_ft_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit,
                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FILTER_INT_OVERRIDE_set", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FILTER_INT_OVERRIDE_set", stop_bit, 24 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FILTER_INT_OVERRIDE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000005c bits 24:0) field FILTER_INT_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6 */
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
        if (stop_bit < 24) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 24;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000005c bits 24:0) field FILTER_INT_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6 */
        acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_OFF + subfield_offset),
                                                           ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_FILTER_INT_OVERRIDE_get( acb_ft_buffer_t *b_ptr,
                                                                 acb_ft_handle_t *h_ptr,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_FILTER_INT_OVERRIDE_get( acb_ft_buffer_t *b_ptr,
                                                                 acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FILTER_INT_OVERRIDE_get", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FILTER_INT_OVERRIDE_get", stop_bit, 24 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 24) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 24;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000005c bits 24:0) field FILTER_INT_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_MSK)
                  >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_MSK, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_6_BIT_FILTER_INT_OVERRIDE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FILTER_INT_OVERRIDE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void acb_ft_field_FILTER_FRAC_OVERRIDE_set( acb_ft_buffer_t *b_ptr,
                                                          acb_ft_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_FILTER_FRAC_OVERRIDE_set( acb_ft_buffer_t *b_ptr,
                                                          acb_ft_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_FILTER_FRAC_OVERRIDE_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_FILTER_FRAC_OVERRIDE_set", value );

    /* (0x00000060 bits 15:0) field FILTER_FRAC_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7 */
    acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_field_set( b_ptr,
                                                       h_ptr,
                                                       ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_MSK,
                                                       ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_OFF,
                                                       value);
}

static INLINE UINT32 acb_ft_field_FILTER_FRAC_OVERRIDE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FILTER_FRAC_OVERRIDE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000060 bits 15:0) field FILTER_FRAC_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FILTER_FRAC_OVERRIDE_get", value );

    return value;
}
static INLINE void acb_ft_field_range_FILTER_FRAC_OVERRIDE_set( acb_ft_buffer_t *b_ptr,
                                                                acb_ft_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_range_FILTER_FRAC_OVERRIDE_set( acb_ft_buffer_t *b_ptr,
                                                                acb_ft_handle_t *h_ptr,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FILTER_FRAC_OVERRIDE_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FILTER_FRAC_OVERRIDE_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FILTER_FRAC_OVERRIDE_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000060 bits 15:0) field FILTER_FRAC_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7 */
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
        /* (0x00000060 bits 15:0) field FILTER_FRAC_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7 */
        acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_field_set( b_ptr,
                                                           h_ptr,
                                                           subfield_mask << (ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_OFF + subfield_offset),
                                                           ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_OFF + subfield_offset,
                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 acb_ft_field_range_FILTER_FRAC_OVERRIDE_get( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_FILTER_FRAC_OVERRIDE_get( acb_ft_buffer_t *b_ptr,
                                                                  acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FILTER_FRAC_OVERRIDE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FILTER_FRAC_OVERRIDE_get", stop_bit, 15 );
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
    /* (0x00000060 bits 15:0) field FILTER_FRAC_OVERRIDE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_MSK)
                  >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_MSK, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_7_BIT_FILTER_FRAC_OVERRIDE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FILTER_FRAC_OVERRIDE_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_active_both
 * ==================================================================================
 */
static INLINE void acb_ft_field_ACB_FAIL_I_set_to_clear( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_ACB_FAIL_I_set_to_clear( acb_ft_buffer_t *b_ptr,
                                                         acb_ft_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_ACB_FAIL_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_ACB_FAIL_I_set_to_clear", value );

    /* (0x00000068 bits 0) field ACB_FAIL_I of register PMC_ACB_FT_REG_ACB_FT_INT_REG */
    acb_ft_reg_ACB_FT_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         ACB_FT_REG_ACB_FT_INT_REG_BIT_ACB_FAIL_I_MSK,
                                                         ACB_FT_REG_ACB_FT_INT_REG_BIT_ACB_FAIL_I_OFF,
                                                         value);
}

static INLINE UINT32 acb_ft_field_ACB_FAIL_I_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_ACB_FAIL_I_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000068 bits 0) field ACB_FAIL_I of register PMC_ACB_FT_REG_ACB_FT_INT_REG */
    reg_value = acb_ft_reg_ACB_FT_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_INT_REG_BIT_ACB_FAIL_I_MSK) >> ACB_FT_REG_ACB_FT_INT_REG_BIT_ACB_FAIL_I_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_ACB_FAIL_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE acb_ft_field_ACB_FAIL_I_poll( acb_ft_buffer_t *b_ptr,
                                                                 acb_ft_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE acb_ft_field_ACB_FAIL_I_poll( acb_ft_buffer_t *b_ptr,
                                                                 acb_ft_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "acb_ft_field_ACB_FAIL_I_poll", value );

    /* (0x00000068 bits 0) field ACB_FAIL_I of register PMC_ACB_FT_REG_ACB_FT_INT_REG */
    return acb_ft_reg_ACB_FT_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           ACB_FT_REG_ACB_FT_INT_REG_BIT_ACB_FAIL_I_MSK,
                                           (value<<ACB_FT_REG_ACB_FT_INT_REG_BIT_ACB_FAIL_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void acb_ft_field_ACB_FAIL_E_set( acb_ft_buffer_t *b_ptr,
                                                acb_ft_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_ACB_FAIL_E_set( acb_ft_buffer_t *b_ptr,
                                                acb_ft_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_ACB_FAIL_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_ACB_FAIL_E_set", value );

    /* (0x0000006c bits 0) field ACB_FAIL_E of register PMC_ACB_FT_REG_ACB_FT_INT_EN_REG */
    acb_ft_reg_ACB_FT_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            ACB_FT_REG_ACB_FT_INT_EN_REG_BIT_ACB_FAIL_E_MSK,
                                            ACB_FT_REG_ACB_FT_INT_EN_REG_BIT_ACB_FAIL_E_OFF,
                                            value);
}

static INLINE UINT32 acb_ft_field_ACB_FAIL_E_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_ACB_FAIL_E_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000006c bits 0) field ACB_FAIL_E of register PMC_ACB_FT_REG_ACB_FT_INT_EN_REG */
    reg_value = acb_ft_reg_ACB_FT_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_INT_EN_REG_BIT_ACB_FAIL_E_MSK) >> ACB_FT_REG_ACB_FT_INT_EN_REG_BIT_ACB_FAIL_E_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_ACB_FAIL_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value
 * ==================================================================================
 */
static INLINE UINT32 acb_ft_field_ACB_FAIL_V_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_ACB_FAIL_V_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000070 bits 0) field ACB_FAIL_V of register PMC_ACB_FT_REG_ACB_FT_INT_STAT_REG */
    reg_value = acb_ft_reg_ACB_FT_INT_STAT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_INT_STAT_REG_BIT_ACB_FAIL_V_MSK) >> ACB_FT_REG_ACB_FT_INT_STAT_REG_BIT_ACB_FAIL_V_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_ACB_FAIL_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE acb_ft_field_ACB_FAIL_V_poll( acb_ft_buffer_t *b_ptr,
                                                                 acb_ft_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE acb_ft_field_ACB_FAIL_V_poll( acb_ft_buffer_t *b_ptr,
                                                                 acb_ft_handle_t *h_ptr,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "acb_ft_field_ACB_FAIL_V_poll", value );

    /* (0x00000070 bits 0) field ACB_FAIL_V of register PMC_ACB_FT_REG_ACB_FT_INT_STAT_REG */
    return acb_ft_reg_ACB_FT_INT_STAT_REG_poll( b_ptr,
                                                h_ptr,
                                                ACB_FT_REG_ACB_FT_INT_STAT_REG_BIT_ACB_FAIL_V_MSK,
                                                (value<<ACB_FT_REG_ACB_FT_INT_STAT_REG_BIT_ACB_FAIL_V_OFF),
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
static INLINE UINT32 acb_ft_field_PLL_S_OUT_MIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PLL_S_OUT_MIN_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000048 bits 24:0) field PLL_S_OUT_MIN of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_1_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_BIT_PLL_S_OUT_MIN_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_BIT_PLL_S_OUT_MIN_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PLL_S_OUT_MIN_get", value );

    return value;
}
static INLINE UINT32 acb_ft_field_range_PLL_S_OUT_MIN_get( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PLL_S_OUT_MIN_get( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PLL_S_OUT_MIN_get", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PLL_S_OUT_MIN_get", stop_bit, 24 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 24) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 24;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000048 bits 24:0) field PLL_S_OUT_MIN of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_1_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_BIT_PLL_S_OUT_MIN_MSK)
                  >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_BIT_PLL_S_OUT_MIN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_BIT_PLL_S_OUT_MIN_MSK, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_1_BIT_PLL_S_OUT_MIN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PLL_S_OUT_MIN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 acb_ft_field_PLL_S_OUT_MAX_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_PLL_S_OUT_MAX_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000004c bits 24:0) field PLL_S_OUT_MAX of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_2_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_BIT_PLL_S_OUT_MAX_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_BIT_PLL_S_OUT_MAX_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_PLL_S_OUT_MAX_get", value );

    return value;
}
static INLINE UINT32 acb_ft_field_range_PLL_S_OUT_MAX_get( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_PLL_S_OUT_MAX_get( acb_ft_buffer_t *b_ptr,
                                                           acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_PLL_S_OUT_MAX_get", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_PLL_S_OUT_MAX_get", stop_bit, 24 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 24) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 24;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000004c bits 24:0) field PLL_S_OUT_MAX of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_2_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_BIT_PLL_S_OUT_MAX_MSK)
                  >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_BIT_PLL_S_OUT_MAX_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_BIT_PLL_S_OUT_MAX_MSK, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_2_BIT_PLL_S_OUT_MAX_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_PLL_S_OUT_MAX_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 acb_ft_field_INTEGRATOR_VALUE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_INTEGRATOR_VALUE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000050 bits 24:0) field INTEGRATOR_VALUE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_3_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_BIT_INTEGRATOR_VALUE_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_BIT_INTEGRATOR_VALUE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_INTEGRATOR_VALUE_get", value );

    return value;
}
static INLINE UINT32 acb_ft_field_range_INTEGRATOR_VALUE_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_INTEGRATOR_VALUE_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_INTEGRATOR_VALUE_get", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_INTEGRATOR_VALUE_get", stop_bit, 24 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 24) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 24;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000050 bits 24:0) field INTEGRATOR_VALUE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_3_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_BIT_INTEGRATOR_VALUE_MSK)
                  >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_BIT_INTEGRATOR_VALUE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_BIT_INTEGRATOR_VALUE_MSK, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_3_BIT_INTEGRATOR_VALUE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_INTEGRATOR_VALUE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 acb_ft_field_FILTER_INT_VALUE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FILTER_INT_VALUE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000054 bits 24:0) field FILTER_INT_VALUE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_4_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_BIT_FILTER_INT_VALUE_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_BIT_FILTER_INT_VALUE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FILTER_INT_VALUE_get", value );

    return value;
}
static INLINE UINT32 acb_ft_field_range_FILTER_INT_VALUE_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_FILTER_INT_VALUE_get( acb_ft_buffer_t *b_ptr,
                                                              acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FILTER_INT_VALUE_get", stop_bit, start_bit );
    if (stop_bit > 24) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FILTER_INT_VALUE_get", stop_bit, 24 );
    if (start_bit > 0) {
        subfield_offset = start_bit - 0;
        subfield_shift = 0;
        subfield_start_bit = start_bit;
    } else {
        subfield_offset = 0;
        subfield_shift = 0 - start_bit;
        subfield_start_bit = 0;
    }
    if (stop_bit < 24) {
        subfield_stop_bit = stop_bit;
    } else {
        subfield_stop_bit = 24;
    }
    subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
    subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00000054 bits 24:0) field FILTER_INT_VALUE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_4_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_BIT_FILTER_INT_VALUE_MSK)
                  >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_BIT_FILTER_INT_VALUE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_BIT_FILTER_INT_VALUE_MSK, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_4_BIT_FILTER_INT_VALUE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FILTER_INT_VALUE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 acb_ft_field_FILTER_FRAC_VALUE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_FILTER_FRAC_VALUE_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000058 bits 15:0) field FILTER_FRAC_VALUE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_5_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_BIT_FILTER_FRAC_VALUE_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_BIT_FILTER_FRAC_VALUE_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_FILTER_FRAC_VALUE_get", value );

    return value;
}
static INLINE UINT32 acb_ft_field_range_FILTER_FRAC_VALUE_get( acb_ft_buffer_t *b_ptr,
                                                               acb_ft_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_FILTER_FRAC_VALUE_get( acb_ft_buffer_t *b_ptr,
                                                               acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_FILTER_FRAC_VALUE_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_FILTER_FRAC_VALUE_get", stop_bit, 15 );
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
    /* (0x00000058 bits 15:0) field FILTER_FRAC_VALUE of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_5_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_BIT_FILTER_FRAC_VALUE_MSK)
                  >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_BIT_FILTER_FRAC_VALUE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_BIT_FILTER_FRAC_VALUE_MSK, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_5_BIT_FILTER_FRAC_VALUE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_FILTER_FRAC_VALUE_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 acb_ft_field_B_CNT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_B_CNT_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000064 bits 31:0) field B_CNT of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_8_read( b_ptr,
                                                              h_ptr);
    value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_BIT_B_CNT_MSK) >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_BIT_B_CNT_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_B_CNT_get", value );

    return value;
}
static INLINE UINT32 acb_ft_field_range_B_CNT_get( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_range_B_CNT_get( acb_ft_buffer_t *b_ptr,
                                                   acb_ft_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "acb_ft_field_range_B_CNT_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "acb_ft_field_range_B_CNT_get", stop_bit, 31 );
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
    /* (0x00000064 bits 31:0) field B_CNT of register PMC_ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8 */
    reg_value = acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_8_read( b_ptr,
                                                              h_ptr);
    field_value = (reg_value & ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_BIT_B_CNT_MSK)
                  >> ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_BIT_B_CNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_BIT_B_CNT_MSK, ACB_FT_REG_ACB_FT_OUTPUT_MON_STAT_REG_8_BIT_B_CNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "acb_ft_field_range_B_CNT_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset sw_reset
 * ==================================================================================
 */
static INLINE void acb_ft_field_SOFTWARE_RESET_set( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void acb_ft_field_SOFTWARE_RESET_set( acb_ft_buffer_t *b_ptr,
                                                    acb_ft_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "acb_ft_field_SOFTWARE_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "acb_ft_field_SOFTWARE_RESET_set", value );

    /* (0x00000000 bits 0) field SOFTWARE_RESET of register PMC_ACB_FT_REG_SW_RESET_REG */
    acb_ft_reg_SW_RESET_REG_field_set( b_ptr,
                                       h_ptr,
                                       ACB_FT_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_MSK,
                                       ACB_FT_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_OFF,
                                       value);
}

static INLINE UINT32 acb_ft_field_SOFTWARE_RESET_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 acb_ft_field_SOFTWARE_RESET_get( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field SOFTWARE_RESET of register PMC_ACB_FT_REG_SW_RESET_REG */
    reg_value = acb_ft_reg_SW_RESET_REG_read(  b_ptr, h_ptr);
    value = (reg_value & ACB_FT_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_MSK) >> ACB_FT_REG_SW_RESET_REG_BIT_SOFTWARE_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "acb_ft_field_SOFTWARE_RESET_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ACB_FT_IO_INLINE_H */
