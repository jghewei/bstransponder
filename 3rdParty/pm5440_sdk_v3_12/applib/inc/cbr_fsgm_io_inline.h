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
 *     and register accessor functions for the cbr_fsgm block
 *****************************************************************************/
#ifndef _CBR_FSGM_IO_INLINE_H
#define _CBR_FSGM_IO_INLINE_H

#include "cbr_fsgm_api.h"
#include "cbr_fsgm_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CBR_FSGM_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for cbr_fsgm
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
    cbr_fsgm_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} cbr_fsgm_buffer_t;
static INLINE void cbr_fsgm_buffer_init( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_buffer_init( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "cbr_fsgm_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void cbr_fsgm_buffer_flush( cbr_fsgm_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_buffer_flush( cbr_fsgm_buffer_t *b_ptr )
{
    IOLOG( "cbr_fsgm_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 cbr_fsgm_reg_read( cbr_fsgm_buffer_t *b_ptr,
                                        cbr_fsgm_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_reg_read( cbr_fsgm_buffer_t *b_ptr,
                                        cbr_fsgm_handle_t *h_ptr,
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
static INLINE void cbr_fsgm_reg_write( cbr_fsgm_buffer_t *b_ptr,
                                       cbr_fsgm_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_write( cbr_fsgm_buffer_t *b_ptr,
                                       cbr_fsgm_handle_t *h_ptr,
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

static INLINE void cbr_fsgm_field_set( cbr_fsgm_buffer_t *b_ptr,
                                       cbr_fsgm_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 mask,
                                       UINT32 unused_mask,
                                       UINT32 ofs,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_set( cbr_fsgm_buffer_t *b_ptr,
                                       cbr_fsgm_handle_t *h_ptr,
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

static INLINE void cbr_fsgm_action_on_write_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                       cbr_fsgm_handle_t *h_ptr,
                                                       UINT32 mem_type,
                                                       UINT32 reg,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_action_on_write_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                       cbr_fsgm_handle_t *h_ptr,
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

static INLINE void cbr_fsgm_burst_read( cbr_fsgm_buffer_t *b_ptr,
                                        cbr_fsgm_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_burst_read( cbr_fsgm_buffer_t *b_ptr,
                                        cbr_fsgm_handle_t *h_ptr,
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

static INLINE void cbr_fsgm_burst_write( cbr_fsgm_buffer_t *b_ptr,
                                         cbr_fsgm_handle_t *h_ptr,
                                         UINT32 mem_type,
                                         UINT32 reg,
                                         UINT32 len,
                                         UINT32 *value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_burst_write( cbr_fsgm_buffer_t *b_ptr,
                                         cbr_fsgm_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_poll( cbr_fsgm_buffer_t *b_ptr,
                                                  cbr_fsgm_handle_t *h_ptr,
                                                  UINT32 mem_type,
                                                  UINT32 reg,
                                                  UINT32 mask,
                                                  UINT32 value,
                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                  UINT32 max_count,
                                                  UINT32 *num_failed_polls,
                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_poll( cbr_fsgm_buffer_t *b_ptr,
                                                  cbr_fsgm_handle_t *h_ptr,
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
 *  register access functions for cbr_fsgm
 * ==================================================================================
 */

static INLINE void cbr_fsgm_reg_GLOBAL_CFG_REG_write( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_GLOBAL_CFG_REG_write( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_fsgm_reg_GLOBAL_CFG_REG_write", value );
    cbr_fsgm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CBR_FSGM_REG_GLOBAL_CFG_REG,
                        value);
}

static INLINE void cbr_fsgm_reg_GLOBAL_CFG_REG_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_GLOBAL_CFG_REG_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbr_fsgm_reg_GLOBAL_CFG_REG_field_set", mask, ofs, value );
    cbr_fsgm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CBR_FSGM_REG_GLOBAL_CFG_REG,
                        mask,
                        PMC_CBR_FSGM_REG_GLOBAL_CFG_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 cbr_fsgm_reg_GLOBAL_CFG_REG_read( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_reg_GLOBAL_CFG_REG_read( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbr_fsgm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CBR_FSGM_REG_GLOBAL_CFG_REG);

    IOLOG( "%s -> 0x%08x;", "cbr_fsgm_reg_GLOBAL_CFG_REG_read", reg_value);
    return reg_value;
}

static INLINE void cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_write( cbr_fsgm_buffer_t *b_ptr,
                                                                          cbr_fsgm_handle_t *h_ptr,
                                                                          UINT32  channel,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_write( cbr_fsgm_buffer_t *b_ptr,
                                                                          cbr_fsgm_handle_t *h_ptr,
                                                                          UINT32  channel,
                                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_write", value );
    cbr_fsgm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL(channel),
                        value);
}

static INLINE void cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32  channel,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32  channel,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- channel=%d mask=0x%08x ofs=%d value=0x%08x", "cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set", channel, mask, ofs, value );
    cbr_fsgm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL(channel),
                        mask,
                        PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( cbr_fsgm_buffer_t *b_ptr,
                                                                           cbr_fsgm_handle_t *h_ptr,
                                                                           UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( cbr_fsgm_buffer_t *b_ptr,
                                                                           cbr_fsgm_handle_t *h_ptr,
                                                                           UINT32  channel )
{
    UINT32 reg_value;

    reg_value = cbr_fsgm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL(channel));

    IOLOG( "%s -> 0x%08x; channel=%d", "cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read", reg_value, channel);
    return reg_value;
}

static INLINE void cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_write( cbr_fsgm_buffer_t *b_ptr,
                                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                                  UINT32  channel,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_write( cbr_fsgm_buffer_t *b_ptr,
                                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                                  UINT32  channel,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_write", value );
    cbr_fsgm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL(channel),
                        value);
}

static INLINE void cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                                      UINT32  channel,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                                      UINT32  channel,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- channel=%d mask=0x%08x ofs=%d value=0x%08x", "cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_field_set", channel, mask, ofs, value );
    cbr_fsgm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL(channel),
                        mask,
                        PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_read( cbr_fsgm_buffer_t *b_ptr,
                                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                                   UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_read( cbr_fsgm_buffer_t *b_ptr,
                                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                                   UINT32  channel )
{
    UINT32 reg_value;

    reg_value = cbr_fsgm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL(channel));

    IOLOG( "%s -> 0x%08x; channel=%d", "cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_read", reg_value, channel);
    return reg_value;
}

static INLINE void cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_write( cbr_fsgm_buffer_t *b_ptr,
                                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                                  UINT32  channel,
                                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_write( cbr_fsgm_buffer_t *b_ptr,
                                                                                  cbr_fsgm_handle_t *h_ptr,
                                                                                  UINT32  channel,
                                                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_write", value );
    cbr_fsgm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL(channel),
                        value);
}

static INLINE void cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                                      UINT32  channel,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                                                      cbr_fsgm_handle_t *h_ptr,
                                                                                      UINT32  channel,
                                                                                      UINT32 mask,
                                                                                      UINT32 ofs,
                                                                                      UINT32 value )
{
    IOLOG( "%s <- channel=%d mask=0x%08x ofs=%d value=0x%08x", "cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_field_set", channel, mask, ofs, value );
    cbr_fsgm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_CONFIG,
                        PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL(channel),
                        mask,
                        PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_read( cbr_fsgm_buffer_t *b_ptr,
                                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                                   UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_read( cbr_fsgm_buffer_t *b_ptr,
                                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                                   UINT32  channel )
{
    UINT32 reg_value;

    reg_value = cbr_fsgm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_CONFIG,
                                   PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL(channel));

    IOLOG( "%s -> 0x%08x; channel=%d", "cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_read", reg_value, channel);
    return reg_value;
}

static INLINE void cbr_fsgm_reg_INT_EN_REG_write( cbr_fsgm_buffer_t *b_ptr,
                                                  cbr_fsgm_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_INT_EN_REG_write( cbr_fsgm_buffer_t *b_ptr,
                                                  cbr_fsgm_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_fsgm_reg_INT_EN_REG_write", value );
    cbr_fsgm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_CBR_FSGM_REG_INT_EN_REG,
                        value);
}

static INLINE void cbr_fsgm_reg_INT_EN_REG_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_INT_EN_REG_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbr_fsgm_reg_INT_EN_REG_field_set", mask, ofs, value );
    cbr_fsgm_field_set( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_CBR_FSGM_REG_INT_EN_REG,
                        mask,
                        PMC_CBR_FSGM_REG_INT_EN_REG_UNUSED_MASK,
                        ofs,
                        value);

}

static INLINE UINT32 cbr_fsgm_reg_INT_EN_REG_read( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_reg_INT_EN_REG_read( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbr_fsgm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_CBR_FSGM_REG_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "cbr_fsgm_reg_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void cbr_fsgm_reg_INT_INDICATION_REG_write( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_INT_INDICATION_REG_write( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_fsgm_reg_INT_INDICATION_REG_write", value );
    cbr_fsgm_reg_write( b_ptr,
                        h_ptr,
                        MEM_TYPE_STATUS,
                        PMC_CBR_FSGM_REG_INT_INDICATION_REG,
                        value);
}

static INLINE void cbr_fsgm_reg_INT_INDICATION_REG_action_on_write_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_reg_INT_INDICATION_REG_action_on_write_field_set( cbr_fsgm_buffer_t *b_ptr,
                                                                              cbr_fsgm_handle_t *h_ptr,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbr_fsgm_reg_INT_INDICATION_REG_action_on_write_field_set", mask, ofs, value );
    cbr_fsgm_action_on_write_field_set( b_ptr,
                                        h_ptr,
                                        MEM_TYPE_STATUS,
                                        PMC_CBR_FSGM_REG_INT_INDICATION_REG,
                                        mask,
                                        ofs,
                                        value);

}

static INLINE UINT32 cbr_fsgm_reg_INT_INDICATION_REG_read( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_reg_INT_INDICATION_REG_read( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbr_fsgm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_CBR_FSGM_REG_INT_INDICATION_REG);

    IOLOG( "%s -> 0x%08x;", "cbr_fsgm_reg_INT_INDICATION_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_reg_INT_INDICATION_REG_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                         cbr_fsgm_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_reg_INT_INDICATION_REG_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                         cbr_fsgm_handle_t *h_ptr,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "cbr_fsgm_reg_INT_INDICATION_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return cbr_fsgm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_CBR_FSGM_REG_INT_INDICATION_REG,
                          mask,
                          value,
                          cmp,
                          max_count,
                          num_failed_polls,
                          delay_between_polls_in_microseconds);

}


static INLINE UINT32 cbr_fsgm_reg_INT_STAT_REG_read( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_reg_INT_STAT_REG_read( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbr_fsgm_reg_read( b_ptr,
                                   h_ptr,
                                   MEM_TYPE_STATUS,
                                   PMC_CBR_FSGM_REG_INT_STAT_REG);

    IOLOG( "%s -> 0x%08x;", "cbr_fsgm_reg_INT_STAT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_reg_INT_STAT_REG_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_reg_INT_STAT_REG_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                   cbr_fsgm_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "cbr_fsgm_reg_INT_STAT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return cbr_fsgm_poll( b_ptr,
                          h_ptr,
                          MEM_TYPE_STATUS,
                          PMC_CBR_FSGM_REG_INT_STAT_REG,
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
static INLINE void cbr_fsgm_field_MS_AIS_RSOH_FILL_set( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_MS_AIS_RSOH_FILL_set( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_MS_AIS_RSOH_FILL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_MS_AIS_RSOH_FILL_set", value );

    /* (0x00000000 bits 5) field MS_AIS_RSOH_FILL of register PMC_CBR_FSGM_REG_GLOBAL_CFG_REG */
    cbr_fsgm_reg_GLOBAL_CFG_REG_field_set( b_ptr,
                                           h_ptr,
                                           CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MS_AIS_RSOH_FILL_MSK,
                                           CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MS_AIS_RSOH_FILL_OFF,
                                           value);
}

static INLINE UINT32 cbr_fsgm_field_MS_AIS_RSOH_FILL_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_MS_AIS_RSOH_FILL_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 5) field MS_AIS_RSOH_FILL of register PMC_CBR_FSGM_REG_GLOBAL_CFG_REG */
    reg_value = cbr_fsgm_reg_GLOBAL_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MS_AIS_RSOH_FILL_MSK) >> CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MS_AIS_RSOH_FILL_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_MS_AIS_RSOH_FILL_get", value );

    return value;
}
static INLINE void cbr_fsgm_field_MODE_40G_set( cbr_fsgm_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_MODE_40G_set( cbr_fsgm_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_MODE_40G_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_MODE_40G_set", value );

    /* (0x00000000 bits 4) field MODE_40G of register PMC_CBR_FSGM_REG_GLOBAL_CFG_REG */
    cbr_fsgm_reg_GLOBAL_CFG_REG_field_set( b_ptr,
                                           h_ptr,
                                           CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MODE_40G_MSK,
                                           CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MODE_40G_OFF,
                                           value);
}

static INLINE UINT32 cbr_fsgm_field_MODE_40G_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_MODE_40G_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 4) field MODE_40G of register PMC_CBR_FSGM_REG_GLOBAL_CFG_REG */
    reg_value = cbr_fsgm_reg_GLOBAL_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MODE_40G_MSK) >> CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_MODE_40G_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_MODE_40G_get", value );

    return value;
}
static INLINE void cbr_fsgm_field_SW_RESET_set( cbr_fsgm_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_SW_RESET_set( cbr_fsgm_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_SW_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_SW_RESET_set", value );

    /* (0x00000000 bits 0) field SW_RESET of register PMC_CBR_FSGM_REG_GLOBAL_CFG_REG */
    cbr_fsgm_reg_GLOBAL_CFG_REG_field_set( b_ptr,
                                           h_ptr,
                                           CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_SW_RESET_MSK,
                                           CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_SW_RESET_OFF,
                                           value);
}

static INLINE UINT32 cbr_fsgm_field_SW_RESET_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_SW_RESET_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000000 bits 0) field SW_RESET of register PMC_CBR_FSGM_REG_GLOBAL_CFG_REG */
    reg_value = cbr_fsgm_reg_GLOBAL_CFG_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_SW_RESET_MSK) >> CBR_FSGM_REG_GLOBAL_CFG_REG_BIT_SW_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_SW_RESET_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_channel_size4
 * ==================================================================================
 */
static INLINE void cbr_fsgm_field_CH_n_FAULT_INS_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32  channel,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_FAULT_INS_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32  channel,
                                                          UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_FAULT_INS_ENA_set", channel, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH_n_FAULT_INS_ENA_set", value, 1);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_FAULT_INS_ENA_set", channel, value );

    /* ((0x00000004 + (channel) * 0x0000000c) bits 6) field CH_n_FAULT_INS_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                               h_ptr,
                                                               channel,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_INS_ENA_MSK,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_INS_ENA_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_FAULT_INS_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_FAULT_INS_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_FAULT_INS_ENA_get", channel, 3);
    /* ((0x00000004 + (channel) * 0x0000000c) bits 6) field CH_n_FAULT_INS_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_INS_ENA_MSK) >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_INS_ENA_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_FAULT_INS_ENA_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_PN11_MON_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32  channel,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_PN11_MON_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32  channel,
                                                         UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_PN11_MON_ENA_set", channel, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH_n_PN11_MON_ENA_set", value, 1);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_PN11_MON_ENA_set", channel, value );

    /* ((0x00000004 + (channel) * 0x0000000c) bits 1) field CH_n_PN11_MON_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                               h_ptr,
                                                               channel,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PN11_MON_ENA_MSK,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PN11_MON_ENA_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_PN11_MON_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_PN11_MON_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_PN11_MON_ENA_get", channel, 3);
    /* ((0x00000004 + (channel) * 0x0000000c) bits 1) field CH_n_PN11_MON_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PN11_MON_ENA_MSK) >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PN11_MON_ENA_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_PN11_MON_ENA_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32  channel,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32  channel,
                                                           UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set", channel, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set", value, 1);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set", channel, value );

    /* ((0x00000004 + (channel) * 0x0000000c) bits 8) field CH_n_FAULT_PROP_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                               h_ptr,
                                                               channel,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_PROP_ENA_MSK,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_PROP_ENA_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_FAULT_PROP_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_FAULT_PROP_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                             cbr_fsgm_handle_t *h_ptr,
                                                             UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_FAULT_PROP_ENA_get", channel, 3);
    /* ((0x00000004 + (channel) * 0x0000000c) bits 8) field CH_n_FAULT_PROP_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_PROP_ENA_MSK) >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FAULT_PROP_ENA_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_FAULT_PROP_ENA_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_FORCE_FAULT_set( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32  channel,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_FORCE_FAULT_set( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32  channel,
                                                        UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_FORCE_FAULT_set", channel, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH_n_FORCE_FAULT_set", value, 1);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_FORCE_FAULT_set", channel, value );

    /* ((0x00000004 + (channel) * 0x0000000c) bits 4) field CH_n_FORCE_FAULT of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                               h_ptr,
                                                               channel,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FORCE_FAULT_MSK,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FORCE_FAULT_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_FORCE_FAULT_get( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_FORCE_FAULT_get( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_FORCE_FAULT_get", channel, 3);
    /* ((0x00000004 + (channel) * 0x0000000c) bits 4) field CH_n_FORCE_FAULT of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FORCE_FAULT_MSK) >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_FORCE_FAULT_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_FORCE_FAULT_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32  channel,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                cbr_fsgm_handle_t *h_ptr,
                                                UINT32  channel,
                                                UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_ENA_set", channel, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH_n_ENA_set", value, 1);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_ENA_set", channel, value );

    /* ((0x00000004 + (channel) * 0x0000000c) bits 0) field CH_n_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                               h_ptr,
                                                               channel,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_ENA_MSK,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_ENA_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                  cbr_fsgm_handle_t *h_ptr,
                                                  UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                  cbr_fsgm_handle_t *h_ptr,
                                                  UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_ENA_get", channel, 3);
    /* ((0x00000004 + (channel) * 0x0000000c) bits 0) field CH_n_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_ENA_MSK) >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_ENA_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_ENA_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_PATT_MODE_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32  channel,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_PATT_MODE_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32  channel,
                                                      UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_PATT_MODE_set", channel, 3);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH_n_PATT_MODE_set", value, 3);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_PATT_MODE_set", channel, value );

    /* ((0x00000004 + (channel) * 0x0000000c) bits 3:2) field CH_n_PATT_MODE of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                               h_ptr,
                                                               channel,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_MSK,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_PATT_MODE_get( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_PATT_MODE_get( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_PATT_MODE_get", channel, 3);
    /* ((0x00000004 + (channel) * 0x0000000c) bits 3:2) field CH_n_PATT_MODE of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_MSK) >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_PATT_MODE_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_range_CH_n_PATT_MODE_set( cbr_fsgm_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32  channel,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_range_CH_n_PATT_MODE_set( cbr_fsgm_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32  channel,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_range_CH_n_PATT_MODE_set", channel, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbr_fsgm_field_range_CH_n_PATT_MODE_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbr_fsgm_field_range_CH_n_PATT_MODE_set", stop_bit, 1 );
    IOLOG( "%s <= channel=%d start_bit=%d stop_bit=%d 0x%08x", "cbr_fsgm_field_range_CH_n_PATT_MODE_set", channel, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000004 + (channel) * 0x0000000c) bits 3:2) field CH_n_PATT_MODE of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
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
        /* ((0x00000004 + (channel) * 0x0000000c) bits 3:2) field CH_n_PATT_MODE of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
        cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                                   h_ptr,
                                                                   channel,
                                                                   subfield_mask << (CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_OFF + subfield_offset),
                                                                   CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_OFF + subfield_offset,
                                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 cbr_fsgm_field_range_CH_n_PATT_MODE_get( cbr_fsgm_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32  channel,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_range_CH_n_PATT_MODE_get( cbr_fsgm_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32  channel,
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

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_range_CH_n_PATT_MODE_get", channel, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbr_fsgm_field_range_CH_n_PATT_MODE_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbr_fsgm_field_range_CH_n_PATT_MODE_get", stop_bit, 1 );
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
    /* ((0x00000004 + (channel) * 0x0000000c) bits 3:2) field CH_n_PATT_MODE of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    field_value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_MSK)
                  >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_MSK, CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_PATT_MODE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> channel=%d start_bit=%d stop_bit=%d 0x%08x", "cbr_fsgm_field_range_CH_n_PATT_MODE_get", channel, start_bit, stop_bit, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_LOS_PROP_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32  channel,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_LOS_PROP_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                         cbr_fsgm_handle_t *h_ptr,
                                                         UINT32  channel,
                                                         UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_LOS_PROP_ENA_set", channel, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH_n_LOS_PROP_ENA_set", value, 1);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_LOS_PROP_ENA_set", channel, value );

    /* ((0x00000004 + (channel) * 0x0000000c) bits 7) field CH_n_LOS_PROP_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                               h_ptr,
                                                               channel,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_PROP_ENA_MSK,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_PROP_ENA_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_LOS_PROP_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_LOS_PROP_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                           cbr_fsgm_handle_t *h_ptr,
                                                           UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_LOS_PROP_ENA_get", channel, 3);
    /* ((0x00000004 + (channel) * 0x0000000c) bits 7) field CH_n_LOS_PROP_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_PROP_ENA_MSK) >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_PROP_ENA_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_LOS_PROP_ENA_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_LOS_INS_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32  channel,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_LOS_INS_ENA_set( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32  channel,
                                                        UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_LOS_INS_ENA_set", channel, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH_n_LOS_INS_ENA_set", value, 1);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_LOS_INS_ENA_set", channel, value );

    /* ((0x00000004 + (channel) * 0x0000000c) bits 5) field CH_n_LOS_INS_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_field_set( b_ptr,
                                                               h_ptr,
                                                               channel,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_INS_ENA_MSK,
                                                               CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_INS_ENA_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_LOS_INS_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_LOS_INS_ENA_get( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_LOS_INS_ENA_get", channel, 3);
    /* ((0x00000004 + (channel) * 0x0000000c) bits 5) field CH_n_LOS_INS_ENA of register PMC_CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_CFG_REG_by_channel_array_read( b_ptr,
                                                                      h_ptr,
                                                                      channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_INS_ENA_MSK) >> CBR_FSGM_REG_CHANNEL_0_CFG_REG_BY_CHANNEL_BIT_CH_N_LOS_INS_ENA_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_LOS_INS_ENA_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_GAP_DENOM_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32  channel,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_GAP_DENOM_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32  channel,
                                                      UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_GAP_DENOM_set", channel, 3);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_GAP_DENOM_set", channel, value );

    /* ((0x0000000c + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_DENOM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_field_set( b_ptr,
                                                                       h_ptr,
                                                                       channel,
                                                                       CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_MSK,
                                                                       CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_OFF,
                                                                       value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_GAP_DENOM_get( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_GAP_DENOM_get( cbr_fsgm_buffer_t *b_ptr,
                                                        cbr_fsgm_handle_t *h_ptr,
                                                        UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_GAP_DENOM_get", channel, 3);
    /* ((0x0000000c + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_DENOM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_read( b_ptr,
                                                                              h_ptr,
                                                                              channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_MSK) >> CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_GAP_DENOM_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_range_CH_n_GAP_DENOM_set( cbr_fsgm_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32  channel,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_range_CH_n_GAP_DENOM_set( cbr_fsgm_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32  channel,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit,
                                                            UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_range_CH_n_GAP_DENOM_set", channel, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbr_fsgm_field_range_CH_n_GAP_DENOM_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbr_fsgm_field_range_CH_n_GAP_DENOM_set", stop_bit, 31 );
    IOLOG( "%s <= channel=%d start_bit=%d stop_bit=%d 0x%08x", "cbr_fsgm_field_range_CH_n_GAP_DENOM_set", channel, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x0000000c + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_DENOM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL index channel=0..3 */
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
        /* ((0x0000000c + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_DENOM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL index channel=0..3 */
        cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_field_set( b_ptr,
                                                                           h_ptr,
                                                                           channel,
                                                                           subfield_mask << (CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_OFF + subfield_offset),
                                                                           CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 cbr_fsgm_field_range_CH_n_GAP_DENOM_get( cbr_fsgm_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32  channel,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_range_CH_n_GAP_DENOM_get( cbr_fsgm_buffer_t *b_ptr,
                                                              cbr_fsgm_handle_t *h_ptr,
                                                              UINT32  channel,
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

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_range_CH_n_GAP_DENOM_get", channel, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbr_fsgm_field_range_CH_n_GAP_DENOM_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbr_fsgm_field_range_CH_n_GAP_DENOM_get", stop_bit, 31 );
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
    /* ((0x0000000c + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_DENOM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_D_REG_by_channel_array_read( b_ptr,
                                                                              h_ptr,
                                                                              channel);
    field_value = (reg_value & CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_MSK)
                  >> CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_MSK, CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_D_REG_BY_CHANNEL_BIT_CH_N_GAP_DENOM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> channel=%d start_bit=%d stop_bit=%d 0x%08x", "cbr_fsgm_field_range_CH_n_GAP_DENOM_get", channel, start_bit, stop_bit, value );

    return value;
}
static INLINE void cbr_fsgm_field_CH_n_GAP_NUM_set( cbr_fsgm_buffer_t *b_ptr,
                                                    cbr_fsgm_handle_t *h_ptr,
                                                    UINT32  channel,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH_n_GAP_NUM_set( cbr_fsgm_buffer_t *b_ptr,
                                                    cbr_fsgm_handle_t *h_ptr,
                                                    UINT32  channel,
                                                    UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_GAP_NUM_set", channel, 3);
    IOLOG( "%s <= channel=%d 0x%08x", "cbr_fsgm_field_CH_n_GAP_NUM_set", channel, value );

    /* ((0x00000008 + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_NUM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL index channel=0..3 */
    cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_field_set( b_ptr,
                                                                       h_ptr,
                                                                       channel,
                                                                       CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_MSK,
                                                                       CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_OFF,
                                                                       value);
}

static INLINE UINT32 cbr_fsgm_field_CH_n_GAP_NUM_get( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32  channel ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH_n_GAP_NUM_get( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32  channel )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_CH_n_GAP_NUM_get", channel, 3);
    /* ((0x00000008 + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_NUM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_read( b_ptr,
                                                                              h_ptr,
                                                                              channel);
    value = (reg_value & CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_MSK) >> CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_OFF;
    IOLOG( "%s -> channel=%d 0x%08x", "cbr_fsgm_field_CH_n_GAP_NUM_get", channel, value );

    return value;
}
static INLINE void cbr_fsgm_field_range_CH_n_GAP_NUM_set( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32  channel,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_range_CH_n_GAP_NUM_set( cbr_fsgm_buffer_t *b_ptr,
                                                          cbr_fsgm_handle_t *h_ptr,
                                                          UINT32  channel,
                                                          UINT32 start_bit,
                                                          UINT32 stop_bit,
                                                          UINT32 value )
{
    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_range_CH_n_GAP_NUM_set", channel, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbr_fsgm_field_range_CH_n_GAP_NUM_set", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbr_fsgm_field_range_CH_n_GAP_NUM_set", stop_bit, 31 );
    IOLOG( "%s <= channel=%d start_bit=%d stop_bit=%d 0x%08x", "cbr_fsgm_field_range_CH_n_GAP_NUM_set", channel, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00000008 + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_NUM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL index channel=0..3 */
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
        /* ((0x00000008 + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_NUM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL index channel=0..3 */
        cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_field_set( b_ptr,
                                                                           h_ptr,
                                                                           channel,
                                                                           subfield_mask << (CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_OFF + subfield_offset),
                                                                           CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_OFF + subfield_offset,
                                                                           value >> subfield_shift);
    }
}

static INLINE UINT32 cbr_fsgm_field_range_CH_n_GAP_NUM_get( cbr_fsgm_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32  channel,
                                                            UINT32 start_bit,
                                                            UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_range_CH_n_GAP_NUM_get( cbr_fsgm_buffer_t *b_ptr,
                                                            cbr_fsgm_handle_t *h_ptr,
                                                            UINT32  channel,
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

    if (channel > 3)
        IO_RANGE_CHECK("%s channel is %d but max is %d", "cbr_fsgm_field_range_CH_n_GAP_NUM_get", channel, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbr_fsgm_field_range_CH_n_GAP_NUM_get", stop_bit, start_bit );
    if (stop_bit > 31) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbr_fsgm_field_range_CH_n_GAP_NUM_get", stop_bit, 31 );
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
    /* ((0x00000008 + (channel) * 0x0000000c) bits 31:0) field CH_n_GAP_NUM of register PMC_CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL index channel=0..3 */
    reg_value = cbr_fsgm_reg_CHANNEL_0_GAP_RATIO_N_REG_by_channel_array_read( b_ptr,
                                                                              h_ptr,
                                                                              channel);
    field_value = (reg_value & CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_MSK)
                  >> CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_MSK, CBR_FSGM_REG_CHANNEL_0_GAP_RATIO_N_REG_BY_CHANNEL_BIT_CH_N_GAP_NUM_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> channel=%d start_bit=%d stop_bit=%d 0x%08x", "cbr_fsgm_field_range_CH_n_GAP_NUM_get", channel, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_grouped_by_channel
 * ==================================================================================
 */
static INLINE void cbr_fsgm_field_CH3_PN11_DET_E_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH3_PN11_DET_E_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH3_PN11_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_CH3_PN11_DET_E_set", value );

    /* (0x00000034 bits 3) field CH3_PN11_DET_E of register PMC_CBR_FSGM_REG_INT_EN_REG */
    cbr_fsgm_reg_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       CBR_FSGM_REG_INT_EN_REG_BIT_CH3_PN11_DET_E_MSK,
                                       CBR_FSGM_REG_INT_EN_REG_BIT_CH3_PN11_DET_E_OFF,
                                       value);
}

static INLINE UINT32 cbr_fsgm_field_CH3_PN11_DET_E_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH3_PN11_DET_E_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 3) field CH3_PN11_DET_E of register PMC_CBR_FSGM_REG_INT_EN_REG */
    reg_value = cbr_fsgm_reg_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_EN_REG_BIT_CH3_PN11_DET_E_MSK) >> CBR_FSGM_REG_INT_EN_REG_BIT_CH3_PN11_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH3_PN11_DET_E_get", value );

    return value;
}
static INLINE void cbr_fsgm_field_CH2_PN11_DET_E_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH2_PN11_DET_E_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH2_PN11_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_CH2_PN11_DET_E_set", value );

    /* (0x00000034 bits 2) field CH2_PN11_DET_E of register PMC_CBR_FSGM_REG_INT_EN_REG */
    cbr_fsgm_reg_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       CBR_FSGM_REG_INT_EN_REG_BIT_CH2_PN11_DET_E_MSK,
                                       CBR_FSGM_REG_INT_EN_REG_BIT_CH2_PN11_DET_E_OFF,
                                       value);
}

static INLINE UINT32 cbr_fsgm_field_CH2_PN11_DET_E_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH2_PN11_DET_E_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 2) field CH2_PN11_DET_E of register PMC_CBR_FSGM_REG_INT_EN_REG */
    reg_value = cbr_fsgm_reg_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_EN_REG_BIT_CH2_PN11_DET_E_MSK) >> CBR_FSGM_REG_INT_EN_REG_BIT_CH2_PN11_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH2_PN11_DET_E_get", value );

    return value;
}
static INLINE void cbr_fsgm_field_CH1_PN11_DET_E_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH1_PN11_DET_E_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH1_PN11_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_CH1_PN11_DET_E_set", value );

    /* (0x00000034 bits 1) field CH1_PN11_DET_E of register PMC_CBR_FSGM_REG_INT_EN_REG */
    cbr_fsgm_reg_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       CBR_FSGM_REG_INT_EN_REG_BIT_CH1_PN11_DET_E_MSK,
                                       CBR_FSGM_REG_INT_EN_REG_BIT_CH1_PN11_DET_E_OFF,
                                       value);
}

static INLINE UINT32 cbr_fsgm_field_CH1_PN11_DET_E_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH1_PN11_DET_E_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 1) field CH1_PN11_DET_E of register PMC_CBR_FSGM_REG_INT_EN_REG */
    reg_value = cbr_fsgm_reg_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_EN_REG_BIT_CH1_PN11_DET_E_MSK) >> CBR_FSGM_REG_INT_EN_REG_BIT_CH1_PN11_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH1_PN11_DET_E_get", value );

    return value;
}
static INLINE void cbr_fsgm_field_CH0_PN11_DET_E_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH0_PN11_DET_E_set( cbr_fsgm_buffer_t *b_ptr,
                                                      cbr_fsgm_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH0_PN11_DET_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_CH0_PN11_DET_E_set", value );

    /* (0x00000034 bits 0) field CH0_PN11_DET_E of register PMC_CBR_FSGM_REG_INT_EN_REG */
    cbr_fsgm_reg_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       CBR_FSGM_REG_INT_EN_REG_BIT_CH0_PN11_DET_E_MSK,
                                       CBR_FSGM_REG_INT_EN_REG_BIT_CH0_PN11_DET_E_OFF,
                                       value);
}

