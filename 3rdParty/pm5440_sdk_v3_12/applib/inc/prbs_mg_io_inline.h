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
 *     and register accessor functions for the prbs_mg block
 *****************************************************************************/
#ifndef _PRBS_MG_IO_INLINE_H
#define _PRBS_MG_IO_INLINE_H

#include "cbrc_api.h"
#include "prbs_mg_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PRBS_MG_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for prbs_mg
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
    cbrc_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} prbs_mg_buffer_t;
static INLINE void prbs_mg_buffer_init( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void prbs_mg_buffer_init( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "prbs_mg_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void prbs_mg_buffer_flush( prbs_mg_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void prbs_mg_buffer_flush( prbs_mg_buffer_t *b_ptr )
{
    IOLOG( "prbs_mg_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 prbs_mg_reg_read( prbs_mg_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_reg_read( prbs_mg_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
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
static INLINE void prbs_mg_reg_write( prbs_mg_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_write( prbs_mg_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
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

static INLINE void prbs_mg_field_set( prbs_mg_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_set( prbs_mg_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
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

static INLINE void prbs_mg_action_on_write_field_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_action_on_write_field_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
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

static INLINE void prbs_mg_burst_read( prbs_mg_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void prbs_mg_burst_read( prbs_mg_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
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

static INLINE void prbs_mg_burst_write( prbs_mg_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void prbs_mg_burst_write( prbs_mg_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE prbs_mg_poll( prbs_mg_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_poll( prbs_mg_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
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
 *  register access functions for prbs_mg
 * ==================================================================================
 */

static INLINE void prbs_mg_reg_TOP_CFG_REG_write( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_TOP_CFG_REG_write( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prbs_mg_reg_TOP_CFG_REG_write", value );
    prbs_mg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PRBS_MG_REG_TOP_CFG_REG,
                       value);
}

static INLINE void prbs_mg_reg_TOP_CFG_REG_field_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_TOP_CFG_REG_field_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "prbs_mg_reg_TOP_CFG_REG_field_set", mask, ofs, value );
    prbs_mg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PRBS_MG_REG_TOP_CFG_REG,
                       mask,
                       PMC_PRBS_MG_REG_TOP_CFG_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 prbs_mg_reg_TOP_CFG_REG_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_reg_TOP_CFG_REG_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = prbs_mg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_PRBS_MG_REG_TOP_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "prbs_mg_reg_TOP_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void prbs_mg_reg_SW_RESET_REG__write( prbs_mg_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_SW_RESET_REG__write( prbs_mg_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prbs_mg_reg_SW_RESET_REG__write", value );
    prbs_mg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PRBS_MG_REG_SW_RESET_REG_,
                       value);
}

static INLINE void prbs_mg_reg_SW_RESET_REG__field_set( prbs_mg_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_SW_RESET_REG__field_set( prbs_mg_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 mask,
                                                        UINT32 ofs,
                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "prbs_mg_reg_SW_RESET_REG__field_set", mask, ofs, value );
    prbs_mg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PRBS_MG_REG_SW_RESET_REG_,
                       mask,
                       PMC_PRBS_MG_REG_SW_RESET_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 prbs_mg_reg_SW_RESET_REG__read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_reg_SW_RESET_REG__read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = prbs_mg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_PRBS_MG_REG_SW_RESET_REG_);

    IOLOG( "%s -> 0x%08x;", "prbs_mg_reg_SW_RESET_REG__read", reg_value);
    return reg_value;
}

static INLINE void prbs_mg_reg_LOW_POWER_CFG__write( prbs_mg_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_LOW_POWER_CFG__write( prbs_mg_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prbs_mg_reg_LOW_POWER_CFG__write", value );
    prbs_mg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PRBS_MG_REG_LOW_POWER_CFG_,
                       value);
}

static INLINE void prbs_mg_reg_LOW_POWER_CFG__field_set( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_LOW_POWER_CFG__field_set( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "prbs_mg_reg_LOW_POWER_CFG__field_set", mask, ofs, value );
    prbs_mg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PRBS_MG_REG_LOW_POWER_CFG_,
                       mask,
                       PMC_PRBS_MG_REG_LOW_POWER_CFG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 prbs_mg_reg_LOW_POWER_CFG__read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_reg_LOW_POWER_CFG__read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = prbs_mg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_PRBS_MG_REG_LOW_POWER_CFG_);

    IOLOG( "%s -> 0x%08x;", "prbs_mg_reg_LOW_POWER_CFG__read", reg_value);
    return reg_value;
}

static INLINE void prbs_mg_reg_MTSB_INT_EN_write( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_MTSB_INT_EN_write( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prbs_mg_reg_MTSB_INT_EN_write", value );
    prbs_mg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PRBS_MG_REG_MTSB_INT_EN,
                       value);
}

static INLINE void prbs_mg_reg_MTSB_INT_EN_field_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_MTSB_INT_EN_field_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "prbs_mg_reg_MTSB_INT_EN_field_set", mask, ofs, value );
    prbs_mg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_PRBS_MG_REG_MTSB_INT_EN,
                       mask,
                       PMC_PRBS_MG_REG_MTSB_INT_EN_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 prbs_mg_reg_MTSB_INT_EN_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_reg_MTSB_INT_EN_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = prbs_mg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_PRBS_MG_REG_MTSB_INT_EN);

    IOLOG( "%s -> 0x%08x;", "prbs_mg_reg_MTSB_INT_EN_read", reg_value);
    return reg_value;
}

static INLINE void prbs_mg_reg_ASCLK_PULSE_REG_write( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_ASCLK_PULSE_REG_write( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "prbs_mg_reg_ASCLK_PULSE_REG_write", value );
    prbs_mg_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PRBS_MG_REG_ASCLK_PULSE_REG,
                       value);
}

static INLINE void prbs_mg_reg_ASCLK_PULSE_REG_field_set( prbs_mg_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_reg_ASCLK_PULSE_REG_field_set( prbs_mg_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "prbs_mg_reg_ASCLK_PULSE_REG_field_set", mask, ofs, value );
    prbs_mg_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_PRBS_MG_REG_ASCLK_PULSE_REG,
                       mask,
                       PMC_PRBS_MG_REG_ASCLK_PULSE_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 prbs_mg_reg_ASCLK_PULSE_REG_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_reg_ASCLK_PULSE_REG_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = prbs_mg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PRBS_MG_REG_ASCLK_PULSE_REG);

    IOLOG( "%s -> 0x%08x;", "prbs_mg_reg_ASCLK_PULSE_REG_read", reg_value);
    return reg_value;
}

static INLINE UINT32 prbs_mg_reg_INT_SUMMARY_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_reg_INT_SUMMARY_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = prbs_mg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PRBS_MG_REG_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "prbs_mg_reg_INT_SUMMARY_read", reg_value);
    return reg_value;
}

static INLINE UINT32 prbs_mg_reg_TIP_SUMMARY_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_reg_TIP_SUMMARY_read( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = prbs_mg_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_PRBS_MG_REG_TIP_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "prbs_mg_reg_TIP_SUMMARY_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_reg_TIP_SUMMARY_poll( prbs_mg_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_reg_TIP_SUMMARY_poll( prbs_mg_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 value,
                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                 UINT32 max_count,
                                                                 UINT32 *num_failed_polls,
                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "prbs_mg_reg_TIP_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return prbs_mg_poll( b_ptr,
                         h_ptr,
                         MEM_TYPE_STATUS,
                         PMC_PRBS_MG_REG_TIP_SUMMARY,
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
static INLINE void prbs_mg_field_PRBS_MG_RX_CHNL_EN_set( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_PRBS_MG_RX_CHNL_EN_set( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_PRBS_MG_RX_CHNL_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_PRBS_MG_RX_CHNL_EN_set", value );

    /* (0x00001000 bits 2) field PRBS_MG_RX_CHNL_EN of register PMC_PRBS_MG_REG_TOP_CFG_REG */
    prbs_mg_reg_TOP_CFG_REG_field_set( b_ptr,
                                       h_ptr,
                                       PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_RX_CHNL_EN_MSK,
                                       PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_RX_CHNL_EN_OFF,
                                       value);
}

static INLINE UINT32 prbs_mg_field_PRBS_MG_RX_CHNL_EN_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRBS_MG_RX_CHNL_EN_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 2) field PRBS_MG_RX_CHNL_EN of register PMC_PRBS_MG_REG_TOP_CFG_REG */
    reg_value = prbs_mg_reg_TOP_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_RX_CHNL_EN_MSK) >> PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_RX_CHNL_EN_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRBS_MG_RX_CHNL_EN_get", value );

    return value;
}
static INLINE void prbs_mg_field_PRBS_MG_TX_CHNL_EN_set( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_PRBS_MG_TX_CHNL_EN_set( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_PRBS_MG_TX_CHNL_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_PRBS_MG_TX_CHNL_EN_set", value );

    /* (0x00001000 bits 1) field PRBS_MG_TX_CHNL_EN of register PMC_PRBS_MG_REG_TOP_CFG_REG */
    prbs_mg_reg_TOP_CFG_REG_field_set( b_ptr,
                                       h_ptr,
                                       PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_TX_CHNL_EN_MSK,
                                       PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_TX_CHNL_EN_OFF,
                                       value);
}

static INLINE UINT32 prbs_mg_field_PRBS_MG_TX_CHNL_EN_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRBS_MG_TX_CHNL_EN_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 1) field PRBS_MG_TX_CHNL_EN of register PMC_PRBS_MG_REG_TOP_CFG_REG */
    reg_value = prbs_mg_reg_TOP_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_TX_CHNL_EN_MSK) >> PRBS_MG_REG_TOP_CFG_REG_BIT_PRBS_MG_TX_CHNL_EN_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRBS_MG_TX_CHNL_EN_get", value );

    return value;
}
static INLINE void prbs_mg_field_OC_768_MODE_set( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_OC_768_MODE_set( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_OC_768_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_OC_768_MODE_set", value );

    /* (0x00001000 bits 0) field OC_768_MODE of register PMC_PRBS_MG_REG_TOP_CFG_REG */
    prbs_mg_reg_TOP_CFG_REG_field_set( b_ptr,
                                       h_ptr,
                                       PRBS_MG_REG_TOP_CFG_REG_BIT_OC_768_MODE_MSK,
                                       PRBS_MG_REG_TOP_CFG_REG_BIT_OC_768_MODE_OFF,
                                       value);
}

static INLINE UINT32 prbs_mg_field_OC_768_MODE_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_OC_768_MODE_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001000 bits 0) field OC_768_MODE of register PMC_PRBS_MG_REG_TOP_CFG_REG */
    reg_value = prbs_mg_reg_TOP_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_TOP_CFG_REG_BIT_OC_768_MODE_MSK) >> PRBS_MG_REG_TOP_CFG_REG_BIT_OC_768_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_OC_768_MODE_get", value );

    return value;
}
static INLINE void prbs_mg_field_PRBS_RX_SW_RST_set( prbs_mg_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_PRBS_RX_SW_RST_set( prbs_mg_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_PRBS_RX_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_PRBS_RX_SW_RST_set", value );

    /* (0x00001004 bits 1) field PRBS_RX_SW_RST of register PMC_PRBS_MG_REG_SW_RESET_REG_ */
    prbs_mg_reg_SW_RESET_REG__field_set( b_ptr,
                                         h_ptr,
                                         PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_RX_SW_RST_MSK,
                                         PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_RX_SW_RST_OFF,
                                         value);
}

static INLINE UINT32 prbs_mg_field_PRBS_RX_SW_RST_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRBS_RX_SW_RST_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 1) field PRBS_RX_SW_RST of register PMC_PRBS_MG_REG_SW_RESET_REG_ */
    reg_value = prbs_mg_reg_SW_RESET_REG__read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_RX_SW_RST_MSK) >> PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_RX_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRBS_RX_SW_RST_get", value );

    return value;
}
static INLINE void prbs_mg_field_PRBS_TX_SW_RST_set( prbs_mg_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_PRBS_TX_SW_RST_set( prbs_mg_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_PRBS_TX_SW_RST_set", value, 1);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_PRBS_TX_SW_RST_set", value );

    /* (0x00001004 bits 0) field PRBS_TX_SW_RST of register PMC_PRBS_MG_REG_SW_RESET_REG_ */
    prbs_mg_reg_SW_RESET_REG__field_set( b_ptr,
                                         h_ptr,
                                         PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_TX_SW_RST_MSK,
                                         PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_TX_SW_RST_OFF,
                                         value);
}

