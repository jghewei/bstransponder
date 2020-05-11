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
 *     and register accessor functions for the cbr_int block
 *****************************************************************************/
#ifndef _CBR_INT_IO_INLINE_H
#define _CBR_INT_IO_INLINE_H

#include "cbrc_api.h"
#include "cbr_int_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define CBR_INT_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for cbr_int
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
} cbr_int_buffer_t;
static INLINE void cbr_int_buffer_init( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void cbr_int_buffer_init( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "cbr_int_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void cbr_int_buffer_flush( cbr_int_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void cbr_int_buffer_flush( cbr_int_buffer_t *b_ptr )
{
    IOLOG( "cbr_int_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 cbr_int_reg_read( cbr_int_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 cbr_int_reg_read( cbr_int_buffer_t *b_ptr,
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
static INLINE void cbr_int_reg_write( cbr_int_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_write( cbr_int_buffer_t *b_ptr,
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

static INLINE void cbr_int_field_set( cbr_int_buffer_t *b_ptr,
                                      cbrc_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 mask,
                                      UINT32 unused_mask,
                                      UINT32 ofs,
                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_field_set( cbr_int_buffer_t *b_ptr,
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

static INLINE void cbr_int_action_on_write_field_set( cbr_int_buffer_t *b_ptr,
                                                      cbrc_handle_t *h_ptr,
                                                      UINT32 mem_type,
                                                      UINT32 reg,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_action_on_write_field_set( cbr_int_buffer_t *b_ptr,
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

static INLINE void cbr_int_burst_read( cbr_int_buffer_t *b_ptr,
                                       cbrc_handle_t *h_ptr,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void cbr_int_burst_read( cbr_int_buffer_t *b_ptr,
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

static INLINE void cbr_int_burst_write( cbr_int_buffer_t *b_ptr,
                                        cbrc_handle_t *h_ptr,
                                        UINT32 mem_type,
                                        UINT32 reg,
                                        UINT32 len,
                                        UINT32 *value ) ALWAYS_INLINE;
static INLINE void cbr_int_burst_write( cbr_int_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE cbr_int_poll( cbr_int_buffer_t *b_ptr,
                                                 cbrc_handle_t *h_ptr,
                                                 UINT32 mem_type,
                                                 UINT32 reg,
                                                 UINT32 mask,
                                                 UINT32 value,
                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                 UINT32 max_count,
                                                 UINT32 *num_failed_polls,
                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE cbr_int_poll( cbr_int_buffer_t *b_ptr,
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
 *  register access functions for cbr_int
 * ==================================================================================
 */

static INLINE void cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_write( cbr_int_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_write( cbr_int_buffer_t *b_ptr,
                                                                         cbrc_handle_t *h_ptr,
                                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_write", value );
    cbr_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG,
                       value);
}

static INLINE void cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_field_set( cbr_int_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_field_set( cbr_int_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32 mask,
                                                                             UINT32 ofs,
                                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_field_set", mask, ofs, value );
    cbr_int_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG,
                       mask,
                       PMC_CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_read( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_read( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbr_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG);

    IOLOG( "%s -> 0x%08x;", "cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_read", reg_value);
    return reg_value;
}

static INLINE void cbr_int_reg_CHANNELN_CONFIG_REG_array_write( cbr_int_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_CHANNELN_CONFIG_REG_array_write( cbr_int_buffer_t *b_ptr,
                                                                cbrc_handle_t *h_ptr,
                                                                UINT32  N,
                                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_int_reg_CHANNELN_CONFIG_REG_array_write", value );
    cbr_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CBR_INT_REG_CHANNELN_CONFIG_REG(N),
                       value);
}

static INLINE void cbr_int_reg_CHANNELN_CONFIG_REG_array_field_set( cbr_int_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_CHANNELN_CONFIG_REG_array_field_set( cbr_int_buffer_t *b_ptr,
                                                                    cbrc_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "cbr_int_reg_CHANNELN_CONFIG_REG_array_field_set", N, mask, ofs, value );
    cbr_int_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CBR_INT_REG_CHANNELN_CONFIG_REG(N),
                       mask,
                       PMC_CBR_INT_REG_CHANNELN_CONFIG_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cbr_int_reg_CHANNELN_CONFIG_REG_array_read( cbr_int_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_reg_CHANNELN_CONFIG_REG_array_read( cbr_int_buffer_t *b_ptr,
                                                                 cbrc_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 reg_value;

    reg_value = cbr_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CBR_INT_REG_CHANNELN_CONFIG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "cbr_int_reg_CHANNELN_CONFIG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_write( cbr_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_write( cbr_int_buffer_t *b_ptr,
                                                                            cbrc_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_write", value );
    cbr_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG(N),
                       value);
}

static INLINE void cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_field_set( cbr_int_buffer_t *b_ptr,
                                                                                cbrc_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_field_set( cbr_int_buffer_t *b_ptr,
                                                                                cbrc_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 mask,
                                                                                UINT32 ofs,
                                                                                UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_field_set", N, mask, ofs, value );
    cbr_int_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_CONFIG,
                       PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG(N),
                       mask,
                       PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_read( cbr_int_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_read( cbr_int_buffer_t *b_ptr,
                                                                             cbrc_handle_t *h_ptr,
                                                                             UINT32  N )
{
    UINT32 reg_value;

    reg_value = cbr_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_CONFIG,
                                  PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void cbr_int_reg_CHANNEL_UPDATE_REG_write( cbr_int_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_CHANNEL_UPDATE_REG_write( cbr_int_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "cbr_int_reg_CHANNEL_UPDATE_REG_write", value );
    cbr_int_reg_write( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_CBR_INT_REG_CHANNEL_UPDATE_REG,
                       value);
}

static INLINE void cbr_int_reg_CHANNEL_UPDATE_REG_field_set( cbr_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_reg_CHANNEL_UPDATE_REG_field_set( cbr_int_buffer_t *b_ptr,
                                                             cbrc_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "cbr_int_reg_CHANNEL_UPDATE_REG_field_set", mask, ofs, value );
    cbr_int_field_set( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_CBR_INT_REG_CHANNEL_UPDATE_REG,
                       mask,
                       PMC_CBR_INT_REG_CHANNEL_UPDATE_REG_UNUSED_MASK,
                       ofs,
                       value);

}

static INLINE UINT32 cbr_int_reg_CHANNEL_UPDATE_REG_read( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_reg_CHANNEL_UPDATE_REG_read( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = cbr_int_reg_read( b_ptr,
                                  h_ptr,
                                  MEM_TYPE_STATUS,
                                  PMC_CBR_INT_REG_CHANNEL_UPDATE_REG);

    IOLOG( "%s -> 0x%08x;", "cbr_int_reg_CHANNEL_UPDATE_REG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void cbr_int_field_CBR_INT_RESET_set( cbr_int_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_field_CBR_INT_RESET_set( cbr_int_buffer_t *b_ptr,
                                                    cbrc_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_int_field_CBR_INT_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_int_field_CBR_INT_RESET_set", value );

    /* (0x00020040 bits 0) field CBR_INT_RESET of register PMC_CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG */
    cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_field_set( b_ptr,
                                                              h_ptr,
                                                              CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG_BIT_CBR_INT_RESET_MSK,
                                                              CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG_BIT_CBR_INT_RESET_OFF,
                                                              value);
}

static INLINE UINT32 cbr_int_field_CBR_INT_RESET_get( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_field_CBR_INT_RESET_get( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00020040 bits 0) field CBR_INT_RESET of register PMC_CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG */
    reg_value = cbr_int_reg_CBR_INTERLEAVER_GENERAL_CONFIG_REG_read( b_ptr,
                                                                     h_ptr);
    value = (reg_value & CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG_BIT_CBR_INT_RESET_MSK) >> CBR_INT_REG_CBR_INTERLEAVER_GENERAL_CONFIG_REG_BIT_CBR_INT_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_int_field_CBR_INT_RESET_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size12
 * ==================================================================================
 */
static INLINE void cbr_int_field_CHN_ID_set( cbr_int_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_field_CHN_ID_set( cbr_int_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbr_int_field_CHN_ID_set", N, 11);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_int_field_CHN_ID_set", value, 15);
    IOLOG( "%s <= N=%d 0x%08x", "cbr_int_field_CHN_ID_set", N, value );

    /* ((0x00020010 + (N) * 4) bits 7:4) field CHN_ID of register PMC_CBR_INT_REG_CHANNELN_CONFIG_REG index N=0..11 */
    cbr_int_reg_CHANNELN_CONFIG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_MSK,
                                                     CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_OFF,
                                                     value);
}

static INLINE UINT32 cbr_int_field_CHN_ID_get( cbr_int_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_field_CHN_ID_get( cbr_int_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbr_int_field_CHN_ID_get", N, 11);
    /* ((0x00020010 + (N) * 4) bits 7:4) field CHN_ID of register PMC_CBR_INT_REG_CHANNELN_CONFIG_REG index N=0..11 */
    reg_value = cbr_int_reg_CHANNELN_CONFIG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_MSK) >> CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cbr_int_field_CHN_ID_get", N, value );

    return value;
}
static INLINE void cbr_int_field_range_CHN_ID_set( cbr_int_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_field_range_CHN_ID_set( cbr_int_buffer_t *b_ptr,
                                                   cbrc_handle_t *h_ptr,
                                                   UINT32  N,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbr_int_field_range_CHN_ID_set", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbr_int_field_range_CHN_ID_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbr_int_field_range_CHN_ID_set", stop_bit, 3 );
    IOLOG( "%s <= N=%d start_bit=%d stop_bit=%d 0x%08x", "cbr_int_field_range_CHN_ID_set", N, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* ((0x00020010 + (N) * 4) bits 7:4) field CHN_ID of register PMC_CBR_INT_REG_CHANNELN_CONFIG_REG index N=0..11 */
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
        /* ((0x00020010 + (N) * 4) bits 7:4) field CHN_ID of register PMC_CBR_INT_REG_CHANNELN_CONFIG_REG index N=0..11 */
        cbr_int_reg_CHANNELN_CONFIG_REG_array_field_set( b_ptr,
                                                         h_ptr,
                                                         N,
                                                         subfield_mask << (CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_OFF + subfield_offset),
                                                         CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 cbr_int_field_range_CHN_ID_get( cbr_int_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_field_range_CHN_ID_get( cbr_int_buffer_t *b_ptr,
                                                     cbrc_handle_t *h_ptr,
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

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbr_int_field_range_CHN_ID_get", N, 11);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "cbr_int_field_range_CHN_ID_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "cbr_int_field_range_CHN_ID_get", stop_bit, 3 );
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
    /* ((0x00020010 + (N) * 4) bits 7:4) field CHN_ID of register PMC_CBR_INT_REG_CHANNELN_CONFIG_REG index N=0..11 */
    reg_value = cbr_int_reg_CHANNELN_CONFIG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    field_value = (reg_value & CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_MSK)
                  >> CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_MSK, CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_ID_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "cbr_int_field_range_CHN_ID_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE void cbr_int_field_CHN_EN_set( cbr_int_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_field_CHN_EN_set( cbr_int_buffer_t *b_ptr,
                                             cbrc_handle_t *h_ptr,
                                             UINT32  N,
                                             UINT32 value )
{
    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbr_int_field_CHN_EN_set", N, 11);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_int_field_CHN_EN_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "cbr_int_field_CHN_EN_set", N, value );

    /* ((0x00020010 + (N) * 4) bits 0) field CHN_EN of register PMC_CBR_INT_REG_CHANNELN_CONFIG_REG index N=0..11 */
    cbr_int_reg_CHANNELN_CONFIG_REG_array_field_set( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_EN_MSK,
                                                     CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_EN_OFF,
                                                     value);
}

static INLINE UINT32 cbr_int_field_CHN_EN_get( cbr_int_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_field_CHN_EN_get( cbr_int_buffer_t *b_ptr,
                                               cbrc_handle_t *h_ptr,
                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 11)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbr_int_field_CHN_EN_get", N, 11);
    /* ((0x00020010 + (N) * 4) bits 0) field CHN_EN of register PMC_CBR_INT_REG_CHANNELN_CONFIG_REG index N=0..11 */
    reg_value = cbr_int_reg_CHANNELN_CONFIG_REG_array_read( b_ptr,
                                                            h_ptr,
                                                            N);
    value = (reg_value & CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_EN_MSK) >> CBR_INT_REG_CHANNELN_CONFIG_REG_BIT_CHN_EN_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cbr_int_field_CHN_EN_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size3
 * ==================================================================================
 */
static INLINE void cbr_int_field_MULTI_LANE_40G_SEL_set( cbr_int_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_field_MULTI_LANE_40G_SEL_set( cbr_int_buffer_t *b_ptr,
                                                         cbrc_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbr_int_field_MULTI_LANE_40G_SEL_set", N, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_int_field_MULTI_LANE_40G_SEL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "cbr_int_field_MULTI_LANE_40G_SEL_set", N, value );

    /* ((0x00020000 + (N) * 4) bits 0) field MULTI_LANE_40G_SEL of register PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG index N=0..2 */
    cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_field_set( b_ptr,
                                                                 h_ptr,
                                                                 N,
                                                                 CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_BIT_MULTI_LANE_40G_SEL_MSK,
                                                                 CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_BIT_MULTI_LANE_40G_SEL_OFF,
                                                                 value);
}

static INLINE UINT32 cbr_int_field_MULTI_LANE_40G_SEL_get( cbr_int_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_field_MULTI_LANE_40G_SEL_get( cbr_int_buffer_t *b_ptr,
                                                           cbrc_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 2)
        IO_RANGE_CHECK("%s N is %d but max is %d", "cbr_int_field_MULTI_LANE_40G_SEL_get", N, 2);
    /* ((0x00020000 + (N) * 4) bits 0) field MULTI_LANE_40G_SEL of register PMC_CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG index N=0..2 */
    reg_value = cbr_int_reg_MULTI_LANE_40G_GROUP_CONFIG_REG_array_read( b_ptr,
                                                                        h_ptr,
                                                                        N);
    value = (reg_value & CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_BIT_MULTI_LANE_40G_SEL_MSK) >> CBR_INT_REG_MULTI_LANE_40G_GROUP_CONFIG_REG_BIT_MULTI_LANE_40G_SEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "cbr_int_field_MULTI_LANE_40G_SEL_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_edge
 * ==================================================================================
 */
static INLINE void cbr_int_field_CH_UPDATE_set( cbr_int_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void cbr_int_field_CH_UPDATE_set( cbr_int_buffer_t *b_ptr,
                                                cbrc_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "cbr_int_field_CH_UPDATE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "cbr_int_field_CH_UPDATE_set", value );

    /* (0x0002000c bits 0) field CH_UPDATE of register PMC_CBR_INT_REG_CHANNEL_UPDATE_REG */
    cbr_int_reg_CHANNEL_UPDATE_REG_field_set( b_ptr,
                                              h_ptr,
                                              CBR_INT_REG_CHANNEL_UPDATE_REG_BIT_CH_UPDATE_MSK,
                                              CBR_INT_REG_CHANNEL_UPDATE_REG_BIT_CH_UPDATE_OFF,
                                              value);
}

static INLINE UINT32 cbr_int_field_CH_UPDATE_get( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 cbr_int_field_CH_UPDATE_get( cbr_int_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0002000c bits 0) field CH_UPDATE of register PMC_CBR_INT_REG_CHANNEL_UPDATE_REG */
    reg_value = cbr_int_reg_CHANNEL_UPDATE_REG_read(  b_ptr, h_ptr);
    value = (reg_value & CBR_INT_REG_CHANNEL_UPDATE_REG_BIT_CH_UPDATE_MSK) >> CBR_INT_REG_CHANNEL_UPDATE_REG_BIT_CH_UPDATE_OFF;
    IOLOG( "%s -> 0x%08x", "cbr_int_field_CH_UPDATE_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CBR_INT_IO_INLINE_H */
