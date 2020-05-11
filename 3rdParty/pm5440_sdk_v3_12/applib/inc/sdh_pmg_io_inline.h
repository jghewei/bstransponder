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
 *     and register accessor functions for the sdh_pmg block
 *****************************************************************************/
#ifndef _SDH_PMG_IO_INLINE_H
#define _SDH_PMG_IO_INLINE_H

#include "sdh_pmg_api.h"
#include "sdh_pmg_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SDH_PMG_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for sdh_pmg
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
    sdh_pmg_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} sdh_pmg_buffer_t;
static INLINE void sdh_pmg_buffer_init( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void sdh_pmg_buffer_init( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "sdh_pmg_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void sdh_pmg_buffer_flush( sdh_pmg_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void sdh_pmg_buffer_flush( sdh_pmg_buffer_t *b_ptr )
{
    IOLOG( "sdh_pmg_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 sdh_pmg_reg_read( sdh_pmg_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_reg_read( sdh_pmg_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
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
static INLINE void sdh_pmg_reg_write( sdh_pmg_buffer_t *b_ptr,
                                      sdh_pmg_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_write( sdh_pmg_buffer_t *b_ptr,
                                      sdh_pmg_handle_t *h_ptr,
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

static INLINE void sdh_pmg_field_set( sdh_pmg_buffer_t *b_ptr,
                                      sdh_pmg_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_set( sdh_pmg_buffer_t *b_ptr,
                                      sdh_pmg_handle_t *h_ptr,
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

static INLINE void sdh_pmg_action_on_write_field_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_action_on_write_field_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
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

static INLINE void sdh_pmg_burst_read( sdh_pmg_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_burst_read( sdh_pmg_buffer_t *b_ptr,
                                       sdh_pmg_handle_t *h_ptr,
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

static INLINE void sdh_pmg_burst_write( sdh_pmg_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_burst_write( sdh_pmg_buffer_t *b_ptr,
                                        sdh_pmg_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE sdh_pmg_poll( sdh_pmg_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sdh_pmg_poll( sdh_pmg_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
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
 *  register access functions for sdh_pmg
 * ==================================================================================
 */

static INLINE void sdh_pmg_reg_TOP_CFG_REG_write( sdh_pmg_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_TOP_CFG_REG_write( sdh_pmg_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sdh_pmg_reg_TOP_CFG_REG_write", value );
    sdh_pmg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_TOP_CFG_REG,
                       value);
}

static INLINE void sdh_pmg_reg_TOP_CFG_REG_field_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_TOP_CFG_REG_field_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sdh_pmg_reg_TOP_CFG_REG_field_set", mask, ofs, value );
    sdh_pmg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_TOP_CFG_REG,
                       mask,
                       PMC_SDH_PMG_REG_TOP_CFG_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 sdh_pmg_reg_TOP_CFG_REG_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_reg_TOP_CFG_REG_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sdh_pmg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_SDH_PMG_REG_TOP_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "sdh_pmg_reg_TOP_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void sdh_pmg_reg_TOP_SW_RESET_write( sdh_pmg_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_TOP_SW_RESET_write( sdh_pmg_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sdh_pmg_reg_TOP_SW_RESET_write", value );
    sdh_pmg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_TOP_SW_RESET,
                       value);
}

static INLINE void sdh_pmg_reg_TOP_SW_RESET_field_set( sdh_pmg_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_TOP_SW_RESET_field_set( sdh_pmg_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sdh_pmg_reg_TOP_SW_RESET_field_set", mask, ofs, value );
    sdh_pmg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_TOP_SW_RESET,
                       mask,
                       PMC_SDH_PMG_REG_TOP_SW_RESET_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 sdh_pmg_reg_TOP_SW_RESET_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_reg_TOP_SW_RESET_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sdh_pmg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_SDH_PMG_REG_TOP_SW_RESET);

    IOLOG( "%s -> 0x%08x;", "sdh_pmg_reg_TOP_SW_RESET_read", reg_value);
    return reg_value;
}

static INLINE void sdh_pmg_reg_MTSB_INT_EN_write( sdh_pmg_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_MTSB_INT_EN_write( sdh_pmg_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sdh_pmg_reg_MTSB_INT_EN_write", value );
    sdh_pmg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_MTSB_INT_EN,
                       value);
}

static INLINE void sdh_pmg_reg_MTSB_INT_EN_field_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_MTSB_INT_EN_field_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "sdh_pmg_reg_MTSB_INT_EN_field_set", mask, ofs, value );
    sdh_pmg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_MTSB_INT_EN,
                       mask,
                       PMC_SDH_PMG_REG_MTSB_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 sdh_pmg_reg_MTSB_INT_EN_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_reg_MTSB_INT_EN_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sdh_pmg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_SDH_PMG_REG_MTSB_INT_EN);

    IOLOG( "%s -> 0x%08x;", "sdh_pmg_reg_MTSB_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_write( sdh_pmg_buffer_t *b_ptr,
                                                                       sdh_pmg_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_write( sdh_pmg_buffer_t *b_ptr,
                                                                       sdh_pmg_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_write", value );
    sdh_pmg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET(N),
                       value);
}

static INLINE void sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_field_set( sdh_pmg_buffer_t *b_ptr,
                                                                           sdh_pmg_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_field_set( sdh_pmg_buffer_t *b_ptr,
                                                                           sdh_pmg_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_field_set", N, mask, ofs, value );
    sdh_pmg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET(N),
                       mask,
                       PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_read( sdh_pmg_buffer_t *b_ptr,
                                                                        sdh_pmg_handle_t *h_ptr,
                                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_read( sdh_pmg_buffer_t *b_ptr,
                                                                        sdh_pmg_handle_t *h_ptr,
                                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = sdh_pmg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_read", reg_value, N);
    return reg_value;
}

static INLINE void sdh_pmg_reg_CHANNEL_CFG_REG_array_write( sdh_pmg_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_CHANNEL_CFG_REG_array_write( sdh_pmg_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "sdh_pmg_reg_CHANNEL_CFG_REG_array_write", value );
    sdh_pmg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_CHANNEL_CFG_REG(N),
                       value);
}

static INLINE void sdh_pmg_reg_CHANNEL_CFG_REG_array_field_set( sdh_pmg_buffer_t *b_ptr,
                                                                sdh_pmg_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_reg_CHANNEL_CFG_REG_array_field_set( sdh_pmg_buffer_t *b_ptr,
                                                                sdh_pmg_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "sdh_pmg_reg_CHANNEL_CFG_REG_array_field_set", N, mask, ofs, value );
    sdh_pmg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_SDH_PMG_REG_CHANNEL_CFG_REG(N),
                       mask,
                       PMC_SDH_PMG_REG_CHANNEL_CFG_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 sdh_pmg_reg_CHANNEL_CFG_REG_array_read( sdh_pmg_buffer_t *b_ptr,
                                                             sdh_pmg_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_reg_CHANNEL_CFG_REG_array_read( sdh_pmg_buffer_t *b_ptr,
                                                             sdh_pmg_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = sdh_pmg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_SDH_PMG_REG_CHANNEL_CFG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "sdh_pmg_reg_CHANNEL_CFG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 sdh_pmg_reg_INT_SUMMARY_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_reg_INT_SUMMARY_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sdh_pmg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_SDH_PMG_REG_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "sdh_pmg_reg_INT_SUMMARY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 sdh_pmg_reg_TIP_SUMMARY_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_reg_TIP_SUMMARY_read( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = sdh_pmg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_SDH_PMG_REG_TIP_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "sdh_pmg_reg_TIP_SUMMARY_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE sdh_pmg_reg_TIP_SUMMARY_poll( sdh_pmg_buffer_t *b_ptr,
                                                                 sdh_pmg_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sdh_pmg_reg_TIP_SUMMARY_poll( sdh_pmg_buffer_t *b_ptr,
                                                                 sdh_pmg_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "sdh_pmg_reg_TIP_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return sdh_pmg_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_SDH_PMG_REG_TIP_SUMMARY,
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
static INLINE void sdh_pmg_field_LBIP_BER_SEL_set( sdh_pmg_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_LBIP_BER_SEL_set( sdh_pmg_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_LBIP_BER_SEL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sdh_pmg_field_LBIP_BER_SEL_set", value );

    /* (0x00000000 bits 9) field LBIP_BER_SEL of register PMC_SDH_PMG_REG_TOP_CFG_REG */
    sdh_pmg_reg_TOP_CFG_REG_field_set( b_ptr,
                                       h_ptr,
                                       SDH_PMG_REG_TOP_CFG_REG_BIT_LBIP_BER_SEL_MSK,
                                       SDH_PMG_REG_TOP_CFG_REG_BIT_LBIP_BER_SEL_OFF,
                                       value);
}

static INLINE UINT32 sdh_pmg_field_LBIP_BER_SEL_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_LBIP_BER_SEL_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 9) field LBIP_BER_SEL of register PMC_SDH_PMG_REG_TOP_CFG_REG */
    reg_value = sdh_pmg_reg_TOP_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_TOP_CFG_REG_BIT_LBIP_BER_SEL_MSK) >> SDH_PMG_REG_TOP_CFG_REG_BIT_LBIP_BER_SEL_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_LBIP_BER_SEL_get", value );

    return value;
}
static INLINE void sdh_pmg_field_OC_768_MODE_set( sdh_pmg_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_OC_768_MODE_set( sdh_pmg_buffer_t *b_ptr,
                                                  sdh_pmg_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_OC_768_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sdh_pmg_field_OC_768_MODE_set", value );

    /* (0x00000000 bits 0) field OC_768_MODE of register PMC_SDH_PMG_REG_TOP_CFG_REG */
    sdh_pmg_reg_TOP_CFG_REG_field_set( b_ptr,
                                       h_ptr,
                                       SDH_PMG_REG_TOP_CFG_REG_BIT_OC_768_MODE_MSK,
                                       SDH_PMG_REG_TOP_CFG_REG_BIT_OC_768_MODE_OFF,
                                       value);
}

static INLINE UINT32 sdh_pmg_field_OC_768_MODE_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_OC_768_MODE_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field OC_768_MODE of register PMC_SDH_PMG_REG_TOP_CFG_REG */
    reg_value = sdh_pmg_reg_TOP_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_TOP_CFG_REG_BIT_OC_768_MODE_MSK) >> SDH_PMG_REG_TOP_CFG_REG_BIT_OC_768_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_OC_768_MODE_get", value );

    return value;
}
static INLINE void sdh_pmg_field_TOP_SW_RST_set( sdh_pmg_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_TOP_SW_RST_set( sdh_pmg_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_TOP_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sdh_pmg_field_TOP_SW_RST_set", value );

    /* (0x00000004 bits 0) field TOP_SW_RST of register PMC_SDH_PMG_REG_TOP_SW_RESET */
    sdh_pmg_reg_TOP_SW_RESET_field_set( b_ptr,
                                        h_ptr,
                                        SDH_PMG_REG_TOP_SW_RESET_BIT_TOP_SW_RST_MSK,
                                        SDH_PMG_REG_TOP_SW_RESET_BIT_TOP_SW_RST_OFF,
                                        value);
}

static INLINE UINT32 sdh_pmg_field_TOP_SW_RST_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_TOP_SW_RST_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000004 bits 0) field TOP_SW_RST of register PMC_SDH_PMG_REG_TOP_SW_RESET */
    reg_value = sdh_pmg_reg_TOP_SW_RESET_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_TOP_SW_RESET_BIT_TOP_SW_RST_MSK) >> SDH_PMG_REG_TOP_SW_RESET_BIT_TOP_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_TOP_SW_RST_get", value );

    return value;
}
static INLINE void sdh_pmg_field_SBER_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_SBER_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_SBER_U_INT_EN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sdh_pmg_field_SBER_U_INT_EN_set", value );

    /* (0x00000028 bits 11:8) field SBER_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    sdh_pmg_reg_MTSB_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_MSK,
                                       SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_OFF,
                                       value);
}

static INLINE UINT32 sdh_pmg_field_SBER_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_SBER_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 11:8) field SBER_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    reg_value = sdh_pmg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_MSK) >> SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_SBER_U_INT_EN_get", value );

    return value;
}
static INLINE void sdh_pmg_field_range_SBER_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_range_SBER_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_SBER_U_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_SBER_U_INT_EN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_SBER_U_INT_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000028 bits 11:8) field SBER_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
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
        /* (0x00000028 bits 11:8) field SBER_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
        sdh_pmg_reg_MTSB_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_OFF + subfield_offset),
                                           SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 sdh_pmg_field_range_SBER_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_range_SBER_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_SBER_U_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_SBER_U_INT_EN_get", stop_bit, 3 );
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
    /* (0x00000028 bits 11:8) field SBER_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    reg_value = sdh_pmg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_MSK)
                  >> SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_MSK, SDH_PMG_REG_MTSB_INT_EN_BIT_SBER_U_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_SBER_U_INT_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sdh_pmg_field_RTTP_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_RTTP_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_RTTP_U_INT_EN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sdh_pmg_field_RTTP_U_INT_EN_set", value );

    /* (0x00000028 bits 7:4) field RTTP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    sdh_pmg_reg_MTSB_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_MSK,
                                       SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_OFF,
                                       value);
}

static INLINE UINT32 sdh_pmg_field_RTTP_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_RTTP_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 7:4) field RTTP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    reg_value = sdh_pmg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_MSK) >> SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_RTTP_U_INT_EN_get", value );

    return value;
}
static INLINE void sdh_pmg_field_range_RTTP_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_range_RTTP_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_RTTP_U_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_RTTP_U_INT_EN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_RTTP_U_INT_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000028 bits 7:4) field RTTP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
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
        /* (0x00000028 bits 7:4) field RTTP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
        sdh_pmg_reg_MTSB_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_OFF + subfield_offset),
                                           SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 sdh_pmg_field_range_RTTP_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_range_RTTP_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_RTTP_U_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_RTTP_U_INT_EN_get", stop_bit, 3 );
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
    /* (0x00000028 bits 7:4) field RTTP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    reg_value = sdh_pmg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_MSK)
                  >> SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_MSK, SDH_PMG_REG_MTSB_INT_EN_BIT_RTTP_U_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_RTTP_U_INT_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void sdh_pmg_field_RRMP_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_RRMP_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                    sdh_pmg_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_RRMP_U_INT_EN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "sdh_pmg_field_RRMP_U_INT_EN_set", value );

    /* (0x00000028 bits 3:0) field RRMP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    sdh_pmg_reg_MTSB_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_MSK,
                                       SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_OFF,
                                       value);
}

static INLINE UINT32 sdh_pmg_field_RRMP_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_RRMP_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000028 bits 3:0) field RRMP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    reg_value = sdh_pmg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_MSK) >> SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_RRMP_U_INT_EN_get", value );

    return value;
}
static INLINE void sdh_pmg_field_range_RRMP_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_range_RRMP_U_INT_EN_set( sdh_pmg_buffer_t *b_ptr,
                                                          sdh_pmg_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_RRMP_U_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_RRMP_U_INT_EN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_RRMP_U_INT_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000028 bits 3:0) field RRMP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
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
        /* (0x00000028 bits 3:0) field RRMP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
        sdh_pmg_reg_MTSB_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_OFF + subfield_offset),
                                           SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 sdh_pmg_field_range_RRMP_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_range_RRMP_U_INT_EN_get( sdh_pmg_buffer_t *b_ptr,
                                                            sdh_pmg_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_RRMP_U_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_RRMP_U_INT_EN_get", stop_bit, 3 );
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
    /* (0x00000028 bits 3:0) field RRMP_U_INT_EN of register PMC_SDH_PMG_REG_MTSB_INT_EN */
    reg_value = sdh_pmg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_MSK)
                  >> SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_MSK, SDH_PMG_REG_MTSB_INT_EN_BIT_RRMP_U_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_RRMP_U_INT_EN_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size4
 * ==================================================================================
 */
static INLINE void sdh_pmg_field_CH_SBER_U_SW_RST_set( sdh_pmg_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_CH_SBER_U_SW_RST_set( sdh_pmg_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CH_SBER_U_SW_RST_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_CH_SBER_U_SW_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sdh_pmg_field_CH_SBER_U_SW_RST_set", N, value );

    /* ((0x00000008 + (N) * 0x4) bits 10) field CH_SBER_U_SW_RST of register PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET index N=0..3 */
    sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_SBER_U_SW_RST_MSK,
                                                            SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_SBER_U_SW_RST_OFF,
                                                            value);
}

static INLINE UINT32 sdh_pmg_field_CH_SBER_U_SW_RST_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_CH_SBER_U_SW_RST_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CH_SBER_U_SW_RST_get", N, 3);
    /* ((0x00000008 + (N) * 0x4) bits 10) field CH_SBER_U_SW_RST of register PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET index N=0..3 */
    reg_value = sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_SBER_U_SW_RST_MSK) >> SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_SBER_U_SW_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sdh_pmg_field_CH_SBER_U_SW_RST_get", N, value );

    return value;
}
static INLINE void sdh_pmg_field_CH_RTTP_U_SW_RST_set( sdh_pmg_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_CH_RTTP_U_SW_RST_set( sdh_pmg_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CH_RTTP_U_SW_RST_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_CH_RTTP_U_SW_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sdh_pmg_field_CH_RTTP_U_SW_RST_set", N, value );

    /* ((0x00000008 + (N) * 0x4) bits 9) field CH_RTTP_U_SW_RST of register PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET index N=0..3 */
    sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RTTP_U_SW_RST_MSK,
                                                            SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RTTP_U_SW_RST_OFF,
                                                            value);
}

static INLINE UINT32 sdh_pmg_field_CH_RTTP_U_SW_RST_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_CH_RTTP_U_SW_RST_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CH_RTTP_U_SW_RST_get", N, 3);
    /* ((0x00000008 + (N) * 0x4) bits 9) field CH_RTTP_U_SW_RST of register PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET index N=0..3 */
    reg_value = sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RTTP_U_SW_RST_MSK) >> SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RTTP_U_SW_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sdh_pmg_field_CH_RTTP_U_SW_RST_get", N, value );

    return value;
}
static INLINE void sdh_pmg_field_CH_RRMP_U_SW_RST_set( sdh_pmg_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_CH_RRMP_U_SW_RST_set( sdh_pmg_buffer_t *b_ptr,
                                                       sdh_pmg_handle_t *h_ptr,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CH_RRMP_U_SW_RST_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_CH_RRMP_U_SW_RST_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sdh_pmg_field_CH_RRMP_U_SW_RST_set", N, value );

    /* ((0x00000008 + (N) * 0x4) bits 8) field CH_RRMP_U_SW_RST of register PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET index N=0..3 */
    sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RRMP_U_SW_RST_MSK,
                                                            SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RRMP_U_SW_RST_OFF,
                                                            value);
}

static INLINE UINT32 sdh_pmg_field_CH_RRMP_U_SW_RST_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_CH_RRMP_U_SW_RST_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CH_RRMP_U_SW_RST_get", N, 3);
    /* ((0x00000008 + (N) * 0x4) bits 8) field CH_RRMP_U_SW_RST of register PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET index N=0..3 */
    reg_value = sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RRMP_U_SW_RST_MSK) >> SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_RRMP_U_SW_RST_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sdh_pmg_field_CH_RRMP_U_SW_RST_get", N, value );

    return value;
}
static INLINE void sdh_pmg_field_CH_LOW_PWR_set( sdh_pmg_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_CH_LOW_PWR_set( sdh_pmg_buffer_t *b_ptr,
                                                 sdh_pmg_handle_t *h_ptr,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CH_LOW_PWR_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_CH_LOW_PWR_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sdh_pmg_field_CH_LOW_PWR_set", N, value );

    /* ((0x00000008 + (N) * 0x4) bits 0) field CH_LOW_PWR of register PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET index N=0..3 */
    sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_LOW_PWR_MSK,
                                                            SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_LOW_PWR_OFF,
                                                            value);
}

static INLINE UINT32 sdh_pmg_field_CH_LOW_PWR_get( sdh_pmg_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_CH_LOW_PWR_get( sdh_pmg_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CH_LOW_PWR_get", N, 3);
    /* ((0x00000008 + (N) * 0x4) bits 0) field CH_LOW_PWR of register PMC_SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET index N=0..3 */
    reg_value = sdh_pmg_reg_CHANNEL_LOW_POWER_SW_RESET_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_LOW_PWR_MSK) >> SDH_PMG_REG_CHANNEL_LOW_POWER_SW_RESET_BIT_CH_LOW_PWR_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sdh_pmg_field_CH_LOW_PWR_get", N, value );

    return value;
}
static INLINE void sdh_pmg_field_LOF_PN11_INSERT_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_LOF_PN11_INSERT_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_LOF_PN11_INSERT_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_LOF_PN11_INSERT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sdh_pmg_field_LOF_PN11_INSERT_set", N, value );

    /* ((0x00000018 + (N) * 0x4) bits 9) field LOF_PN11_INSERT of register PMC_SDH_PMG_REG_CHANNEL_CFG_REG index N=0..3 */
    sdh_pmg_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOF_PN11_INSERT_MSK,
                                                 SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOF_PN11_INSERT_OFF,
                                                 value);
}

static INLINE UINT32 sdh_pmg_field_LOF_PN11_INSERT_get( sdh_pmg_buffer_t *b_ptr,
                                                        sdh_pmg_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_LOF_PN11_INSERT_get( sdh_pmg_buffer_t *b_ptr,
                                                        sdh_pmg_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_LOF_PN11_INSERT_get", N, 3);
    /* ((0x00000018 + (N) * 0x4) bits 9) field LOF_PN11_INSERT of register PMC_SDH_PMG_REG_CHANNEL_CFG_REG index N=0..3 */
    reg_value = sdh_pmg_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOF_PN11_INSERT_MSK) >> SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOF_PN11_INSERT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sdh_pmg_field_LOF_PN11_INSERT_get", N, value );

    return value;
}
static INLINE void sdh_pmg_field_LOS_PN11_INSERT_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_LOS_PN11_INSERT_set( sdh_pmg_buffer_t *b_ptr,
                                                      sdh_pmg_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_LOS_PN11_INSERT_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_LOS_PN11_INSERT_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sdh_pmg_field_LOS_PN11_INSERT_set", N, value );

    /* ((0x00000018 + (N) * 0x4) bits 8) field LOS_PN11_INSERT of register PMC_SDH_PMG_REG_CHANNEL_CFG_REG index N=0..3 */
    sdh_pmg_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOS_PN11_INSERT_MSK,
                                                 SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOS_PN11_INSERT_OFF,
                                                 value);
}

static INLINE UINT32 sdh_pmg_field_LOS_PN11_INSERT_get( sdh_pmg_buffer_t *b_ptr,
                                                        sdh_pmg_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_LOS_PN11_INSERT_get( sdh_pmg_buffer_t *b_ptr,
                                                        sdh_pmg_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_LOS_PN11_INSERT_get", N, 3);
    /* ((0x00000018 + (N) * 0x4) bits 8) field LOS_PN11_INSERT of register PMC_SDH_PMG_REG_CHANNEL_CFG_REG index N=0..3 */
    reg_value = sdh_pmg_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOS_PN11_INSERT_MSK) >> SDH_PMG_REG_CHANNEL_CFG_REG_BIT_LOS_PN11_INSERT_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sdh_pmg_field_LOS_PN11_INSERT_get", N, value );

    return value;
}
static INLINE void sdh_pmg_field_CHNL_EN_set( sdh_pmg_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_CHNL_EN_set( sdh_pmg_buffer_t *b_ptr,
                                              sdh_pmg_handle_t *h_ptr,
                                              UINT32  N,
                                              UINT32 value )
{
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CHNL_EN_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_CHNL_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "sdh_pmg_field_CHNL_EN_set", N, value );

    /* ((0x00000018 + (N) * 0x4) bits 0) field CHNL_EN of register PMC_SDH_PMG_REG_CHANNEL_CFG_REG index N=0..3 */
    sdh_pmg_reg_CHANNEL_CFG_REG_array_field_set( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 SDH_PMG_REG_CHANNEL_CFG_REG_BIT_CHNL_EN_MSK,
                                                 SDH_PMG_REG_CHANNEL_CFG_REG_BIT_CHNL_EN_OFF,
                                                 value);
}

static INLINE UINT32 sdh_pmg_field_CHNL_EN_get( sdh_pmg_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_CHNL_EN_get( sdh_pmg_buffer_t *b_ptr,
                                                sdh_pmg_handle_t *h_ptr,
                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "sdh_pmg_field_CHNL_EN_get", N, 3);
    /* ((0x00000018 + (N) * 0x4) bits 0) field CHNL_EN of register PMC_SDH_PMG_REG_CHANNEL_CFG_REG index N=0..3 */
    reg_value = sdh_pmg_reg_CHANNEL_CFG_REG_array_read( b_ptr,
                                                        h_ptr,
                                                        N);
    value = (reg_value & SDH_PMG_REG_CHANNEL_CFG_REG_BIT_CHNL_EN_MSK) >> SDH_PMG_REG_CHANNEL_CFG_REG_BIT_CHNL_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "sdh_pmg_field_CHNL_EN_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void sdh_pmg_field_RRMP_U_ASCLK_set( sdh_pmg_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void sdh_pmg_field_RRMP_U_ASCLK_set( sdh_pmg_buffer_t *b_ptr,
                                                   sdh_pmg_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "sdh_pmg_field_RRMP_U_ASCLK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "sdh_pmg_field_RRMP_U_ASCLK_set", value );

    /* (0x00000000 bits 12) field RRMP_U_ASCLK of register PMC_SDH_PMG_REG_TOP_CFG_REG */
    sdh_pmg_reg_TOP_CFG_REG_field_set( b_ptr,
                                       h_ptr,
                                       SDH_PMG_REG_TOP_CFG_REG_BIT_RRMP_U_ASCLK_MSK,
                                       SDH_PMG_REG_TOP_CFG_REG_BIT_RRMP_U_ASCLK_OFF,
                                       value);
}

static INLINE UINT32 sdh_pmg_field_RRMP_U_ASCLK_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_RRMP_U_ASCLK_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 12) field RRMP_U_ASCLK of register PMC_SDH_PMG_REG_TOP_CFG_REG */
    reg_value = sdh_pmg_reg_TOP_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_TOP_CFG_REG_BIT_RRMP_U_ASCLK_MSK) >> SDH_PMG_REG_TOP_CFG_REG_BIT_RRMP_U_ASCLK_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_RRMP_U_ASCLK_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 sdh_pmg_field_SBER_U_INT_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_SBER_U_INT_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 11:8) field SBER_U_INT of register PMC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = sdh_pmg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_INT_SUMMARY_BIT_SBER_U_INT_MSK) >> SDH_PMG_REG_INT_SUMMARY_BIT_SBER_U_INT_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_SBER_U_INT_get", value );

    return value;
}
static INLINE UINT32 sdh_pmg_field_range_SBER_U_INT_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_range_SBER_U_INT_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_SBER_U_INT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_SBER_U_INT_get", stop_bit, 3 );
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
    /* (0x0000002c bits 11:8) field SBER_U_INT of register PMC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = sdh_pmg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & SDH_PMG_REG_INT_SUMMARY_BIT_SBER_U_INT_MSK)
                  >> SDH_PMG_REG_INT_SUMMARY_BIT_SBER_U_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SDH_PMG_REG_INT_SUMMARY_BIT_SBER_U_INT_MSK, SDH_PMG_REG_INT_SUMMARY_BIT_SBER_U_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_SBER_U_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 sdh_pmg_field_RTTP_U_INT_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_RTTP_U_INT_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 7:4) field RTTP_U_INT of register PMC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = sdh_pmg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_INT_SUMMARY_BIT_RTTP_U_INT_MSK) >> SDH_PMG_REG_INT_SUMMARY_BIT_RTTP_U_INT_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_RTTP_U_INT_get", value );

    return value;
}
static INLINE UINT32 sdh_pmg_field_range_RTTP_U_INT_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_range_RTTP_U_INT_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_RTTP_U_INT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_RTTP_U_INT_get", stop_bit, 3 );
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
    /* (0x0000002c bits 7:4) field RTTP_U_INT of register PMC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = sdh_pmg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & SDH_PMG_REG_INT_SUMMARY_BIT_RTTP_U_INT_MSK)
                  >> SDH_PMG_REG_INT_SUMMARY_BIT_RTTP_U_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SDH_PMG_REG_INT_SUMMARY_BIT_RTTP_U_INT_MSK, SDH_PMG_REG_INT_SUMMARY_BIT_RTTP_U_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_RTTP_U_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 sdh_pmg_field_RRMP_U_INT_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_RRMP_U_INT_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000002c bits 3:0) field RRMP_U_INT of register PMC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = sdh_pmg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_INT_SUMMARY_BIT_RRMP_U_INT_MSK) >> SDH_PMG_REG_INT_SUMMARY_BIT_RRMP_U_INT_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_RRMP_U_INT_get", value );

    return value;
}
static INLINE UINT32 sdh_pmg_field_range_RRMP_U_INT_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_range_RRMP_U_INT_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_RRMP_U_INT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_RRMP_U_INT_get", stop_bit, 3 );
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
    /* (0x0000002c bits 3:0) field RRMP_U_INT of register PMC_SDH_PMG_REG_INT_SUMMARY */
    reg_value = sdh_pmg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & SDH_PMG_REG_INT_SUMMARY_BIT_RRMP_U_INT_MSK)
                  >> SDH_PMG_REG_INT_SUMMARY_BIT_RRMP_U_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SDH_PMG_REG_INT_SUMMARY_BIT_RRMP_U_INT_MSK, SDH_PMG_REG_INT_SUMMARY_BIT_RRMP_U_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_RRMP_U_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 sdh_pmg_field_RRMP_U_TIP_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_RRMP_U_TIP_get( sdh_pmg_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000030 bits 15:0) field RRMP_U_TIP of register PMC_SDH_PMG_REG_TIP_SUMMARY */
    reg_value = sdh_pmg_reg_TIP_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_MSK) >> SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_OFF;
    IOLOG( "%s -> 0x%08x", "sdh_pmg_field_RRMP_U_TIP_get", value );

    return value;
}
static INLINE UINT32 sdh_pmg_field_range_RRMP_U_TIP_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 sdh_pmg_field_range_RRMP_U_TIP_get( sdh_pmg_buffer_t *b_ptr,
                                                         sdh_pmg_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_RRMP_U_TIP_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_RRMP_U_TIP_get", stop_bit, 15 );
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
    /* (0x00000030 bits 15:0) field RRMP_U_TIP of register PMC_SDH_PMG_REG_TIP_SUMMARY */
    reg_value = sdh_pmg_reg_TIP_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_MSK)
                  >> SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_MSK, SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_RRMP_U_TIP_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE sdh_pmg_field_range_RRMP_U_TIP_poll( sdh_pmg_buffer_t *b_ptr,
                                                                        sdh_pmg_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sdh_pmg_field_range_RRMP_U_TIP_poll( sdh_pmg_buffer_t *b_ptr,
                                                                        sdh_pmg_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "sdh_pmg_field_range_RRMP_U_TIP_poll", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "sdh_pmg_field_range_RRMP_U_TIP_poll", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "sdh_pmg_field_range_RRMP_U_TIP_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000030 bits 15:0) field RRMP_U_TIP of register PMC_SDH_PMG_REG_TIP_SUMMARY */
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
        if (stop_bit < 15) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 15;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00000030 bits 15:0) field RRMP_U_TIP of register PMC_SDH_PMG_REG_TIP_SUMMARY */
        return sdh_pmg_reg_TIP_SUMMARY_poll( b_ptr,
                                             h_ptr,
                                             subfield_mask << (SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_OFF + subfield_offset),
                                             value << (SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_OFF + subfield_offset),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE sdh_pmg_field_RRMP_U_TIP_poll( sdh_pmg_buffer_t *b_ptr,
                                                                  sdh_pmg_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE sdh_pmg_field_RRMP_U_TIP_poll( sdh_pmg_buffer_t *b_ptr,
                                                                  sdh_pmg_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "sdh_pmg_field_RRMP_U_TIP_poll", value );

    /* (0x00000030 bits 15:0) field RRMP_U_TIP of register PMC_SDH_PMG_REG_TIP_SUMMARY */
    return sdh_pmg_reg_TIP_SUMMARY_poll( b_ptr,
                                         h_ptr,
                                         SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_MSK,
                                         (value<<SDH_PMG_REG_TIP_SUMMARY_BIT_RRMP_U_TIP_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SDH_PMG_IO_INLINE_H */
