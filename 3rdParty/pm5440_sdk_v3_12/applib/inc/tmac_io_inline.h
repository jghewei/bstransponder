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
 *     and register accessor functions for the tmac block
 *****************************************************************************/
#ifndef _TMAC_IO_INLINE_H
#define _TMAC_IO_INLINE_H

#include "enet_fege_loc.h"
#include "tmac_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TMAC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for tmac
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
} tmac_buffer_t;
static INLINE void tmac_buffer_init( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void tmac_buffer_init( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "tmac_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void tmac_buffer_flush( tmac_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void tmac_buffer_flush( tmac_buffer_t *b_ptr )
{
    IOLOG( "tmac_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 tmac_reg_read( tmac_buffer_t *b_ptr,
                                    enet_fege_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_read( tmac_buffer_t *b_ptr,
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
static INLINE void tmac_reg_write( tmac_buffer_t *b_ptr,
                                   enet_fege_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_write( tmac_buffer_t *b_ptr,
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

static INLINE void tmac_field_set( tmac_buffer_t *b_ptr,
                                   enet_fege_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_set( tmac_buffer_t *b_ptr,
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

static INLINE void tmac_action_on_write_field_set( tmac_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_action_on_write_field_set( tmac_buffer_t *b_ptr,
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

static INLINE void tmac_burst_read( tmac_buffer_t *b_ptr,
                                    enet_fege_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void tmac_burst_read( tmac_buffer_t *b_ptr,
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

static INLINE void tmac_burst_write( tmac_buffer_t *b_ptr,
                                     enet_fege_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void tmac_burst_write( tmac_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE tmac_poll( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE tmac_poll( tmac_buffer_t *b_ptr,
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
 *  register access functions for tmac
 * ==================================================================================
 */

static INLINE void tmac_reg_CFG_1_write( tmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_CFG_1_write( tmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_CFG_1_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_CFG_1,
                    value);
}

static INLINE void tmac_reg_CFG_1_field_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_CFG_1_field_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_CFG_1_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_CFG_1,
                    mask,
                    PMC_TMAC_REG_CFG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_CFG_1_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_CFG_1_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_CFG_1);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_CFG_1_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_CFG_2_write( tmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_CFG_2_write( tmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_CFG_2_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_CFG_2,
                    value);
}

static INLINE void tmac_reg_CFG_2_field_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_CFG_2_field_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_CFG_2_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_CFG_2,
                    mask,
                    PMC_TMAC_REG_CFG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_CFG_2_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_CFG_2_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_CFG_2);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_CFG_2_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_INTER_PACKET_GAP_CFG_2_write( tmac_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_INTER_PACKET_GAP_CFG_2_write( tmac_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_INTER_PACKET_GAP_CFG_2_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2,
                    value);
}

static INLINE void tmac_reg_INTER_PACKET_GAP_CFG_2_field_set( tmac_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_INTER_PACKET_GAP_CFG_2_field_set( tmac_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_INTER_PACKET_GAP_CFG_2_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2,
                    mask,
                    PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_INTER_PACKET_GAP_CFG_2_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_INTER_PACKET_GAP_CFG_2_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_INTER_PACKET_GAP_CFG_2_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_STATION_ADR_HIGH_WORD_CFG_write( tmac_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_STATION_ADR_HIGH_WORD_CFG_write( tmac_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_STATION_ADR_HIGH_WORD_CFG_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG,
                    value);
}

static INLINE void tmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set( tmac_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set( tmac_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG,
                    mask,
                    PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_STATION_ADR_HIGH_WORD_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_STATION_ADR_HIGH_WORD_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_STATION_ADR_HIGH_WORD_CFG_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write( tmac_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write( tmac_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG,
                    value);
}

static INLINE void tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set( tmac_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set( tmac_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG,
                    mask,
                    PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_STATION_ADR_LOW_WORD_CFG_write( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_STATION_ADR_LOW_WORD_CFG_write( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_STATION_ADR_LOW_WORD_CFG_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG,
                    value);
}

static INLINE void tmac_reg_STATION_ADR_LOW_WORD_CFG_field_set( tmac_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_STATION_ADR_LOW_WORD_CFG_field_set( tmac_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_STATION_ADR_LOW_WORD_CFG_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG,
                    mask,
                    PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_STATION_ADR_LOW_WORD_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_STATION_ADR_LOW_WORD_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_STATION_ADR_LOW_WORD_CFG_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_MAX_FRAME_LENGTH_CFG_write( tmac_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_MAX_FRAME_LENGTH_CFG_write( tmac_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_MAX_FRAME_LENGTH_CFG_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG,
                    value);
}

static INLINE void tmac_reg_MAX_FRAME_LENGTH_CFG_field_set( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_MAX_FRAME_LENGTH_CFG_field_set( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_MAX_FRAME_LENGTH_CFG_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG,
                    mask,
                    PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_MAX_FRAME_LENGTH_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_MAX_FRAME_LENGTH_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_MAX_FRAME_LENGTH_CFG_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_MIN_FRAME_LENGTH_CFG_write( tmac_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_MIN_FRAME_LENGTH_CFG_write( tmac_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_MIN_FRAME_LENGTH_CFG_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG,
                    value);
}

static INLINE void tmac_reg_MIN_FRAME_LENGTH_CFG_field_set( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_MIN_FRAME_LENGTH_CFG_field_set( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_MIN_FRAME_LENGTH_CFG_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG,
                    mask,
                    PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_MIN_FRAME_LENGTH_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_MIN_FRAME_LENGTH_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_MIN_FRAME_LENGTH_CFG_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_PAUSE_FRAME_TIME_CFG_write( tmac_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_PAUSE_FRAME_TIME_CFG_write( tmac_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_PAUSE_FRAME_TIME_CFG_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG,
                    value);
}

static INLINE void tmac_reg_PAUSE_FRAME_TIME_CFG_field_set( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_PAUSE_FRAME_TIME_CFG_field_set( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_PAUSE_FRAME_TIME_CFG_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG,
                    mask,
                    PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_PAUSE_FRAME_TIME_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_PAUSE_FRAME_TIME_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_PAUSE_FRAME_TIME_CFG_read", reg_value);
    return reg_value;
}

static INLINE void tmac_reg_PAUSE_FRAME_INTERVAL_CFG_write( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_PAUSE_FRAME_INTERVAL_CFG_write( tmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "tmac_reg_PAUSE_FRAME_INTERVAL_CFG_write", value );
    tmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG,
                    value);
}

static INLINE void tmac_reg_PAUSE_FRAME_INTERVAL_CFG_field_set( tmac_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_reg_PAUSE_FRAME_INTERVAL_CFG_field_set( tmac_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "tmac_reg_PAUSE_FRAME_INTERVAL_CFG_field_set", mask, ofs, value );
    tmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG,
                    mask,
                    PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 tmac_reg_PAUSE_FRAME_INTERVAL_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_reg_PAUSE_FRAME_INTERVAL_CFG_read( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = tmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG);

    IOLOG( "%s -> 0x%08x;", "tmac_reg_PAUSE_FRAME_INTERVAL_CFG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void tmac_field_CRC_CK_set( tmac_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_CRC_CK_set( tmac_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_CRC_CK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_CRC_CK_set", value );

    /* (0x00002000 bits 14) field CRC_CK of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_CRC_CK_MSK,
                              TMAC_REG_CFG_1_BIT_CRC_CK_OFF,
                              value);
}

static INLINE UINT32 tmac_field_CRC_CK_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_CRC_CK_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 14) field CRC_CK of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_CRC_CK_MSK) >> TMAC_REG_CFG_1_BIT_CRC_CK_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_CRC_CK_get", value );

    return value;
}
static INLINE void tmac_field_PAD_ADD_EN_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_PAD_ADD_EN_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_PAD_ADD_EN_set", value, 3);
    IOLOG( "%s <= 0x%08x", "tmac_field_PAD_ADD_EN_set", value );

    /* (0x00002000 bits 13:12) bits 0:1 use field PAD_ADD_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_PAD_ADD_EN_MSK,
                              TMAC_REG_CFG_1_BIT_PAD_ADD_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_PAD_ADD_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_PAD_ADD_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 13:12) bits 0:1 use field PAD_ADD_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_PAD_ADD_EN_MSK) >> TMAC_REG_CFG_1_BIT_PAD_ADD_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_PAD_ADD_EN_get", value );

    return value;
}
static INLINE void tmac_field_range_PAD_ADD_EN_set( tmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_range_PAD_ADD_EN_set( tmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_PAD_ADD_EN_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_PAD_ADD_EN_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_PAD_ADD_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002000 bits 13:12) bits 0:1 use field PAD_ADD_EN of register PMC_TMAC_REG_CFG_1 */
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
        /* (0x00002000 bits 13:12) bits 0:1 use field PAD_ADD_EN of register PMC_TMAC_REG_CFG_1 */
        tmac_reg_CFG_1_field_set( b_ptr,
                                  h_ptr,
                                  subfield_mask << (TMAC_REG_CFG_1_BIT_PAD_ADD_EN_OFF + subfield_offset),
                                  TMAC_REG_CFG_1_BIT_PAD_ADD_EN_OFF + subfield_offset,
                                  value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_field_range_PAD_ADD_EN_get( tmac_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_range_PAD_ADD_EN_get( tmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_PAD_ADD_EN_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_PAD_ADD_EN_get", stop_bit, 1 );
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
    /* (0x00002000 bits 13:12) bits 0:1 use field PAD_ADD_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & TMAC_REG_CFG_1_BIT_PAD_ADD_EN_MSK)
                  >> TMAC_REG_CFG_1_BIT_PAD_ADD_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_CFG_1_BIT_PAD_ADD_EN_MSK, TMAC_REG_CFG_1_BIT_PAD_ADD_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_PAD_ADD_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tmac_field_CRC_ADD_EN_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_CRC_ADD_EN_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_CRC_ADD_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_CRC_ADD_EN_set", value );

    /* (0x00002000 bits 11) field CRC_ADD_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_CRC_ADD_EN_MSK,
                              TMAC_REG_CFG_1_BIT_CRC_ADD_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_CRC_ADD_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_CRC_ADD_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 11) field CRC_ADD_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_CRC_ADD_EN_MSK) >> TMAC_REG_CFG_1_BIT_CRC_ADD_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_CRC_ADD_EN_get", value );

    return value;
}
static INLINE void tmac_field_MIN_FRM_CK_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_MIN_FRM_CK_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_MIN_FRM_CK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_MIN_FRM_CK_set", value );

    /* (0x00002000 bits 10) field MIN_FRM_CK of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_MIN_FRM_CK_MSK,
                              TMAC_REG_CFG_1_BIT_MIN_FRM_CK_OFF,
                              value);
}

static INLINE UINT32 tmac_field_MIN_FRM_CK_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_MIN_FRM_CK_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 10) field MIN_FRM_CK of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_MIN_FRM_CK_MSK) >> TMAC_REG_CFG_1_BIT_MIN_FRM_CK_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_MIN_FRM_CK_get", value );

    return value;
}
static INLINE void tmac_field_TRUNC_EN_set( tmac_buffer_t *b_ptr,
                                            enet_fege_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_TRUNC_EN_set( tmac_buffer_t *b_ptr,
                                            enet_fege_handle_t *h_ptr,
                                            UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_TRUNC_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_TRUNC_EN_set", value );

    /* (0x00002000 bits 9) field TRUNC_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_TRUNC_EN_MSK,
                              TMAC_REG_CFG_1_BIT_TRUNC_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_TRUNC_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_TRUNC_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 9) field TRUNC_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_TRUNC_EN_MSK) >> TMAC_REG_CFG_1_BIT_TRUNC_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_TRUNC_EN_get", value );

    return value;
}
static INLINE void tmac_field_MAX_FRM_CK_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_MAX_FRM_CK_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_MAX_FRM_CK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_MAX_FRM_CK_set", value );

    /* (0x00002000 bits 8) field MAX_FRM_CK of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_MAX_FRM_CK_MSK,
                              TMAC_REG_CFG_1_BIT_MAX_FRM_CK_OFF,
                              value);
}

static INLINE UINT32 tmac_field_MAX_FRM_CK_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_MAX_FRM_CK_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 8) field MAX_FRM_CK of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_MAX_FRM_CK_MSK) >> TMAC_REG_CFG_1_BIT_MAX_FRM_CK_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_MAX_FRM_CK_get", value );

    return value;
}
static INLINE void tmac_field_EBUF_PAUSE_EN_set( tmac_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_EBUF_PAUSE_EN_set( tmac_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_EBUF_PAUSE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_EBUF_PAUSE_EN_set", value );

    /* (0x00002000 bits 7) field EBUF_PAUSE_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_EBUF_PAUSE_EN_MSK,
                              TMAC_REG_CFG_1_BIT_EBUF_PAUSE_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_EBUF_PAUSE_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_EBUF_PAUSE_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 7) field EBUF_PAUSE_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_EBUF_PAUSE_EN_MSK) >> TMAC_REG_CFG_1_BIT_EBUF_PAUSE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_EBUF_PAUSE_EN_get", value );

    return value;
}
static INLINE void tmac_field_IBUF_HTHR_EN_set( tmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_IBUF_HTHR_EN_set( tmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_IBUF_HTHR_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_IBUF_HTHR_EN_set", value );

    /* (0x00002000 bits 6) field IBUF_HTHR_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_IBUF_HTHR_EN_MSK,
                              TMAC_REG_CFG_1_BIT_IBUF_HTHR_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_IBUF_HTHR_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_IBUF_HTHR_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 6) field IBUF_HTHR_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_IBUF_HTHR_EN_MSK) >> TMAC_REG_CFG_1_BIT_IBUF_HTHR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_IBUF_HTHR_EN_get", value );

    return value;
}
static INLINE void tmac_field_XON_EN_set( tmac_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_XON_EN_set( tmac_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_XON_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_XON_EN_set", value );

    /* (0x00002000 bits 5) field XON_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_XON_EN_MSK,
                              TMAC_REG_CFG_1_BIT_XON_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_XON_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_XON_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 5) field XON_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_XON_EN_MSK) >> TMAC_REG_CFG_1_BIT_XON_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_XON_EN_get", value );

    return value;
}
static INLINE void tmac_field_RMAC_PAUSE_EN_set( tmac_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_RMAC_PAUSE_EN_set( tmac_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_RMAC_PAUSE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_RMAC_PAUSE_EN_set", value );

    /* (0x00002000 bits 4) field RMAC_PAUSE_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_RMAC_PAUSE_EN_MSK,
                              TMAC_REG_CFG_1_BIT_RMAC_PAUSE_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_RMAC_PAUSE_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_RMAC_PAUSE_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 4) field RMAC_PAUSE_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_RMAC_PAUSE_EN_MSK) >> TMAC_REG_CFG_1_BIT_RMAC_PAUSE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_RMAC_PAUSE_EN_get", value );

    return value;
}
static INLINE void tmac_field_EBUF_ERR_EN_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_EBUF_ERR_EN_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_EBUF_ERR_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_EBUF_ERR_EN_set", value );

    /* (0x00002000 bits 3) field EBUF_ERR_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_EBUF_ERR_EN_MSK,
                              TMAC_REG_CFG_1_BIT_EBUF_ERR_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_EBUF_ERR_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_EBUF_ERR_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 3) field EBUF_ERR_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_EBUF_ERR_EN_MSK) >> TMAC_REG_CFG_1_BIT_EBUF_ERR_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_EBUF_ERR_EN_get", value );

    return value;
}
static INLINE void tmac_field_TRANSPARENT_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_TRANSPARENT_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_TRANSPARENT_set", value );

    /* (0x00002000 bits 2) field TRANSPARENT of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_TRANSPARENT_MSK,
                              TMAC_REG_CFG_1_BIT_TRANSPARENT_OFF,
                              value);
}

static INLINE UINT32 tmac_field_TRANSPARENT_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_TRANSPARENT_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 2) field TRANSPARENT of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_TRANSPARENT_MSK) >> TMAC_REG_CFG_1_BIT_TRANSPARENT_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_TRANSPARENT_get", value );

    return value;
}
static INLINE void tmac_field_HW_STOP_EN_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_HW_STOP_EN_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_HW_STOP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_HW_STOP_EN_set", value );

    /* (0x00002000 bits 1) field HW_STOP_EN of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_HW_STOP_EN_MSK,
                              TMAC_REG_CFG_1_BIT_HW_STOP_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_HW_STOP_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_HW_STOP_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 1) field HW_STOP_EN of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_HW_STOP_EN_MSK) >> TMAC_REG_CFG_1_BIT_HW_STOP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_HW_STOP_EN_get", value );

    return value;
}
static INLINE void tmac_field_TMAC_ENBL_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_TMAC_ENBL_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_TMAC_ENBL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_TMAC_ENBL_set", value );

    /* (0x00002000 bits 0) field TMAC_ENBL of register PMC_TMAC_REG_CFG_1 */
    tmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_1_BIT_TMAC_ENBL_MSK,
                              TMAC_REG_CFG_1_BIT_TMAC_ENBL_OFF,
                              value);
}

static INLINE UINT32 tmac_field_TMAC_ENBL_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_TMAC_ENBL_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 0) field TMAC_ENBL of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_TMAC_ENBL_MSK) >> TMAC_REG_CFG_1_BIT_TMAC_ENBL_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_TMAC_ENBL_get", value );

    return value;
}
static INLINE void tmac_field_LATE_COL_EN_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_LATE_COL_EN_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_LATE_COL_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_LATE_COL_EN_set", value );

    /* (0x00002004 bits 7) field LATE_COL_EN of register PMC_TMAC_REG_CFG_2 */
    tmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_2_BIT_LATE_COL_EN_MSK,
                              TMAC_REG_CFG_2_BIT_LATE_COL_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_LATE_COL_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_LATE_COL_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002004 bits 7) field LATE_COL_EN of register PMC_TMAC_REG_CFG_2 */
    reg_value = tmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_2_BIT_LATE_COL_EN_MSK) >> TMAC_REG_CFG_2_BIT_LATE_COL_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_LATE_COL_EN_get", value );

    return value;
}
static INLINE void tmac_field_SFD_EN_set( tmac_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_SFD_EN_set( tmac_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_SFD_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_SFD_EN_set", value );

    /* (0x00002004 bits 4) field SFD_EN of register PMC_TMAC_REG_CFG_2 */
    tmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_2_BIT_SFD_EN_MSK,
                              TMAC_REG_CFG_2_BIT_SFD_EN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_SFD_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_SFD_EN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002004 bits 4) field SFD_EN of register PMC_TMAC_REG_CFG_2 */
    reg_value = tmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_2_BIT_SFD_EN_MSK) >> TMAC_REG_CFG_2_BIT_SFD_EN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_SFD_EN_get", value );

    return value;
}
static INLINE void tmac_field_PAMB_LEN_set( tmac_buffer_t *b_ptr,
                                            enet_fege_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_PAMB_LEN_set( tmac_buffer_t *b_ptr,
                                            enet_fege_handle_t *h_ptr,
                                            UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_PAMB_LEN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "tmac_field_PAMB_LEN_set", value );

    /* (0x00002004 bits 3:0) bits 0:3 use field PAMB_LEN of register PMC_TMAC_REG_CFG_2 */
    tmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              TMAC_REG_CFG_2_BIT_PAMB_LEN_MSK,
                              TMAC_REG_CFG_2_BIT_PAMB_LEN_OFF,
                              value);
}

static INLINE UINT32 tmac_field_PAMB_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_PAMB_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002004 bits 3:0) bits 0:3 use field PAMB_LEN of register PMC_TMAC_REG_CFG_2 */
    reg_value = tmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_2_BIT_PAMB_LEN_MSK) >> TMAC_REG_CFG_2_BIT_PAMB_LEN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_PAMB_LEN_get", value );

    return value;
}
static INLINE void tmac_field_range_PAMB_LEN_set( tmac_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_range_PAMB_LEN_set( tmac_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 start_bit,
                                                  UINT32 stop_bit,
                                                  UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_PAMB_LEN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_PAMB_LEN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_PAMB_LEN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002004 bits 3:0) bits 0:3 use field PAMB_LEN of register PMC_TMAC_REG_CFG_2 */
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
        /* (0x00002004 bits 3:0) bits 0:3 use field PAMB_LEN of register PMC_TMAC_REG_CFG_2 */
        tmac_reg_CFG_2_field_set( b_ptr,
                                  h_ptr,
                                  subfield_mask << (TMAC_REG_CFG_2_BIT_PAMB_LEN_OFF + subfield_offset),
                                  TMAC_REG_CFG_2_BIT_PAMB_LEN_OFF + subfield_offset,
                                  value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_field_range_PAMB_LEN_get( tmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_range_PAMB_LEN_get( tmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_PAMB_LEN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_PAMB_LEN_get", stop_bit, 3 );
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
    /* (0x00002004 bits 3:0) bits 0:3 use field PAMB_LEN of register PMC_TMAC_REG_CFG_2 */
    reg_value = tmac_reg_CFG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & TMAC_REG_CFG_2_BIT_PAMB_LEN_MSK)
                  >> TMAC_REG_CFG_2_BIT_PAMB_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_CFG_2_BIT_PAMB_LEN_MSK, TMAC_REG_CFG_2_BIT_PAMB_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_PAMB_LEN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tmac_field_FIPG1_LEN_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_FIPG1_LEN_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_FIPG1_LEN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "tmac_field_FIPG1_LEN_set", value );

    /* (0x00002008 bits 15:12) bits 0:3 use field FIPG1_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
    tmac_reg_INTER_PACKET_GAP_CFG_2_field_set( b_ptr,
                                               h_ptr,
                                               TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_MSK,
                                               TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_OFF,
                                               value);
}

static INLINE UINT32 tmac_field_FIPG1_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_FIPG1_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002008 bits 15:12) bits 0:3 use field FIPG1_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
    reg_value = tmac_reg_INTER_PACKET_GAP_CFG_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_MSK) >> TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_FIPG1_LEN_get", value );

    return value;
}
static INLINE void tmac_field_range_FIPG1_LEN_set( tmac_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_range_FIPG1_LEN_set( tmac_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_FIPG1_LEN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_FIPG1_LEN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_FIPG1_LEN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002008 bits 15:12) bits 0:3 use field FIPG1_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
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
        /* (0x00002008 bits 15:12) bits 0:3 use field FIPG1_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
        tmac_reg_INTER_PACKET_GAP_CFG_2_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_OFF + subfield_offset),
                                                   TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_field_range_FIPG1_LEN_get( tmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_range_FIPG1_LEN_get( tmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_FIPG1_LEN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_FIPG1_LEN_get", stop_bit, 3 );
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
    /* (0x00002008 bits 15:12) bits 0:3 use field FIPG1_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
    reg_value = tmac_reg_INTER_PACKET_GAP_CFG_2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_MSK)
                  >> TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_MSK, TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG1_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_FIPG1_LEN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tmac_field_FIPG2_LEN_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_FIPG2_LEN_set( tmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_FIPG2_LEN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "tmac_field_FIPG2_LEN_set", value );

    /* (0x00002008 bits 11:8) bits 0:3 use field FIPG2_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
    tmac_reg_INTER_PACKET_GAP_CFG_2_field_set( b_ptr,
                                               h_ptr,
                                               TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_MSK,
                                               TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_OFF,
                                               value);
}

static INLINE UINT32 tmac_field_FIPG2_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_FIPG2_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002008 bits 11:8) bits 0:3 use field FIPG2_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
    reg_value = tmac_reg_INTER_PACKET_GAP_CFG_2_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_MSK) >> TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_FIPG2_LEN_get", value );

    return value;
}
static INLINE void tmac_field_range_FIPG2_LEN_set( tmac_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_range_FIPG2_LEN_set( tmac_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 start_bit,
                                                   UINT32 stop_bit,
                                                   UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_FIPG2_LEN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_FIPG2_LEN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_FIPG2_LEN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002008 bits 11:8) bits 0:3 use field FIPG2_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
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
        /* (0x00002008 bits 11:8) bits 0:3 use field FIPG2_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
        tmac_reg_INTER_PACKET_GAP_CFG_2_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_OFF + subfield_offset),
                                                   TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_field_range_FIPG2_LEN_get( tmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_range_FIPG2_LEN_get( tmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_FIPG2_LEN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_FIPG2_LEN_get", stop_bit, 3 );
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
    /* (0x00002008 bits 11:8) bits 0:3 use field FIPG2_LEN of register PMC_TMAC_REG_INTER_PACKET_GAP_CFG_2 */
    reg_value = tmac_reg_INTER_PACKET_GAP_CFG_2_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_MSK)
                  >> TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_MSK, TMAC_REG_INTER_PACKET_GAP_CFG_2_BIT_FIPG2_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_FIPG2_LEN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tmac_lfield_STA_set( tmac_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void tmac_lfield_STA_set( tmac_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "tmac_lfield_STA_set", value[1] , value[0] );

    /* (0x0000200c bits 15:0) bits 32:47 use field STA of register PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG */
    tmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK,
                                                  TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF,
                                                  value[1]);

    /* (0x00002010 bits 15:0) bits 16:31 use field STA of register PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
    tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set( b_ptr,
                                                    h_ptr,
                                                    TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK,
                                                    TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF,
                                                    value[0]);

    /* (0x00002014 bits 15:0) bits 0:15 use field STA of register PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG */
    tmac_reg_STATION_ADR_LOW_WORD_CFG_field_set( b_ptr,
                                                 h_ptr,
                                                 TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK,
                                                 TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF,
                                                 value[0]);
}

static INLINE void tmac_lfield_STA_get( tmac_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void tmac_lfield_STA_get( tmac_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000200c bits 15:0) bits 32:47 use field STA of register PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG */
    reg_value = tmac_reg_STATION_ADR_HIGH_WORD_CFG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK) >> TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF;
    value[1] |= field_value;

    /* (0x00002010 bits 15:0) bits 16:31 use field STA of register PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
    reg_value = tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK) >> TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF;
    value[0] |= (field_value << 16);

    /* (0x00002014 bits 15:0) bits 0:15 use field STA of register PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG */
    reg_value = tmac_reg_STATION_ADR_LOW_WORD_CFG_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK) >> TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF;
    value[0] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "tmac_lfield_STA_get", value[1] , value[0] );

}
static INLINE void tmac_lfield_range_STA_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_lfield_range_STA_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_lfield_range_STA_set", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_lfield_range_STA_set", stop_bit, 47 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_lfield_range_STA_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000200c bits 15:0) bits 32:47 use field STA of register PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG */
    if ((start_bit <= 47) && (stop_bit >= 32)) {
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
        if (stop_bit < 47) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 47;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0000200c bits 15:0) bits 32:47 use field STA of register PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG */
        tmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF + subfield_offset),
                                                      TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002010 bits 15:0) bits 16:31 use field STA of register PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
    if ((start_bit <= 31) && (stop_bit >= 16)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 16) {
            subfield_offset = start_bit - 16;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 16 - start_bit;
            subfield_start_bit = 16;
        }
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00002010 bits 15:0) bits 16:31 use field STA of register PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
        tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF + subfield_offset),
                                                        TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002014 bits 15:0) bits 0:15 use field STA of register PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG */
    if (start_bit <= 15) {
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
        /* (0x00002014 bits 15:0) bits 0:15 use field STA of register PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG */
        tmac_reg_STATION_ADR_LOW_WORD_CFG_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF + subfield_offset),
                                                     TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_lfield_range_STA_get( tmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_lfield_range_STA_get( tmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_lfield_range_STA_get", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_lfield_range_STA_get", stop_bit, 47 );
    if ((start_bit <= 47) && (stop_bit >= 32)) {
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
        if (stop_bit < 47) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 47;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x0000200c bits 15:0) bits 32:47 use field STA of register PMC_TMAC_REG_STATION_ADR_HIGH_WORD_CFG */
        reg_value = tmac_reg_STATION_ADR_HIGH_WORD_CFG_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK)
                      >> TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK, TMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 31) && (stop_bit >= 16)) {
        UINT32 subfield_start_bit;
        UINT32 subfield_stop_bit;
        UINT32 subfield_offset;
        UINT32 subfield_shift;
        UINT32 subfield_num_bits;
        UINT32 subfield_mask;

        if (start_bit > 16) {
            subfield_offset = start_bit - 16;
            subfield_shift = 0;
            subfield_start_bit = start_bit;
        } else {
            subfield_offset = 0;
            subfield_shift = 16 - start_bit;
            subfield_start_bit = 16;
        }
        if (stop_bit < 31) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 31;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00002010 bits 15:0) bits 16:31 use field STA of register PMC_TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
        reg_value = tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK)
                      >> TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK, TMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if (start_bit <= 15) {
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
    /* (0x00002014 bits 15:0) bits 0:15 use field STA of register PMC_TMAC_REG_STATION_ADR_LOW_WORD_CFG */
        reg_value = tmac_reg_STATION_ADR_LOW_WORD_CFG_read( b_ptr,
                                                            h_ptr);
        field_value = (reg_value & TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK)
                      >> TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK, TMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_lfield_range_STA_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tmac_field_MAX_FRM_LEN_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_MAX_FRM_LEN_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_MAX_FRM_LEN_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tmac_field_MAX_FRM_LEN_set", value );

    /* (0x00002018 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG */
    tmac_reg_MAX_FRAME_LENGTH_CFG_field_set( b_ptr,
                                             h_ptr,
                                             TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK,
                                             TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF,
                                             value);
}

static INLINE UINT32 tmac_field_MAX_FRM_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_MAX_FRM_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002018 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG */
    reg_value = tmac_reg_MAX_FRAME_LENGTH_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK) >> TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_MAX_FRM_LEN_get", value );

    return value;
}
static INLINE void tmac_field_range_MAX_FRM_LEN_set( tmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_range_MAX_FRM_LEN_set( tmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_MAX_FRM_LEN_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_MAX_FRM_LEN_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_MAX_FRM_LEN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002018 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG */
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
        /* (0x00002018 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG */
        tmac_reg_MAX_FRAME_LENGTH_CFG_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF + subfield_offset),
                                                 TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_field_range_MAX_FRM_LEN_get( tmac_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_range_MAX_FRM_LEN_get( tmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_MAX_FRM_LEN_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_MAX_FRM_LEN_get", stop_bit, 15 );
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
    /* (0x00002018 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_TMAC_REG_MAX_FRAME_LENGTH_CFG */
    reg_value = tmac_reg_MAX_FRAME_LENGTH_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK)
                  >> TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK, TMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_MAX_FRM_LEN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tmac_field_MIN_FRM_TAG_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_MIN_FRM_TAG_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_MIN_FRM_TAG_set", value, 1);
    IOLOG( "%s <= 0x%08x", "tmac_field_MIN_FRM_TAG_set", value );

    /* (0x0000201c bits 6) field MIN_FRM_TAG of register PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG */
    tmac_reg_MIN_FRAME_LENGTH_CFG_field_set( b_ptr,
                                             h_ptr,
                                             TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_TAG_MSK,
                                             TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_TAG_OFF,
                                             value);
}

static INLINE UINT32 tmac_field_MIN_FRM_TAG_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_MIN_FRM_TAG_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000201c bits 6) field MIN_FRM_TAG of register PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG */
    reg_value = tmac_reg_MIN_FRAME_LENGTH_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_TAG_MSK) >> TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_TAG_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_MIN_FRM_TAG_get", value );

    return value;
}
static INLINE void tmac_field_MIN_FRM_LEN_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_MIN_FRM_LEN_set( tmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_MIN_FRM_LEN_set", value, 63);
    IOLOG( "%s <= 0x%08x", "tmac_field_MIN_FRM_LEN_set", value );

    /* (0x0000201c bits 5:0) bits 0:5 use field MIN_FRM_LEN of register PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG */
    tmac_reg_MIN_FRAME_LENGTH_CFG_field_set( b_ptr,
                                             h_ptr,
                                             TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_MSK,
                                             TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_OFF,
                                             value);
}

static INLINE UINT32 tmac_field_MIN_FRM_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_MIN_FRM_LEN_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0000201c bits 5:0) bits 0:5 use field MIN_FRM_LEN of register PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG */
    reg_value = tmac_reg_MIN_FRAME_LENGTH_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_MSK) >> TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_MIN_FRM_LEN_get", value );

    return value;
}
static INLINE void tmac_field_range_MIN_FRM_LEN_set( tmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_range_MIN_FRM_LEN_set( tmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_MIN_FRM_LEN_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_MIN_FRM_LEN_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_MIN_FRM_LEN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000201c bits 5:0) bits 0:5 use field MIN_FRM_LEN of register PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG */
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
        /* (0x0000201c bits 5:0) bits 0:5 use field MIN_FRM_LEN of register PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG */
        tmac_reg_MIN_FRAME_LENGTH_CFG_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_OFF + subfield_offset),
                                                 TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_field_range_MIN_FRM_LEN_get( tmac_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_range_MIN_FRM_LEN_get( tmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_MIN_FRM_LEN_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_MIN_FRM_LEN_get", stop_bit, 5 );
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
    /* (0x0000201c bits 5:0) bits 0:5 use field MIN_FRM_LEN of register PMC_TMAC_REG_MIN_FRAME_LENGTH_CFG */
    reg_value = tmac_reg_MIN_FRAME_LENGTH_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_MSK)
                  >> TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_MSK, TMAC_REG_MIN_FRAME_LENGTH_CFG_BIT_MIN_FRM_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_MIN_FRM_LEN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tmac_field_PAUSE_TIME_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_PAUSE_TIME_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_PAUSE_TIME_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tmac_field_PAUSE_TIME_set", value );

    /* (0x00002020 bits 15:0) bits 0:15 use field PAUSE_TIME of register PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG */
    tmac_reg_PAUSE_FRAME_TIME_CFG_field_set( b_ptr,
                                             h_ptr,
                                             TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_MSK,
                                             TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_OFF,
                                             value);
}