static INLINE UINT32 prbs_mg_field_PRBS_TX_SW_RST_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRBS_TX_SW_RST_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001004 bits 0) field PRBS_TX_SW_RST of register PMC_PRBS_MG_REG_SW_RESET_REG_ */
    reg_value = prbs_mg_reg_SW_RESET_REG__read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_TX_SW_RST_MSK) >> PRBS_MG_REG_SW_RESET_REG_BIT_PRBS_TX_SW_RST_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRBS_TX_SW_RST_get", value );

    return value;
}
static INLINE void prbs_mg_field_PRBS_RX_LOW_PWR_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_PRBS_RX_LOW_PWR_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_PRBS_RX_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_PRBS_RX_LOW_PWR_set", value );

    /* (0x0000100c bits 1) field PRBS_RX_LOW_PWR of register PMC_PRBS_MG_REG_LOW_POWER_CFG_ */
    prbs_mg_reg_LOW_POWER_CFG__field_set( b_ptr,
                                          h_ptr,
                                          PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_RX_LOW_PWR_MSK,
                                          PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_RX_LOW_PWR_OFF,
                                          value);
}

static INLINE UINT32 prbs_mg_field_PRBS_RX_LOW_PWR_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRBS_RX_LOW_PWR_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 1) field PRBS_RX_LOW_PWR of register PMC_PRBS_MG_REG_LOW_POWER_CFG_ */
    reg_value = prbs_mg_reg_LOW_POWER_CFG__read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_RX_LOW_PWR_MSK) >> PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_RX_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRBS_RX_LOW_PWR_get", value );

    return value;
}
static INLINE void prbs_mg_field_PRBS_TX_LOW_PWR_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_PRBS_TX_LOW_PWR_set( prbs_mg_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_PRBS_TX_LOW_PWR_set", value, 1);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_PRBS_TX_LOW_PWR_set", value );

    /* (0x0000100c bits 0) field PRBS_TX_LOW_PWR of register PMC_PRBS_MG_REG_LOW_POWER_CFG_ */
    prbs_mg_reg_LOW_POWER_CFG__field_set( b_ptr,
                                          h_ptr,
                                          PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_TX_LOW_PWR_MSK,
                                          PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_TX_LOW_PWR_OFF,
                                          value);
}