static INLINE UINT32 cbr_fsgm_field_CH0_PN11_DET_E_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH0_PN11_DET_E_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000034 bits 0) field CH0_PN11_DET_E of register PMC_CBR_FSGM_REG_INT_EN_REG */
    reg_value = cbr_fsgm_reg_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_EN_REG_BIT_CH0_PN11_DET_E_MSK) >> CBR_FSGM_REG_INT_EN_REG_BIT_CH0_PN11_DET_E_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH0_PN11_DET_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_grouped_by_channel
 * ==================================================================================
 */
static INLINE void cbr_fsgm_field_CH3_PN11_DET_I_set_to_clear( cbr_fsgm_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH3_PN11_DET_I_set_to_clear( cbr_fsgm_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH3_PN11_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_CH3_PN11_DET_I_set_to_clear", value );

    /* (0x00000038 bits 3) field CH3_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    cbr_fsgm_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH3_PN11_DET_I_MSK,
                                                               CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH3_PN11_DET_I_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH3_PN11_DET_I_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH3_PN11_DET_I_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 3) field CH3_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    reg_value = cbr_fsgm_reg_INT_INDICATION_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH3_PN11_DET_I_MSK) >> CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH3_PN11_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH3_PN11_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH3_PN11_DET_I_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH3_PN11_DET_I_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "cbr_fsgm_field_CH3_PN11_DET_I_poll", value );

    /* (0x00000038 bits 3) field CH3_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    return cbr_fsgm_reg_INT_INDICATION_REG_poll( b_ptr,
                                                 h_ptr,
                                                 CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH3_PN11_DET_I_MSK,
                                                 (value<<CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH3_PN11_DET_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void cbr_fsgm_field_CH2_PN11_DET_I_set_to_clear( cbr_fsgm_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH2_PN11_DET_I_set_to_clear( cbr_fsgm_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH2_PN11_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_CH2_PN11_DET_I_set_to_clear", value );

    /* (0x00000038 bits 2) field CH2_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    cbr_fsgm_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH2_PN11_DET_I_MSK,
                                                               CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH2_PN11_DET_I_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH2_PN11_DET_I_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH2_PN11_DET_I_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 2) field CH2_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    reg_value = cbr_fsgm_reg_INT_INDICATION_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH2_PN11_DET_I_MSK) >> CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH2_PN11_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH2_PN11_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH2_PN11_DET_I_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH2_PN11_DET_I_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "cbr_fsgm_field_CH2_PN11_DET_I_poll", value );

    /* (0x00000038 bits 2) field CH2_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    return cbr_fsgm_reg_INT_INDICATION_REG_poll( b_ptr,
                                                 h_ptr,
                                                 CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH2_PN11_DET_I_MSK,
                                                 (value<<CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH2_PN11_DET_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void cbr_fsgm_field_CH1_PN11_DET_I_set_to_clear( cbr_fsgm_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH1_PN11_DET_I_set_to_clear( cbr_fsgm_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH1_PN11_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_CH1_PN11_DET_I_set_to_clear", value );

    /* (0x00000038 bits 1) field CH1_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    cbr_fsgm_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH1_PN11_DET_I_MSK,
                                                               CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH1_PN11_DET_I_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH1_PN11_DET_I_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH1_PN11_DET_I_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 1) field CH1_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    reg_value = cbr_fsgm_reg_INT_INDICATION_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH1_PN11_DET_I_MSK) >> CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH1_PN11_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH1_PN11_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH1_PN11_DET_I_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH1_PN11_DET_I_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "cbr_fsgm_field_CH1_PN11_DET_I_poll", value );

    /* (0x00000038 bits 1) field CH1_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    return cbr_fsgm_reg_INT_INDICATION_REG_poll( b_ptr,
                                                 h_ptr,
                                                 CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH1_PN11_DET_I_MSK,
                                                 (value<<CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH1_PN11_DET_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void cbr_fsgm_field_CH0_PN11_DET_I_set_to_clear( cbr_fsgm_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_fsgm_field_CH0_PN11_DET_I_set_to_clear( cbr_fsgm_buffer_t *b_ptr,
                                                               cbr_fsgm_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_fsgm_field_CH0_PN11_DET_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_fsgm_field_CH0_PN11_DET_I_set_to_clear", value );

    /* (0x00000038 bits 0) field CH0_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    cbr_fsgm_reg_INT_INDICATION_REG_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH0_PN11_DET_I_MSK,
                                                               CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH0_PN11_DET_I_OFF,
                                                               value);
}

static INLINE UINT32 cbr_fsgm_field_CH0_PN11_DET_I_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH0_PN11_DET_I_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000038 bits 0) field CH0_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    reg_value = cbr_fsgm_reg_INT_INDICATION_REG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH0_PN11_DET_I_MSK) >> CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH0_PN11_DET_I_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH0_PN11_DET_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH0_PN11_DET_I_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH0_PN11_DET_I_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "cbr_fsgm_field_CH0_PN11_DET_I_poll", value );

    /* (0x00000038 bits 0) field CH0_PN11_DET_I of register PMC_CBR_FSGM_REG_INT_INDICATION_REG */
    return cbr_fsgm_reg_INT_INDICATION_REG_poll( b_ptr,
                                                 h_ptr,
                                                 CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH0_PN11_DET_I_MSK,
                                                 (value<<CBR_FSGM_REG_INT_INDICATION_REG_BIT_CH0_PN11_DET_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value_grouped_by_channel
 * ==================================================================================
 */
static INLINE UINT32 cbr_fsgm_field_CH3_PN11_DET_V_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH3_PN11_DET_V_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 3) field CH3_PN11_DET_V of register PMC_CBR_FSGM_REG_INT_STAT_REG */
    reg_value = cbr_fsgm_reg_INT_STAT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_STAT_REG_BIT_CH3_PN11_DET_V_MSK) >> CBR_FSGM_REG_INT_STAT_REG_BIT_CH3_PN11_DET_V_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH3_PN11_DET_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH3_PN11_DET_V_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH3_PN11_DET_V_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "cbr_fsgm_field_CH3_PN11_DET_V_poll", value );

    /* (0x0000003c bits 3) field CH3_PN11_DET_V of register PMC_CBR_FSGM_REG_INT_STAT_REG */
    return cbr_fsgm_reg_INT_STAT_REG_poll( b_ptr,
                                           h_ptr,
                                           CBR_FSGM_REG_INT_STAT_REG_BIT_CH3_PN11_DET_V_MSK,
                                           (value<<CBR_FSGM_REG_INT_STAT_REG_BIT_CH3_PN11_DET_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 cbr_fsgm_field_CH2_PN11_DET_V_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH2_PN11_DET_V_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 2) field CH2_PN11_DET_V of register PMC_CBR_FSGM_REG_INT_STAT_REG */
    reg_value = cbr_fsgm_reg_INT_STAT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_STAT_REG_BIT_CH2_PN11_DET_V_MSK) >> CBR_FSGM_REG_INT_STAT_REG_BIT_CH2_PN11_DET_V_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH2_PN11_DET_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH2_PN11_DET_V_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH2_PN11_DET_V_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "cbr_fsgm_field_CH2_PN11_DET_V_poll", value );

    /* (0x0000003c bits 2) field CH2_PN11_DET_V of register PMC_CBR_FSGM_REG_INT_STAT_REG */
    return cbr_fsgm_reg_INT_STAT_REG_poll( b_ptr,
                                           h_ptr,
                                           CBR_FSGM_REG_INT_STAT_REG_BIT_CH2_PN11_DET_V_MSK,
                                           (value<<CBR_FSGM_REG_INT_STAT_REG_BIT_CH2_PN11_DET_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 cbr_fsgm_field_CH1_PN11_DET_V_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH1_PN11_DET_V_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 1) field CH1_PN11_DET_V of register PMC_CBR_FSGM_REG_INT_STAT_REG */
    reg_value = cbr_fsgm_reg_INT_STAT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_STAT_REG_BIT_CH1_PN11_DET_V_MSK) >> CBR_FSGM_REG_INT_STAT_REG_BIT_CH1_PN11_DET_V_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH1_PN11_DET_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH1_PN11_DET_V_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH1_PN11_DET_V_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "cbr_fsgm_field_CH1_PN11_DET_V_poll", value );

    /* (0x0000003c bits 1) field CH1_PN11_DET_V of register PMC_CBR_FSGM_REG_INT_STAT_REG */
    return cbr_fsgm_reg_INT_STAT_REG_poll( b_ptr,
                                           h_ptr,
                                           CBR_FSGM_REG_INT_STAT_REG_BIT_CH1_PN11_DET_V_MSK,
                                           (value<<CBR_FSGM_REG_INT_STAT_REG_BIT_CH1_PN11_DET_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 cbr_fsgm_field_CH0_PN11_DET_V_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_fsgm_field_CH0_PN11_DET_V_get( cbr_fsgm_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000003c bits 0) field CH0_PN11_DET_V of register PMC_CBR_FSGM_REG_INT_STAT_REG */
    reg_value = cbr_fsgm_reg_INT_STAT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_FSGM_REG_INT_STAT_REG_BIT_CH0_PN11_DET_V_MSK) >> CBR_FSGM_REG_INT_STAT_REG_BIT_CH0_PN11_DET_V_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_fsgm_field_CH0_PN11_DET_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH0_PN11_DET_V_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_fsgm_field_CH0_PN11_DET_V_poll( cbr_fsgm_buffer_t *b_ptr,
                                                                       cbr_fsgm_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "cbr_fsgm_field_CH0_PN11_DET_V_poll", value );

    /* (0x0000003c bits 0) field CH0_PN11_DET_V of register PMC_CBR_FSGM_REG_INT_STAT_REG */
    return cbr_fsgm_reg_INT_STAT_REG_poll( b_ptr,
                                           h_ptr,
                                           CBR_FSGM_REG_INT_STAT_REG_BIT_CH0_PN11_DET_V_MSK,
                                           (value<<CBR_FSGM_REG_INT_STAT_REG_BIT_CH0_PN11_DET_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBR_FSGM_IO_INLINE_H */