static INLINE UINT32 tmac_field_PAUSE_TIME_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_PAUSE_TIME_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002020 bits 15:0) bits 0:15 use field PAUSE_TIME of register PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG */
    reg_value = tmac_reg_PAUSE_FRAME_TIME_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_MSK) >> TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_PAUSE_TIME_get", value );

    return value;
}
static INLINE void tmac_field_range_PAUSE_TIME_set( tmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_range_PAUSE_TIME_set( tmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_PAUSE_TIME_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_PAUSE_TIME_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_PAUSE_TIME_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002020 bits 15:0) bits 0:15 use field PAUSE_TIME of register PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG */
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
        /* (0x00002020 bits 15:0) bits 0:15 use field PAUSE_TIME of register PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG */
        tmac_reg_PAUSE_FRAME_TIME_CFG_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_OFF + subfield_offset),
                                                 TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_field_range_PAUSE_TIME_get( tmac_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_range_PAUSE_TIME_get( tmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_PAUSE_TIME_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_PAUSE_TIME_get", stop_bit, 15 );
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
    /* (0x00002020 bits 15:0) bits 0:15 use field PAUSE_TIME of register PMC_TMAC_REG_PAUSE_FRAME_TIME_CFG */
    reg_value = tmac_reg_PAUSE_FRAME_TIME_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_MSK)
                  >> TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_MSK, TMAC_REG_PAUSE_FRAME_TIME_CFG_BIT_PAUSE_TIME_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_PAUSE_TIME_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void tmac_field_PAUSE_IVAL_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_PAUSE_IVAL_set( tmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "tmac_field_PAUSE_IVAL_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "tmac_field_PAUSE_IVAL_set", value );

    /* (0x00002024 bits 15:0) bits 0:15 use field PAUSE_IVAL of register PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG */
    tmac_reg_PAUSE_FRAME_INTERVAL_CFG_field_set( b_ptr,
                                                 h_ptr,
                                                 TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_MSK,
                                                 TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_OFF,
                                                 value);
}

static INLINE UINT32 tmac_field_PAUSE_IVAL_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_PAUSE_IVAL_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002024 bits 15:0) bits 0:15 use field PAUSE_IVAL of register PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG */
    reg_value = tmac_reg_PAUSE_FRAME_INTERVAL_CFG_read( b_ptr,
                                                        h_ptr);
    value = (reg_value & TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_MSK) >> TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_PAUSE_IVAL_get", value );

    return value;
}
static INLINE void tmac_field_range_PAUSE_IVAL_set( tmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void tmac_field_range_PAUSE_IVAL_set( tmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_PAUSE_IVAL_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_PAUSE_IVAL_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_PAUSE_IVAL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00002024 bits 15:0) bits 0:15 use field PAUSE_IVAL of register PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG */
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
        /* (0x00002024 bits 15:0) bits 0:15 use field PAUSE_IVAL of register PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG */
        tmac_reg_PAUSE_FRAME_INTERVAL_CFG_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_OFF + subfield_offset),
                                                     TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 tmac_field_range_PAUSE_IVAL_get( tmac_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_range_PAUSE_IVAL_get( tmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "tmac_field_range_PAUSE_IVAL_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "tmac_field_range_PAUSE_IVAL_get", stop_bit, 15 );
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
    /* (0x00002024 bits 15:0) bits 0:15 use field PAUSE_IVAL of register PMC_TMAC_REG_PAUSE_FRAME_INTERVAL_CFG */
    reg_value = tmac_reg_PAUSE_FRAME_INTERVAL_CFG_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_MSK)
                  >> TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_MSK, TMAC_REG_PAUSE_FRAME_INTERVAL_CFG_BIT_PAUSE_IVAL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "tmac_field_range_PAUSE_IVAL_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 tmac_field_TMAC_STATUS_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 tmac_field_TMAC_STATUS_get( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00002000 bits 15) field TMAC_STATUS of register PMC_TMAC_REG_CFG_1 */
    reg_value = tmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & TMAC_REG_CFG_1_BIT_TMAC_STATUS_MSK) >> TMAC_REG_CFG_1_BIT_TMAC_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "tmac_field_TMAC_STATUS_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TMAC_IO_INLINE_H */