static INLINE UINT32 prbs_mg_field_PRBS_TX_LOW_PWR_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRBS_TX_LOW_PWR_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000100c bits 0) field PRBS_TX_LOW_PWR of register PMC_PRBS_MG_REG_LOW_POWER_CFG_ */
    reg_value = prbs_mg_reg_LOW_POWER_CFG__read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_TX_LOW_PWR_MSK) >> PRBS_MG_REG_LOW_POWER_CFG_BIT_PRBS_TX_LOW_PWR_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRBS_TX_LOW_PWR_get", value );

    return value;
}
static INLINE void prbs_mg_field_SHPI_U_INT_EN_set( prbs_mg_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_SHPI_U_INT_EN_set( prbs_mg_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_SHPI_U_INT_EN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_SHPI_U_INT_EN_set", value );

    /* (0x00001010 bits 7:4) field SHPI_U_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
    prbs_mg_reg_MTSB_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_MSK,
                                       PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_OFF,
                                       value);
}

static INLINE UINT32 prbs_mg_field_SHPI_U_INT_EN_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_SHPI_U_INT_EN_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 7:4) field SHPI_U_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
    reg_value = prbs_mg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_MSK) >> PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_SHPI_U_INT_EN_get", value );

    return value;
}
static INLINE void prbs_mg_field_range_SHPI_U_INT_EN_set( prbs_mg_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_range_SHPI_U_INT_EN_set( prbs_mg_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_SHPI_U_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_SHPI_U_INT_EN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_SHPI_U_INT_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001010 bits 7:4) field SHPI_U_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
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
        /* (0x00001010 bits 7:4) field SHPI_U_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
        prbs_mg_reg_MTSB_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_OFF + subfield_offset),
                                           PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 prbs_mg_field_range_SHPI_U_INT_EN_get( prbs_mg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_range_SHPI_U_INT_EN_get( prbs_mg_buffer_t *b_ptr,
                                                            cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_SHPI_U_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_SHPI_U_INT_EN_get", stop_bit, 3 );
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
    /* (0x00001010 bits 7:4) field SHPI_U_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
    reg_value = prbs_mg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_MSK)
                  >> PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_MSK, PRBS_MG_REG_MTSB_INT_EN_BIT_SHPI_U_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_SHPI_U_INT_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void prbs_mg_field_PRGM_INT_EN_set( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_PRGM_INT_EN_set( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_PRGM_INT_EN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_PRGM_INT_EN_set", value );

    /* (0x00001010 bits 3:0) field PRGM_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
    prbs_mg_reg_MTSB_INT_EN_field_set( b_ptr,
                                       h_ptr,
                                       PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_MSK,
                                       PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_OFF,
                                       value);
}

static INLINE UINT32 prbs_mg_field_PRGM_INT_EN_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRGM_INT_EN_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001010 bits 3:0) field PRGM_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
    reg_value = prbs_mg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_MSK) >> PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRGM_INT_EN_get", value );

    return value;
}
static INLINE void prbs_mg_field_range_PRGM_INT_EN_set( prbs_mg_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_range_PRGM_INT_EN_set( prbs_mg_buffer_t *b_ptr,
                                                        cbrc_handle_t *h_ptr,
                                                        UINT32 start_bit,
                                                        UINT32 stop_bit,
                                                        UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_PRGM_INT_EN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_PRGM_INT_EN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_PRGM_INT_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001010 bits 3:0) field PRGM_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
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
        /* (0x00001010 bits 3:0) field PRGM_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
        prbs_mg_reg_MTSB_INT_EN_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_OFF + subfield_offset),
                                           PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 prbs_mg_field_range_PRGM_INT_EN_get( prbs_mg_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_range_PRGM_INT_EN_get( prbs_mg_buffer_t *b_ptr,
                                                          cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_PRGM_INT_EN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_PRGM_INT_EN_get", stop_bit, 3 );
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
    /* (0x00001010 bits 3:0) field PRGM_INT_EN of register PMC_PRBS_MG_REG_MTSB_INT_EN */
    reg_value = prbs_mg_reg_MTSB_INT_EN_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_MSK)
                  >> PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_MSK, PRBS_MG_REG_MTSB_INT_EN_BIT_PRGM_INT_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_PRGM_INT_EN_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void prbs_mg_field_ASCLK_PULSE_set( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void prbs_mg_field_ASCLK_PULSE_set( prbs_mg_buffer_t *b_ptr,
                                                  cbrc_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "prbs_mg_field_ASCLK_PULSE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "prbs_mg_field_ASCLK_PULSE_set", value );

    /* (0x00001008 bits 0) field ASCLK_PULSE of register PMC_PRBS_MG_REG_ASCLK_PULSE_REG */
    prbs_mg_reg_ASCLK_PULSE_REG_field_set( b_ptr,
                                           h_ptr,
                                           PRBS_MG_REG_ASCLK_PULSE_REG_BIT_ASCLK_PULSE_MSK,
                                           PRBS_MG_REG_ASCLK_PULSE_REG_BIT_ASCLK_PULSE_OFF,
                                           value);
}

static INLINE UINT32 prbs_mg_field_ASCLK_PULSE_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_ASCLK_PULSE_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001008 bits 0) field ASCLK_PULSE of register PMC_PRBS_MG_REG_ASCLK_PULSE_REG */
    reg_value = prbs_mg_reg_ASCLK_PULSE_REG_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_ASCLK_PULSE_REG_BIT_ASCLK_PULSE_MSK) >> PRBS_MG_REG_ASCLK_PULSE_REG_BIT_ASCLK_PULSE_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_ASCLK_PULSE_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 prbs_mg_field_SHPI_U_INT_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_SHPI_U_INT_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 7:4) field SHPI_U_INT of register PMC_PRBS_MG_REG_INT_SUMMARY */
    reg_value = prbs_mg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_INT_SUMMARY_BIT_SHPI_U_INT_MSK) >> PRBS_MG_REG_INT_SUMMARY_BIT_SHPI_U_INT_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_SHPI_U_INT_get", value );

    return value;
}
static INLINE UINT32 prbs_mg_field_range_SHPI_U_INT_get( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_range_SHPI_U_INT_get( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_SHPI_U_INT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_SHPI_U_INT_get", stop_bit, 3 );
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
    /* (0x00001014 bits 7:4) field SHPI_U_INT of register PMC_PRBS_MG_REG_INT_SUMMARY */
    reg_value = prbs_mg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_MG_REG_INT_SUMMARY_BIT_SHPI_U_INT_MSK)
                  >> PRBS_MG_REG_INT_SUMMARY_BIT_SHPI_U_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_MG_REG_INT_SUMMARY_BIT_SHPI_U_INT_MSK, PRBS_MG_REG_INT_SUMMARY_BIT_SHPI_U_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_SHPI_U_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 prbs_mg_field_PRGM_INT_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRGM_INT_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001014 bits 3:0) field PRGM_INT of register PMC_PRBS_MG_REG_INT_SUMMARY */
    reg_value = prbs_mg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_INT_SUMMARY_BIT_PRGM_INT_MSK) >> PRBS_MG_REG_INT_SUMMARY_BIT_PRGM_INT_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRGM_INT_get", value );

    return value;
}
static INLINE UINT32 prbs_mg_field_range_PRGM_INT_get( prbs_mg_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_range_PRGM_INT_get( prbs_mg_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_PRGM_INT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_PRGM_INT_get", stop_bit, 3 );
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
    /* (0x00001014 bits 3:0) field PRGM_INT of register PMC_PRBS_MG_REG_INT_SUMMARY */
    reg_value = prbs_mg_reg_INT_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_MG_REG_INT_SUMMARY_BIT_PRGM_INT_MSK)
                  >> PRBS_MG_REG_INT_SUMMARY_BIT_PRGM_INT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_MG_REG_INT_SUMMARY_BIT_PRGM_INT_MSK, PRBS_MG_REG_INT_SUMMARY_BIT_PRGM_INT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_PRGM_INT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 prbs_mg_field_SHPI_U_TIP_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_SHPI_U_TIP_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 7:4) field SHPI_U_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
    reg_value = prbs_mg_reg_TIP_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_MSK) >> PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_SHPI_U_TIP_get", value );

    return value;
}
static INLINE UINT32 prbs_mg_field_range_SHPI_U_TIP_get( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 start_bit,
                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_range_SHPI_U_TIP_get( prbs_mg_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_SHPI_U_TIP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_SHPI_U_TIP_get", stop_bit, 3 );
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
    /* (0x00001018 bits 7:4) field SHPI_U_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
    reg_value = prbs_mg_reg_TIP_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_MSK)
                  >> PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_MSK, PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_SHPI_U_TIP_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_field_range_SHPI_U_TIP_poll( prbs_mg_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_field_range_SHPI_U_TIP_poll( prbs_mg_buffer_t *b_ptr,
                                                                        cbrc_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_SHPI_U_TIP_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_SHPI_U_TIP_poll", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_SHPI_U_TIP_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001018 bits 7:4) field SHPI_U_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001018 bits 7:4) field SHPI_U_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
        return prbs_mg_reg_TIP_SUMMARY_poll( b_ptr,
                                             h_ptr,
                                             subfield_mask << (PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_OFF + subfield_offset),
                                             value << (PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_OFF + subfield_offset),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE prbs_mg_field_SHPI_U_TIP_poll( prbs_mg_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_field_SHPI_U_TIP_poll( prbs_mg_buffer_t *b_ptr,
                                                                  cbrc_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "prbs_mg_field_SHPI_U_TIP_poll", value );

    /* (0x00001018 bits 7:4) field SHPI_U_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
    return prbs_mg_reg_TIP_SUMMARY_poll( b_ptr,
                                         h_ptr,
                                         PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_MSK,
                                         (value<<PRBS_MG_REG_TIP_SUMMARY_BIT_SHPI_U_TIP_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}

static INLINE UINT32 prbs_mg_field_PRGM_TIP_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_PRGM_TIP_get( prbs_mg_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00001018 bits 3:0) field PRGM_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
    reg_value = prbs_mg_reg_TIP_SUMMARY_read(  b_ptr, h_ptr);
    value = (reg_value & PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_MSK) >> PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_OFF;
    IOLOG( "%s -> 0x%08x", "prbs_mg_field_PRGM_TIP_get", value );

    return value;
}
static INLINE UINT32 prbs_mg_field_range_PRGM_TIP_get( prbs_mg_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 prbs_mg_field_range_PRGM_TIP_get( prbs_mg_buffer_t *b_ptr,
                                                       cbrc_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_PRGM_TIP_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_PRGM_TIP_get", stop_bit, 3 );
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
    /* (0x00001018 bits 3:0) field PRGM_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
    reg_value = prbs_mg_reg_TIP_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_MSK)
                  >> PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_MSK, PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_PRGM_TIP_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_field_range_PRGM_TIP_poll( prbs_mg_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_field_range_PRGM_TIP_poll( prbs_mg_buffer_t *b_ptr,
                                                                      cbrc_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "prbs_mg_field_range_PRGM_TIP_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "prbs_mg_field_range_PRGM_TIP_poll", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "prbs_mg_field_range_PRGM_TIP_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00001018 bits 3:0) field PRGM_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
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
        if (stop_bit < 3) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 3;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00001018 bits 3:0) field PRGM_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
        return prbs_mg_reg_TIP_SUMMARY_poll( b_ptr,
                                             h_ptr,
                                             subfield_mask << (PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_OFF + subfield_offset),
                                             value << (PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_OFF + subfield_offset),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE prbs_mg_field_PRGM_TIP_poll( prbs_mg_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE prbs_mg_field_PRGM_TIP_poll( prbs_mg_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32 value,
                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                UINT32 max_count,
                                                                UINT32 *num_failed_polls,
                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "prbs_mg_field_PRGM_TIP_poll", value );

    /* (0x00001018 bits 3:0) field PRGM_TIP of register PMC_PRBS_MG_REG_TIP_SUMMARY */
    return prbs_mg_reg_TIP_SUMMARY_poll( b_ptr,
                                         h_ptr,
                                         PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_MSK,
                                         (value<<PRBS_MG_REG_TIP_SUMMARY_BIT_PRGM_TIP_OFF),
                                         cmp,
                                         max_count,
                                         num_failed_polls,
                                         delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PRBS_MG_IO_INLINE_H */
