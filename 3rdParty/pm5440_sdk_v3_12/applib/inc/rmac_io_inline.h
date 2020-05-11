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
 *     and register accessor functions for the rmac block
 *****************************************************************************/
#ifndef _RMAC_IO_INLINE_H
#define _RMAC_IO_INLINE_H

#include "enet_fege_loc.h"
#include "rmac_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RMAC_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for rmac
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
} rmac_buffer_t;
static INLINE void rmac_buffer_init( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void rmac_buffer_init( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "rmac_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void rmac_buffer_flush( rmac_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void rmac_buffer_flush( rmac_buffer_t *b_ptr )
{
    IOLOG( "rmac_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 rmac_reg_read( rmac_buffer_t *b_ptr,
                                    enet_fege_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 rmac_reg_read( rmac_buffer_t *b_ptr,
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
static INLINE void rmac_reg_write( rmac_buffer_t *b_ptr,
                                   enet_fege_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_write( rmac_buffer_t *b_ptr,
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

static INLINE void rmac_field_set( rmac_buffer_t *b_ptr,
                                   enet_fege_handle_t *h_ptr,
                                   UINT32 mem_type,
                                   UINT32 reg,
                                   UINT32 mask,
                                   UINT32 unused_mask,
                                   UINT32 ofs,
                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_set( rmac_buffer_t *b_ptr,
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

static INLINE void rmac_action_on_write_field_set( rmac_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 mem_type,
                                                   UINT32 reg,
                                                   UINT32 mask,
                                                   UINT32 ofs,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_action_on_write_field_set( rmac_buffer_t *b_ptr,
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

static INLINE void rmac_burst_read( rmac_buffer_t *b_ptr,
                                    enet_fege_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 len,
                                    UINT32 *value ) ALWAYS_INLINE;
static INLINE void rmac_burst_read( rmac_buffer_t *b_ptr,
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

static INLINE void rmac_burst_write( rmac_buffer_t *b_ptr,
                                     enet_fege_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void rmac_burst_write( rmac_buffer_t *b_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE rmac_poll( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 mem_type,
                                              UINT32 reg,
                                              UINT32 mask,
                                              UINT32 value,
                                              PMC_POLL_COMPARISON_TYPE cmp,
                                              UINT32 max_count,
                                              UINT32 *num_failed_polls,
                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE rmac_poll( rmac_buffer_t *b_ptr,
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
 *  register access functions for rmac
 * ==================================================================================
 */

static INLINE void rmac_reg_CFG_1_write( rmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_CFG_1_write( rmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rmac_reg_CFG_1_write", value );
    rmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_CFG_1,
                    value);
}

static INLINE void rmac_reg_CFG_1_field_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_CFG_1_field_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rmac_reg_CFG_1_field_set", mask, ofs, value );
    rmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_CFG_1,
                    mask,
                    PMC_RMAC_REG_CFG_1_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rmac_reg_CFG_1_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_reg_CFG_1_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_RMAC_REG_CFG_1);

    IOLOG( "%s -> 0x%08x;", "rmac_reg_CFG_1_read", reg_value);
    return reg_value;
}

static INLINE void rmac_reg_CFG_2_write( rmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_CFG_2_write( rmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rmac_reg_CFG_2_write", value );
    rmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_CFG_2,
                    value);
}

static INLINE void rmac_reg_CFG_2_field_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_CFG_2_field_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 mask,
                                             UINT32 ofs,
                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rmac_reg_CFG_2_field_set", mask, ofs, value );
    rmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_CFG_2,
                    mask,
                    PMC_RMAC_REG_CFG_2_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rmac_reg_CFG_2_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_reg_CFG_2_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_RMAC_REG_CFG_2);

    IOLOG( "%s -> 0x%08x;", "rmac_reg_CFG_2_read", reg_value);
    return reg_value;
}

static INLINE void rmac_reg_MAX_FRAME_LENGTH_CFG_write( rmac_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_MAX_FRAME_LENGTH_CFG_write( rmac_buffer_t *b_ptr,
                                                        enet_fege_handle_t *h_ptr,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rmac_reg_MAX_FRAME_LENGTH_CFG_write", value );
    rmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG,
                    value);
}

static INLINE void rmac_reg_MAX_FRAME_LENGTH_CFG_field_set( rmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_MAX_FRAME_LENGTH_CFG_field_set( rmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rmac_reg_MAX_FRAME_LENGTH_CFG_field_set", mask, ofs, value );
    rmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG,
                    mask,
                    PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rmac_reg_MAX_FRAME_LENGTH_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_reg_MAX_FRAME_LENGTH_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG);

    IOLOG( "%s -> 0x%08x;", "rmac_reg_MAX_FRAME_LENGTH_CFG_read", reg_value);
    return reg_value;
}

static INLINE void rmac_reg_STATION_ADR_HIGH_WORD_CFG_write( rmac_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_STATION_ADR_HIGH_WORD_CFG_write( rmac_buffer_t *b_ptr,
                                                             enet_fege_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rmac_reg_STATION_ADR_HIGH_WORD_CFG_write", value );
    rmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG,
                    value);
}

static INLINE void rmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set( rmac_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set( rmac_buffer_t *b_ptr,
                                                                 enet_fege_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set", mask, ofs, value );
    rmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG,
                    mask,
                    PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rmac_reg_STATION_ADR_HIGH_WORD_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_reg_STATION_ADR_HIGH_WORD_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG);

    IOLOG( "%s -> 0x%08x;", "rmac_reg_STATION_ADR_HIGH_WORD_CFG_read", reg_value);
    return reg_value;
}

static INLINE void rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write( rmac_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write( rmac_buffer_t *b_ptr,
                                                               enet_fege_handle_t *h_ptr,
                                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write", value );
    rmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG,
                    value);
}

static INLINE void rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set( rmac_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set( rmac_buffer_t *b_ptr,
                                                                   enet_fege_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set", mask, ofs, value );
    rmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG,
                    mask,
                    PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG);

    IOLOG( "%s -> 0x%08x;", "rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read", reg_value);
    return reg_value;
}

static INLINE void rmac_reg_STATION_ADR_LOW_WORD_CFG_write( rmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_STATION_ADR_LOW_WORD_CFG_write( rmac_buffer_t *b_ptr,
                                                            enet_fege_handle_t *h_ptr,
                                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rmac_reg_STATION_ADR_LOW_WORD_CFG_write", value );
    rmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG,
                    value);
}

static INLINE void rmac_reg_STATION_ADR_LOW_WORD_CFG_field_set( rmac_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_STATION_ADR_LOW_WORD_CFG_field_set( rmac_buffer_t *b_ptr,
                                                                enet_fege_handle_t *h_ptr,
                                                                UINT32 mask,
                                                                UINT32 ofs,
                                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rmac_reg_STATION_ADR_LOW_WORD_CFG_field_set", mask, ofs, value );
    rmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG,
                    mask,
                    PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rmac_reg_STATION_ADR_LOW_WORD_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_reg_STATION_ADR_LOW_WORD_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG);

    IOLOG( "%s -> 0x%08x;", "rmac_reg_STATION_ADR_LOW_WORD_CFG_read", reg_value);
    return reg_value;
}

static INLINE void rmac_reg_LINK_AND_STATISTIC_CFG_write( rmac_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_LINK_AND_STATISTIC_CFG_write( rmac_buffer_t *b_ptr,
                                                          enet_fege_handle_t *h_ptr,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "rmac_reg_LINK_AND_STATISTIC_CFG_write", value );
    rmac_reg_write( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_LINK_AND_STATISTIC_CFG,
                    value);
}

static INLINE void rmac_reg_LINK_AND_STATISTIC_CFG_field_set( rmac_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_reg_LINK_AND_STATISTIC_CFG_field_set( rmac_buffer_t *b_ptr,
                                                              enet_fege_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "rmac_reg_LINK_AND_STATISTIC_CFG_field_set", mask, ofs, value );
    rmac_field_set( b_ptr,
                    h_ptr,
                    MEM_TYPE_CONFIG,
                    PMC_RMAC_REG_LINK_AND_STATISTIC_CFG,
                    mask,
                    PMC_RMAC_REG_LINK_AND_STATISTIC_CFG_UNUSED_MASK,
                    ofs,
                    value);

}

static INLINE UINT32 rmac_reg_LINK_AND_STATISTIC_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_reg_LINK_AND_STATISTIC_CFG_read( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = rmac_reg_read( b_ptr,
                               h_ptr,
                               MEM_TYPE_CONFIG,
                               PMC_RMAC_REG_LINK_AND_STATISTIC_CFG);

    IOLOG( "%s -> 0x%08x;", "rmac_reg_LINK_AND_STATISTIC_CFG_read", reg_value);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void rmac_field_PAMB_CK_set( rmac_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_PAMB_CK_set( rmac_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_PAMB_CK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_PAMB_CK_set", value );

    /* (0x00000100 bits 14) field PAMB_CK of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_PAMB_CK_MSK,
                              RMAC_REG_CFG_1_BIT_PAMB_CK_OFF,
                              value);
}

static INLINE UINT32 rmac_field_PAMB_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_PAMB_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 14) field PAMB_CK of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_PAMB_CK_MSK) >> RMAC_REG_CFG_1_BIT_PAMB_CK_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_PAMB_CK_get", value );

    return value;
}
static INLINE void rmac_field_TAG_AWARE_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_TAG_AWARE_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_TAG_AWARE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_TAG_AWARE_set", value );

    /* (0x00000100 bits 13) field TAG_AWARE of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_TAG_AWARE_MSK,
                              RMAC_REG_CFG_1_BIT_TAG_AWARE_OFF,
                              value);
}

static INLINE UINT32 rmac_field_TAG_AWARE_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_TAG_AWARE_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 13) field TAG_AWARE of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_TAG_AWARE_MSK) >> RMAC_REG_CFG_1_BIT_TAG_AWARE_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_TAG_AWARE_get", value );

    return value;
}
static INLINE void rmac_field_INRANGE_DIS_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_INRANGE_DIS_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_INRANGE_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_INRANGE_DIS_set", value );

    /* (0x00000100 bits 12) field INRANGE_DIS of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_INRANGE_DIS_MSK,
                              RMAC_REG_CFG_1_BIT_INRANGE_DIS_OFF,
                              value);
}

static INLINE UINT32 rmac_field_INRANGE_DIS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_INRANGE_DIS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 12) field INRANGE_DIS of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_INRANGE_DIS_MSK) >> RMAC_REG_CFG_1_BIT_INRANGE_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_INRANGE_DIS_get", value );

    return value;
}
static INLINE void rmac_field_INRANGE_CK_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_INRANGE_CK_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_INRANGE_CK_set", value, 3);
    IOLOG( "%s <= 0x%08x", "rmac_field_INRANGE_CK_set", value );

    /* (0x00000100 bits 11:10) bits 0:1 use field INRANGE_CK of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_INRANGE_CK_MSK,
                              RMAC_REG_CFG_1_BIT_INRANGE_CK_OFF,
                              value);
}

static INLINE UINT32 rmac_field_INRANGE_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_INRANGE_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 11:10) bits 0:1 use field INRANGE_CK of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_INRANGE_CK_MSK) >> RMAC_REG_CFG_1_BIT_INRANGE_CK_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_INRANGE_CK_get", value );

    return value;
}
static INLINE void rmac_field_range_INRANGE_CK_set( rmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_range_INRANGE_CK_set( rmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 start_bit,
                                                    UINT32 stop_bit,
                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_INRANGE_CK_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_INRANGE_CK_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_INRANGE_CK_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000100 bits 11:10) bits 0:1 use field INRANGE_CK of register PMC_RMAC_REG_CFG_1 */
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
        /* (0x00000100 bits 11:10) bits 0:1 use field INRANGE_CK of register PMC_RMAC_REG_CFG_1 */
        rmac_reg_CFG_1_field_set( b_ptr,
                                  h_ptr,
                                  subfield_mask << (RMAC_REG_CFG_1_BIT_INRANGE_CK_OFF + subfield_offset),
                                  RMAC_REG_CFG_1_BIT_INRANGE_CK_OFF + subfield_offset,
                                  value >> subfield_shift);
    }
}

static INLINE UINT32 rmac_field_range_INRANGE_CK_get( rmac_buffer_t *b_ptr,
                                                      enet_fege_handle_t *h_ptr,
                                                      UINT32 start_bit,
                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_range_INRANGE_CK_get( rmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_INRANGE_CK_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_INRANGE_CK_get", stop_bit, 1 );
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
    /* (0x00000100 bits 11:10) bits 0:1 use field INRANGE_CK of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    field_value = (reg_value & RMAC_REG_CFG_1_BIT_INRANGE_CK_MSK)
                  >> RMAC_REG_CFG_1_BIT_INRANGE_CK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RMAC_REG_CFG_1_BIT_INRANGE_CK_MSK, RMAC_REG_CFG_1_BIT_INRANGE_CK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_INRANGE_CK_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void rmac_field_MAX_FRM_TRN_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_MAX_FRM_TRN_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_MAX_FRM_TRN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_MAX_FRM_TRN_set", value );

    /* (0x00000100 bits 9) field MAX_FRM_TRN of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_MAX_FRM_TRN_MSK,
                              RMAC_REG_CFG_1_BIT_MAX_FRM_TRN_OFF,
                              value);
}

static INLINE UINT32 rmac_field_MAX_FRM_TRN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_MAX_FRM_TRN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 9) field MAX_FRM_TRN of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_MAX_FRM_TRN_MSK) >> RMAC_REG_CFG_1_BIT_MAX_FRM_TRN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_MAX_FRM_TRN_get", value );

    return value;
}
static INLINE void rmac_field_MAX_FRM_DIS_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_MAX_FRM_DIS_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_MAX_FRM_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_MAX_FRM_DIS_set", value );

    /* (0x00000100 bits 8) field MAX_FRM_DIS of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_MAX_FRM_DIS_MSK,
                              RMAC_REG_CFG_1_BIT_MAX_FRM_DIS_OFF,
                              value);
}

static INLINE UINT32 rmac_field_MAX_FRM_DIS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_MAX_FRM_DIS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 8) field MAX_FRM_DIS of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_MAX_FRM_DIS_MSK) >> RMAC_REG_CFG_1_BIT_MAX_FRM_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_MAX_FRM_DIS_get", value );

    return value;
}
static INLINE void rmac_field_MAX_FRM_CK_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_MAX_FRM_CK_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_MAX_FRM_CK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_MAX_FRM_CK_set", value );

    /* (0x00000100 bits 7) field MAX_FRM_CK of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_MAX_FRM_CK_MSK,
                              RMAC_REG_CFG_1_BIT_MAX_FRM_CK_OFF,
                              value);
}

static INLINE UINT32 rmac_field_MAX_FRM_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_MAX_FRM_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 7) field MAX_FRM_CK of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_MAX_FRM_CK_MSK) >> RMAC_REG_CFG_1_BIT_MAX_FRM_CK_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_MAX_FRM_CK_get", value );

    return value;
}
static INLINE void rmac_field_MIN_FRM_DIS_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_MIN_FRM_DIS_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_MIN_FRM_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_MIN_FRM_DIS_set", value );

    /* (0x00000100 bits 6) field MIN_FRM_DIS of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_MIN_FRM_DIS_MSK,
                              RMAC_REG_CFG_1_BIT_MIN_FRM_DIS_OFF,
                              value);
}

static INLINE UINT32 rmac_field_MIN_FRM_DIS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_MIN_FRM_DIS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 6) field MIN_FRM_DIS of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_MIN_FRM_DIS_MSK) >> RMAC_REG_CFG_1_BIT_MIN_FRM_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_MIN_FRM_DIS_get", value );

    return value;
}
static INLINE void rmac_field_MIN_FRM_CK_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_MIN_FRM_CK_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_MIN_FRM_CK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_MIN_FRM_CK_set", value );

    /* (0x00000100 bits 5) field MIN_FRM_CK of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_MIN_FRM_CK_MSK,
                              RMAC_REG_CFG_1_BIT_MIN_FRM_CK_OFF,
                              value);
}

static INLINE UINT32 rmac_field_MIN_FRM_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_MIN_FRM_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 5) field MIN_FRM_CK of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_MIN_FRM_CK_MSK) >> RMAC_REG_CFG_1_BIT_MIN_FRM_CK_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_MIN_FRM_CK_get", value );

    return value;
}
static INLINE void rmac_field_CRC_DIS_set( rmac_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_CRC_DIS_set( rmac_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_CRC_DIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_CRC_DIS_set", value );

    /* (0x00000100 bits 4) field CRC_DIS of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_CRC_DIS_MSK,
                              RMAC_REG_CFG_1_BIT_CRC_DIS_OFF,
                              value);
}

static INLINE UINT32 rmac_field_CRC_DIS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_CRC_DIS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 4) field CRC_DIS of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_CRC_DIS_MSK) >> RMAC_REG_CFG_1_BIT_CRC_DIS_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_CRC_DIS_get", value );

    return value;
}
static INLINE void rmac_field_CRC_CK_set( rmac_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_CRC_CK_set( rmac_buffer_t *b_ptr,
                                          enet_fege_handle_t *h_ptr,
                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_CRC_CK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_CRC_CK_set", value );

    /* (0x00000100 bits 3) field CRC_CK of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_CRC_CK_MSK,
                              RMAC_REG_CFG_1_BIT_CRC_CK_OFF,
                              value);
}

static INLINE UINT32 rmac_field_CRC_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_CRC_CK_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 3) field CRC_CK of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_CRC_CK_MSK) >> RMAC_REG_CFG_1_BIT_CRC_CK_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_CRC_CK_get", value );

    return value;
}
static INLINE void rmac_field_TRANSPARENT_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_TRANSPARENT_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_TRANSPARENT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_TRANSPARENT_set", value );

    /* (0x00000100 bits 2) field TRANSPARENT of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_TRANSPARENT_MSK,
                              RMAC_REG_CFG_1_BIT_TRANSPARENT_OFF,
                              value);
}

static INLINE UINT32 rmac_field_TRANSPARENT_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_TRANSPARENT_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 2) field TRANSPARENT of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_TRANSPARENT_MSK) >> RMAC_REG_CFG_1_BIT_TRANSPARENT_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_TRANSPARENT_get", value );

    return value;
}
static INLINE void rmac_field_ETHER_MODE_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_ETHER_MODE_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_ETHER_MODE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_ETHER_MODE_set", value );

    /* (0x00000100 bits 1) field ETHER_MODE of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_ETHER_MODE_MSK,
                              RMAC_REG_CFG_1_BIT_ETHER_MODE_OFF,
                              value);
}

static INLINE UINT32 rmac_field_ETHER_MODE_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_ETHER_MODE_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 1) field ETHER_MODE of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_ETHER_MODE_MSK) >> RMAC_REG_CFG_1_BIT_ETHER_MODE_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_ETHER_MODE_get", value );

    return value;
}
static INLINE void rmac_field_RMAC_ENBL_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_RMAC_ENBL_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_RMAC_ENBL_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_RMAC_ENBL_set", value );

    /* (0x00000100 bits 0) field RMAC_ENBL of register PMC_RMAC_REG_CFG_1 */
    rmac_reg_CFG_1_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_1_BIT_RMAC_ENBL_MSK,
                              RMAC_REG_CFG_1_BIT_RMAC_ENBL_OFF,
                              value);
}

static INLINE UINT32 rmac_field_RMAC_ENBL_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_RMAC_ENBL_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 0) field RMAC_ENBL of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_RMAC_ENBL_MSK) >> RMAC_REG_CFG_1_BIT_RMAC_ENBL_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_RMAC_ENBL_get", value );

    return value;
}
static INLINE void rmac_field_INTERPKT_L1_EN_set( rmac_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_INTERPKT_L1_EN_set( rmac_buffer_t *b_ptr,
                                                  enet_fege_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_INTERPKT_L1_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_INTERPKT_L1_EN_set", value );

    /* (0x00000104 bits 15) field INTERPKT_L1_EN of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_INTERPKT_L1_EN_MSK,
                              RMAC_REG_CFG_2_BIT_INTERPKT_L1_EN_OFF,
                              value);
}

static INLINE UINT32 rmac_field_INTERPKT_L1_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_INTERPKT_L1_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 15) field INTERPKT_L1_EN of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_INTERPKT_L1_EN_MSK) >> RMAC_REG_CFG_2_BIT_INTERPKT_L1_EN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_INTERPKT_L1_EN_get", value );

    return value;
}
static INLINE void rmac_field_BLIND_set( rmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_BLIND_set( rmac_buffer_t *b_ptr,
                                         enet_fege_handle_t *h_ptr,
                                         UINT32 value )
{
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_BLIND_set", value, 3);
    IOLOG( "%s <= 0x%08x", "rmac_field_BLIND_set", value );

    /* (0x00000104 bits 14:13) bits 0:1 use field BLIND of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_BLIND_MSK,
                              RMAC_REG_CFG_2_BIT_BLIND_OFF,
                              value);
}

static INLINE UINT32 rmac_field_BLIND_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_BLIND_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 14:13) bits 0:1 use field BLIND of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_BLIND_MSK) >> RMAC_REG_CFG_2_BIT_BLIND_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_BLIND_get", value );

    return value;
}
static INLINE void rmac_field_range_BLIND_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_range_BLIND_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 start_bit,
                                               UINT32 stop_bit,
                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_BLIND_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_BLIND_set", stop_bit, 1 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_BLIND_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000104 bits 14:13) bits 0:1 use field BLIND of register PMC_RMAC_REG_CFG_2 */
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
        /* (0x00000104 bits 14:13) bits 0:1 use field BLIND of register PMC_RMAC_REG_CFG_2 */
        rmac_reg_CFG_2_field_set( b_ptr,
                                  h_ptr,
                                  subfield_mask << (RMAC_REG_CFG_2_BIT_BLIND_OFF + subfield_offset),
                                  RMAC_REG_CFG_2_BIT_BLIND_OFF + subfield_offset,
                                  value >> subfield_shift);
    }
}

static INLINE UINT32 rmac_field_range_BLIND_get( rmac_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 start_bit,
                                                 UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_range_BLIND_get( rmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_BLIND_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_BLIND_get", stop_bit, 1 );
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
    /* (0x00000104 bits 14:13) bits 0:1 use field BLIND of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & RMAC_REG_CFG_2_BIT_BLIND_MSK)
                  >> RMAC_REG_CFG_2_BIT_BLIND_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RMAC_REG_CFG_2_BIT_BLIND_MSK, RMAC_REG_CFG_2_BIT_BLIND_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_BLIND_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void rmac_field_MIN_FRM_LEN_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_MIN_FRM_LEN_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 63)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_MIN_FRM_LEN_set", value, 63);
    IOLOG( "%s <= 0x%08x", "rmac_field_MIN_FRM_LEN_set", value );

    /* (0x00000104 bits 12:7) bits 0:5 use field MIN_FRM_LEN of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_MSK,
                              RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_OFF,
                              value);
}

static INLINE UINT32 rmac_field_MIN_FRM_LEN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_MIN_FRM_LEN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 12:7) bits 0:5 use field MIN_FRM_LEN of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_MSK) >> RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_MIN_FRM_LEN_get", value );

    return value;
}
static INLINE void rmac_field_range_MIN_FRM_LEN_set( rmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_range_MIN_FRM_LEN_set( rmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_MIN_FRM_LEN_set", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_MIN_FRM_LEN_set", stop_bit, 5 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_MIN_FRM_LEN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000104 bits 12:7) bits 0:5 use field MIN_FRM_LEN of register PMC_RMAC_REG_CFG_2 */
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
        /* (0x00000104 bits 12:7) bits 0:5 use field MIN_FRM_LEN of register PMC_RMAC_REG_CFG_2 */
        rmac_reg_CFG_2_field_set( b_ptr,
                                  h_ptr,
                                  subfield_mask << (RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_OFF + subfield_offset),
                                  RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_OFF + subfield_offset,
                                  value >> subfield_shift);
    }
}

static INLINE UINT32 rmac_field_range_MIN_FRM_LEN_get( rmac_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_range_MIN_FRM_LEN_get( rmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_MIN_FRM_LEN_get", stop_bit, start_bit );
    if (stop_bit > 5) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_MIN_FRM_LEN_get", stop_bit, 5 );
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
    /* (0x00000104 bits 12:7) bits 0:5 use field MIN_FRM_LEN of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    field_value = (reg_value & RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_MSK)
                  >> RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_MSK, RMAC_REG_CFG_2_BIT_MIN_FRM_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_MIN_FRM_LEN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void rmac_field_PAD_STRIP_EN_set( rmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_PAD_STRIP_EN_set( rmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_PAD_STRIP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_PAD_STRIP_EN_set", value );

    /* (0x00000104 bits 6) field PAD_STRIP_EN of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_PAD_STRIP_EN_MSK,
                              RMAC_REG_CFG_2_BIT_PAD_STRIP_EN_OFF,
                              value);
}

static INLINE UINT32 rmac_field_PAD_STRIP_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_PAD_STRIP_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 6) field PAD_STRIP_EN of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_PAD_STRIP_EN_MSK) >> RMAC_REG_CFG_2_BIT_PAD_STRIP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_PAD_STRIP_EN_get", value );

    return value;
}
static INLINE void rmac_field_CRC_STRIP_EN_set( rmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_CRC_STRIP_EN_set( rmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_CRC_STRIP_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_CRC_STRIP_EN_set", value );

    /* (0x00000104 bits 5) field CRC_STRIP_EN of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_CRC_STRIP_EN_MSK,
                              RMAC_REG_CFG_2_BIT_CRC_STRIP_EN_OFF,
                              value);
}

static INLINE UINT32 rmac_field_CRC_STRIP_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_CRC_STRIP_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 5) field CRC_STRIP_EN of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_CRC_STRIP_EN_MSK) >> RMAC_REG_CFG_2_BIT_CRC_STRIP_EN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_CRC_STRIP_EN_get", value );

    return value;
}
static INLINE void rmac_field_CARRIER_EN_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_CARRIER_EN_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_CARRIER_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_CARRIER_EN_set", value );

    /* (0x00000104 bits 4) field CARRIER_EN of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_CARRIER_EN_MSK,
                              RMAC_REG_CFG_2_BIT_CARRIER_EN_OFF,
                              value);
}

static INLINE UINT32 rmac_field_CARRIER_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_CARRIER_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 4) field CARRIER_EN of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_CARRIER_EN_MSK) >> RMAC_REG_CFG_2_BIT_CARRIER_EN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_CARRIER_EN_get", value );

    return value;
}
static INLINE void rmac_field_INPAUSE_EN_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_INPAUSE_EN_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_INPAUSE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_INPAUSE_EN_set", value );

    /* (0x00000104 bits 3) field INPAUSE_EN of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_INPAUSE_EN_MSK,
                              RMAC_REG_CFG_2_BIT_INPAUSE_EN_OFF,
                              value);
}

static INLINE UINT32 rmac_field_INPAUSE_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_INPAUSE_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 3) field INPAUSE_EN of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_INPAUSE_EN_MSK) >> RMAC_REG_CFG_2_BIT_INPAUSE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_INPAUSE_EN_get", value );

    return value;
}
static INLINE void rmac_field_PAUSEFRM_DROP_set( rmac_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_PAUSEFRM_DROP_set( rmac_buffer_t *b_ptr,
                                                 enet_fege_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_PAUSEFRM_DROP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_PAUSEFRM_DROP_set", value );

    /* (0x00000104 bits 2) field PAUSEFRM_DROP of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_PAUSEFRM_DROP_MSK,
                              RMAC_REG_CFG_2_BIT_PAUSEFRM_DROP_OFF,
                              value);
}

static INLINE UINT32 rmac_field_PAUSEFRM_DROP_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_PAUSEFRM_DROP_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 2) field PAUSEFRM_DROP of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_PAUSEFRM_DROP_MSK) >> RMAC_REG_CFG_2_BIT_PAUSEFRM_DROP_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_PAUSEFRM_DROP_get", value );

    return value;
}
static INLINE void rmac_field_CTRLFRM_DROP_set( rmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_CTRLFRM_DROP_set( rmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_CTRLFRM_DROP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_CTRLFRM_DROP_set", value );

    /* (0x00000104 bits 1) field CTRLFRM_DROP of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_CTRLFRM_DROP_MSK,
                              RMAC_REG_CFG_2_BIT_CTRLFRM_DROP_OFF,
                              value);
}

static INLINE UINT32 rmac_field_CTRLFRM_DROP_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_CTRLFRM_DROP_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 1) field CTRLFRM_DROP of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_CTRLFRM_DROP_MSK) >> RMAC_REG_CFG_2_BIT_CTRLFRM_DROP_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_CTRLFRM_DROP_get", value );

    return value;
}
static INLINE void rmac_field_DA_DROP_set( rmac_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_DA_DROP_set( rmac_buffer_t *b_ptr,
                                           enet_fege_handle_t *h_ptr,
                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_DA_DROP_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_DA_DROP_set", value );

    /* (0x00000104 bits 0) field DA_DROP of register PMC_RMAC_REG_CFG_2 */
    rmac_reg_CFG_2_field_set( b_ptr,
                              h_ptr,
                              RMAC_REG_CFG_2_BIT_DA_DROP_MSK,
                              RMAC_REG_CFG_2_BIT_DA_DROP_OFF,
                              value);
}

static INLINE UINT32 rmac_field_DA_DROP_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_DA_DROP_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000104 bits 0) field DA_DROP of register PMC_RMAC_REG_CFG_2 */
    reg_value = rmac_reg_CFG_2_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_2_BIT_DA_DROP_MSK) >> RMAC_REG_CFG_2_BIT_DA_DROP_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_DA_DROP_get", value );

    return value;
}
static INLINE void rmac_field_MAX_FRM_LEN_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_MAX_FRM_LEN_set( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 65535)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_MAX_FRM_LEN_set", value, 65535);
    IOLOG( "%s <= 0x%08x", "rmac_field_MAX_FRM_LEN_set", value );

    /* (0x00000108 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG */
    rmac_reg_MAX_FRAME_LENGTH_CFG_field_set( b_ptr,
                                             h_ptr,
                                             RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK,
                                             RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF,
                                             value);
}

static INLINE UINT32 rmac_field_MAX_FRM_LEN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_MAX_FRM_LEN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000108 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG */
    reg_value = rmac_reg_MAX_FRAME_LENGTH_CFG_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK) >> RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_MAX_FRM_LEN_get", value );

    return value;
}
static INLINE void rmac_field_range_MAX_FRM_LEN_set( rmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_range_MAX_FRM_LEN_set( rmac_buffer_t *b_ptr,
                                                     enet_fege_handle_t *h_ptr,
                                                     UINT32 start_bit,
                                                     UINT32 stop_bit,
                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_MAX_FRM_LEN_set", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_MAX_FRM_LEN_set", stop_bit, 15 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_MAX_FRM_LEN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000108 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG */
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
        /* (0x00000108 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG */
        rmac_reg_MAX_FRAME_LENGTH_CFG_field_set( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF + subfield_offset),
                                                 RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 rmac_field_range_MAX_FRM_LEN_get( rmac_buffer_t *b_ptr,
                                                       enet_fege_handle_t *h_ptr,
                                                       UINT32 start_bit,
                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_range_MAX_FRM_LEN_get( rmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_MAX_FRM_LEN_get", stop_bit, start_bit );
    if (stop_bit > 15) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_MAX_FRM_LEN_get", stop_bit, 15 );
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
    /* (0x00000108 bits 15:0) bits 0:15 use field MAX_FRM_LEN of register PMC_RMAC_REG_MAX_FRAME_LENGTH_CFG */
    reg_value = rmac_reg_MAX_FRAME_LENGTH_CFG_read(  b_ptr, h_ptr);
    field_value = (reg_value & RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK)
                  >> RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_MSK, RMAC_REG_MAX_FRAME_LENGTH_CFG_BIT_MAX_FRM_LEN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_MAX_FRM_LEN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void rmac_lfield_STA_set( rmac_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void rmac_lfield_STA_set( rmac_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 value[2] )
{
    IOLOG( "%s <= 0x%08x 0x%08x", "rmac_lfield_STA_set", value[1] , value[0] );

    /* (0x0000010c bits 15:0) bits 32:47 use field STA of register PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG */
    rmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set( b_ptr,
                                                  h_ptr,
                                                  RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK,
                                                  RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF,
                                                  value[1]);

    /* (0x00000110 bits 15:0) bits 16:31 use field STA of register PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
    rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set( b_ptr,
                                                    h_ptr,
                                                    RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK,
                                                    RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF,
                                                    value[0]);

    /* (0x00000114 bits 15:0) bits 0:15 use field STA of register PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG */
    rmac_reg_STATION_ADR_LOW_WORD_CFG_field_set( b_ptr,
                                                 h_ptr,
                                                 RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK,
                                                 RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF,
                                                 value[0]);
}

static INLINE void rmac_lfield_STA_get( rmac_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void rmac_lfield_STA_get( rmac_buffer_t *b_ptr,
                                        enet_fege_handle_t *h_ptr,
                                        UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x0000010c bits 15:0) bits 32:47 use field STA of register PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG */
    reg_value = rmac_reg_STATION_ADR_HIGH_WORD_CFG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK) >> RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF;
    value[1] |= field_value;

    /* (0x00000110 bits 15:0) bits 16:31 use field STA of register PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
    reg_value = rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read( b_ptr,
                                                           h_ptr);
    field_value = (reg_value & RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK) >> RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF;
    value[0] |= (field_value << 16);

    /* (0x00000114 bits 15:0) bits 0:15 use field STA of register PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG */
    reg_value = rmac_reg_STATION_ADR_LOW_WORD_CFG_read( b_ptr,
                                                        h_ptr);
    field_value = (reg_value & RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK) >> RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF;
    value[0] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "rmac_lfield_STA_get", value[1] , value[0] );

}
static INLINE void rmac_lfield_range_STA_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_lfield_range_STA_set( rmac_buffer_t *b_ptr,
                                              enet_fege_handle_t *h_ptr,
                                              UINT32 start_bit,
                                              UINT32 stop_bit,
                                              UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_lfield_range_STA_set", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_lfield_range_STA_set", stop_bit, 47 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rmac_lfield_range_STA_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0000010c bits 15:0) bits 32:47 use field STA of register PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG */
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
        /* (0x0000010c bits 15:0) bits 32:47 use field STA of register PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG */
        rmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF + subfield_offset),
                                                      RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000110 bits 15:0) bits 16:31 use field STA of register PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
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
        /* (0x00000110 bits 15:0) bits 16:31 use field STA of register PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
        rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF + subfield_offset),
                                                        RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000114 bits 15:0) bits 0:15 use field STA of register PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG */
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
        /* (0x00000114 bits 15:0) bits 0:15 use field STA of register PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG */
        rmac_reg_STATION_ADR_LOW_WORD_CFG_field_set( b_ptr,
                                                     h_ptr,
                                                     subfield_mask << (RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF + subfield_offset),
                                                     RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF + subfield_offset,
                                                     value >> subfield_shift);
    }
}

static INLINE UINT32 rmac_lfield_range_STA_get( rmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_lfield_range_STA_get( rmac_buffer_t *b_ptr,
                                                enet_fege_handle_t *h_ptr,
                                                UINT32 start_bit,
                                                UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_lfield_range_STA_get", stop_bit, start_bit );
    if (stop_bit > 47) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_lfield_range_STA_get", stop_bit, 47 );
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
    /* (0x0000010c bits 15:0) bits 32:47 use field STA of register PMC_RMAC_REG_STATION_ADR_HIGH_WORD_CFG */
        reg_value = rmac_reg_STATION_ADR_HIGH_WORD_CFG_read( b_ptr,
                                                             h_ptr);
        field_value = (reg_value & RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK)
                      >> RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_MSK, RMAC_REG_STATION_ADR_HIGH_WORD_CFG_BIT_STA_OFF, field_value );
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
    /* (0x00000110 bits 15:0) bits 16:31 use field STA of register PMC_RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG */
        reg_value = rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read( b_ptr,
                                                               h_ptr);
        field_value = (reg_value & RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK)
                      >> RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_MSK, RMAC_REG_STATION_ADR_MIDDLE_WORD_CFG_BIT_STA_OFF, field_value );
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
    /* (0x00000114 bits 15:0) bits 0:15 use field STA of register PMC_RMAC_REG_STATION_ADR_LOW_WORD_CFG */
        reg_value = rmac_reg_STATION_ADR_LOW_WORD_CFG_read( b_ptr,
                                                            h_ptr);
        field_value = (reg_value & RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK)
                      >> RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_MSK, RMAC_REG_STATION_ADR_LOW_WORD_CFG_BIT_STA_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rmac_lfield_range_STA_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void rmac_field_CTRLFRM_STAT_EN_set( rmac_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_CTRLFRM_STAT_EN_set( rmac_buffer_t *b_ptr,
                                                   enet_fege_handle_t *h_ptr,
                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_CTRLFRM_STAT_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_CTRLFRM_STAT_EN_set", value );

    /* (0x00000118 bits 9) field CTRLFRM_STAT_EN of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
    rmac_reg_LINK_AND_STATISTIC_CFG_field_set( b_ptr,
                                               h_ptr,
                                               RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_CTRLFRM_STAT_EN_MSK,
                                               RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_CTRLFRM_STAT_EN_OFF,
                                               value);
}

static INLINE UINT32 rmac_field_CTRLFRM_STAT_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_CTRLFRM_STAT_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000118 bits 9) field CTRLFRM_STAT_EN of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
    reg_value = rmac_reg_LINK_AND_STATISTIC_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_CTRLFRM_STAT_EN_MSK) >> RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_CTRLFRM_STAT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_CTRLFRM_STAT_EN_get", value );

    return value;
}
static INLINE void rmac_field_PAUSEFRM_STAT_EN_set( rmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_PAUSEFRM_STAT_EN_set( rmac_buffer_t *b_ptr,
                                                    enet_fege_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_PAUSEFRM_STAT_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "rmac_field_PAUSEFRM_STAT_EN_set", value );

    /* (0x00000118 bits 8) field PAUSEFRM_STAT_EN of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
    rmac_reg_LINK_AND_STATISTIC_CFG_field_set( b_ptr,
                                               h_ptr,
                                               RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_PAUSEFRM_STAT_EN_MSK,
                                               RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_PAUSEFRM_STAT_EN_OFF,
                                               value);
}

static INLINE UINT32 rmac_field_PAUSEFRM_STAT_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_PAUSEFRM_STAT_EN_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000118 bits 8) field PAUSEFRM_STAT_EN of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
    reg_value = rmac_reg_LINK_AND_STATISTIC_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_PAUSEFRM_STAT_EN_MSK) >> RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_PAUSEFRM_STAT_EN_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_PAUSEFRM_STAT_EN_get", value );

    return value;
}
static INLINE void rmac_field_GPO_set( rmac_buffer_t *b_ptr,
                                       enet_fege_handle_t *h_ptr,
                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_GPO_set( rmac_buffer_t *b_ptr,
                                       enet_fege_handle_t *h_ptr,
                                       UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "rmac_field_GPO_set", value, 15);
    IOLOG( "%s <= 0x%08x", "rmac_field_GPO_set", value );

    /* (0x00000118 bits 7:4) bits 0:3 use field GPO of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
    rmac_reg_LINK_AND_STATISTIC_CFG_field_set( b_ptr,
                                               h_ptr,
                                               RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_MSK,
                                               RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_OFF,
                                               value);
}

static INLINE UINT32 rmac_field_GPO_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_GPO_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000118 bits 7:4) bits 0:3 use field GPO of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
    reg_value = rmac_reg_LINK_AND_STATISTIC_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_MSK) >> RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_GPO_get", value );

    return value;
}
static INLINE void rmac_field_range_GPO_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void rmac_field_range_GPO_set( rmac_buffer_t *b_ptr,
                                             enet_fege_handle_t *h_ptr,
                                             UINT32 start_bit,
                                             UINT32 stop_bit,
                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_GPO_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_GPO_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_GPO_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00000118 bits 7:4) bits 0:3 use field GPO of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
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
        /* (0x00000118 bits 7:4) bits 0:3 use field GPO of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
        rmac_reg_LINK_AND_STATISTIC_CFG_field_set( b_ptr,
                                                   h_ptr,
                                                   subfield_mask << (RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_OFF + subfield_offset),
                                                   RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 rmac_field_range_GPO_get( rmac_buffer_t *b_ptr,
                                               enet_fege_handle_t *h_ptr,
                                               UINT32 start_bit,
                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_range_GPO_get( rmac_buffer_t *b_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "rmac_field_range_GPO_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "rmac_field_range_GPO_get", stop_bit, 3 );
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
    /* (0x00000118 bits 7:4) bits 0:3 use field GPO of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
    reg_value = rmac_reg_LINK_AND_STATISTIC_CFG_read( b_ptr,
                                                      h_ptr);
    field_value = (reg_value & RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_MSK)
                  >> RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_MSK, RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GPO_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "rmac_field_range_GPO_get", start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status
 * ==================================================================================
 */
static INLINE UINT32 rmac_field_RMAC_STATUS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_RMAC_STATUS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000100 bits 15) field RMAC_STATUS of register PMC_RMAC_REG_CFG_1 */
    reg_value = rmac_reg_CFG_1_read(  b_ptr, h_ptr);
    value = (reg_value & RMAC_REG_CFG_1_BIT_RMAC_STATUS_MSK) >> RMAC_REG_CFG_1_BIT_RMAC_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_RMAC_STATUS_get", value );

    return value;
}
static INLINE UINT32 rmac_field_GIGA_STATUS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 rmac_field_GIGA_STATUS_get( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00000118 bits 15) field GIGA_STATUS of register PMC_RMAC_REG_LINK_AND_STATISTIC_CFG */
    reg_value = rmac_reg_LINK_AND_STATISTIC_CFG_read( b_ptr,
                                                      h_ptr);
    value = (reg_value & RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GIGA_STATUS_MSK) >> RMAC_REG_LINK_AND_STATISTIC_CFG_BIT_GIGA_STATUS_OFF;
    IOLOG( "%s -> 0x%08x", "rmac_field_GIGA_STATUS_get", value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RMAC_IO_INLINE_H */
