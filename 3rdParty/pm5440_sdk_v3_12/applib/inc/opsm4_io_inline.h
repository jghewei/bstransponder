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
 *     and register accessor functions for the opsm4 block
 *****************************************************************************/
#ifndef _OPSM4_IO_INLINE_H
#define _OPSM4_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "opsm4_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OPSM4_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for opsm4
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
    lineotn_handle_t * h_ptr;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} opsm4_buffer_t;
static INLINE void opsm4_buffer_init( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ALWAYS_INLINE;
static INLINE void opsm4_buffer_init( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = ((pmc_handle_t *)h_ptr)->base_address;
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s", "opsm4_buffer_init");
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void opsm4_buffer_flush( opsm4_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void opsm4_buffer_flush( opsm4_buffer_t *b_ptr )
{
    IOLOG( "opsm4_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 opsm4_reg_read( opsm4_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_read( opsm4_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
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
static INLINE void opsm4_reg_write( opsm4_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_write( opsm4_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
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

static INLINE void opsm4_field_set( opsm4_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
                                    UINT32 mem_type,
                                    UINT32 reg,
                                    UINT32 mask,
                                    UINT32 unused_mask,
                                    UINT32 ofs,
                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_set( opsm4_buffer_t *b_ptr,
                                    lineotn_handle_t *h_ptr,
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

static INLINE void opsm4_action_on_write_field_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 mem_type,
                                                    UINT32 reg,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_action_on_write_field_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
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

static INLINE void opsm4_burst_read( opsm4_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 len,
                                     UINT32 *value ) ALWAYS_INLINE;
static INLINE void opsm4_burst_read( opsm4_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
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

static INLINE void opsm4_burst_write( opsm4_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void opsm4_burst_write( opsm4_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
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

static INLINE PMC_POLL_RETURN_TYPE opsm4_poll( opsm4_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 mem_type,
                                               UINT32 reg,
                                               UINT32 mask,
                                               UINT32 value,
                                               PMC_POLL_COMPARISON_TYPE cmp,
                                               UINT32 max_count,
                                               UINT32 *num_failed_polls,
                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_poll( opsm4_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
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
 *  register access functions for opsm4
 * ==================================================================================
 */

static INLINE void opsm4_reg_TX_GLOBAL_CTRL_REG_write( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_GLOBAL_CTRL_REG_write( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_GLOBAL_CTRL_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG,
                     value);
}

static INLINE void opsm4_reg_TX_GLOBAL_CTRL_REG_field_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_GLOBAL_CTRL_REG_field_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_GLOBAL_CTRL_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG,
                     mask,
                     PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_GLOBAL_CTRL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_GLOBAL_CTRL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_GLOBAL_CTRL_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_TX_OTUK_AIS_CTRL_REG_write( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_OTUK_AIS_CTRL_REG_write( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_OTUK_AIS_CTRL_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_OTUK_AIS_CTRL_REG,
                     value);
}

static INLINE void opsm4_reg_TX_OTUK_AIS_CTRL_REG_field_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_OTUK_AIS_CTRL_REG_field_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 mask,
                                                             UINT32 ofs,
                                                             UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_OTUK_AIS_CTRL_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_OTUK_AIS_CTRL_REG,
                     mask,
                     PMC_OPSM4_REG_TX_OTUK_AIS_CTRL_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_OTUK_AIS_CTRL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_OTUK_AIS_CTRL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_OTUK_AIS_CTRL_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_OTUK_AIS_CTRL_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING0_REG_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING0_REG_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_L2P_LANE_MAPPING0_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG,
                     value);
}

static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG,
                     mask,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING1_REG_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING1_REG_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_L2P_LANE_MAPPING1_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG,
                     value);
}

static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG,
                     mask,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING2_REG_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING2_REG_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_L2P_LANE_MAPPING2_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG,
                     value);
}

static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG,
                     mask,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING3_REG_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING3_REG_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_L2P_LANE_MAPPING3_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG,
                     value);
}

static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING3_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_L2P_LANE_MAPPING3_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_L2P_LANE_MAPPING3_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG,
                     mask,
                     PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_L2P_LANE_MAPPING3_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_L2P_LANE_MAPPING3_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_L2P_LANE_MAPPING3_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_write( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_write( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG,
                     value);
}

static INLINE void opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_field_set( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG,
                     mask,
                     PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_TX_CONFIG_REG1_write( opsm4_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_CONFIG_REG1_write( opsm4_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_CONFIG_REG1_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_CONFIG_REG1,
                     value);
}

static INLINE void opsm4_reg_TX_CONFIG_REG1_field_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_CONFIG_REG1_field_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_CONFIG_REG1_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_CONFIG_REG1,
                     mask,
                     PMC_OPSM4_REG_TX_CONFIG_REG1_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_CONFIG_REG1_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_CONFIG_REG1_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_CONFIG_REG1);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_CONFIG_REG1_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_RX_GLOBAL_CTRL_REG_write( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_GLOBAL_CTRL_REG_write( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_GLOBAL_CTRL_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG,
                     value);
}

static INLINE void opsm4_reg_RX_GLOBAL_CTRL_REG_field_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_GLOBAL_CTRL_REG_field_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_GLOBAL_CTRL_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG,
                     mask,
                     PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_RX_GLOBAL_CTRL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_GLOBAL_CTRL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_RX_GLOBAL_CTRL_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_RX_OTU4_CTRL_REG_write( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_OTU4_CTRL_REG_write( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_OTU4_CTRL_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_OTU4_CTRL_REG,
                     value);
}

static INLINE void opsm4_reg_RX_OTU4_CTRL_REG_field_set( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_OTU4_CTRL_REG_field_set( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 mask,
                                                         UINT32 ofs,
                                                         UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_OTU4_CTRL_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_OTU4_CTRL_REG,
                     mask,
                     PMC_OPSM4_REG_RX_OTU4_CTRL_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_RX_OTU4_CTRL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_OTU4_CTRL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_RX_OTU4_CTRL_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_RX_OTU4_CTRL_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_RX_PTP_CONFIG_write( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_PTP_CONFIG_write( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_PTP_CONFIG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_PTP_CONFIG,
                     value);
}

static INLINE void opsm4_reg_RX_PTP_CONFIG_field_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_PTP_CONFIG_field_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_PTP_CONFIG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_PTP_CONFIG,
                     mask,
                     PMC_OPSM4_REG_RX_PTP_CONFIG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_RX_PTP_CONFIG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_PTP_CONFIG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_RX_PTP_CONFIG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_RX_PTP_CONFIG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_LPBK_CTRL_write( opsm4_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_LPBK_CTRL_write( opsm4_buffer_t *b_ptr,
                                              lineotn_handle_t *h_ptr,
                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_LPBK_CTRL_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_LPBK_CTRL,
                     value);
}

static INLINE void opsm4_reg_LPBK_CTRL_field_set( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_LPBK_CTRL_field_set( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 mask,
                                                  UINT32 ofs,
                                                  UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_LPBK_CTRL_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_LPBK_CTRL,
                     mask,
                     PMC_OPSM4_REG_LPBK_CTRL_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_LPBK_CTRL_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_LPBK_CTRL_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_LPBK_CTRL);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_LPBK_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_TX_SYS_CTRL_write( opsm4_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_SYS_CTRL_write( opsm4_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_SYS_CTRL_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_SYS_CTRL,
                     value);
}

static INLINE void opsm4_reg_TX_SYS_CTRL_field_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_SYS_CTRL_field_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 mask,
                                                    UINT32 ofs,
                                                    UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_SYS_CTRL_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_TX_SYS_CTRL,
                     mask,
                     PMC_OPSM4_REG_TX_SYS_CTRL_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_SYS_CTRL_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_SYS_CTRL_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_TX_SYS_CTRL);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_SYS_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_RX_CTRL_write( opsm4_buffer_t *b_ptr,
                                            lineotn_handle_t *h_ptr,
                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_CTRL_write( opsm4_buffer_t *b_ptr,
                                            lineotn_handle_t *h_ptr,
                                            UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_CTRL_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_CTRL,
                     value);
}

static INLINE void opsm4_reg_RX_CTRL_field_set( opsm4_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_CTRL_field_set( opsm4_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 mask,
                                                UINT32 ofs,
                                                UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_CTRL_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_CTRL,
                     mask,
                     PMC_OPSM4_REG_RX_CTRL_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_RX_CTRL_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_CTRL_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_RX_CTRL);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_RX_CTRL_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_write( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_write( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG(N),
                     value);
}

static INLINE void opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 mask,
                                                                           UINT32 ofs,
                                                                           UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set", N, mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_CONFIG,
                     PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG(N),
                     mask,
                     PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_CONFIG,
                                PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE void opsm4_reg_TX_INT_EN_REG_write( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_INT_EN_REG_write( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_INT_EN_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_TX_INT_EN_REG,
                     value);
}

static INLINE void opsm4_reg_TX_INT_EN_REG_field_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_INT_EN_REG_field_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 mask,
                                                      UINT32 ofs,
                                                      UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_INT_EN_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_TX_INT_EN_REG,
                     mask,
                     PMC_OPSM4_REG_TX_INT_EN_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_TX_INT_EN_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_INT_EN_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_TX_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_RX_OTU4_INT_EN_REG_write( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_OTU4_INT_EN_REG_write( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_OTU4_INT_EN_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_RX_OTU4_INT_EN_REG,
                     value);
}

static INLINE void opsm4_reg_RX_OTU4_INT_EN_REG_field_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_OTU4_INT_EN_REG_field_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 mask,
                                                           UINT32 ofs,
                                                           UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_OTU4_INT_EN_REG_field_set", mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_RX_OTU4_INT_EN_REG,
                     mask,
                     PMC_OPSM4_REG_RX_OTU4_INT_EN_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_RX_OTU4_INT_EN_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_OTU4_INT_EN_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_RX_OTU4_INT_EN_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_RX_OTU4_INT_EN_REG_read", reg_value);
    return reg_value;
}

static INLINE void opsm4_reg_RX_LANE_INT_EN_REG_array_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_LANE_INT_EN_REG_array_write( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_LANE_INT_EN_REG_array_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_RX_LANE_INT_EN_REG(N),
                     value);
}

static INLINE void opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_LANE_INT_EN_REG_array_field_set", N, mask, ofs, value );
    opsm4_field_set( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_RX_LANE_INT_EN_REG(N),
                     mask,
                     PMC_OPSM4_REG_RX_LANE_INT_EN_REG_UNUSED_MASK,
                     ofs,
                     value);

}

static INLINE UINT32 opsm4_reg_RX_LANE_INT_EN_REG_array_read( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_LANE_INT_EN_REG_array_read( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_RX_LANE_INT_EN_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "opsm4_reg_RX_LANE_INT_EN_REG_array_read", reg_value, N);
    return reg_value;
}

static INLINE UINT32 opsm4_reg_OPSM4_INT_SUMMARY_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_OPSM4_INT_SUMMARY_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_OPSM4_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_OPSM4_INT_SUMMARY_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_OPSM4_INT_SUMMARY_poll( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_OPSM4_INT_SUMMARY_poll( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 mask,
                                                                     UINT32 value,
                                                                     PMC_POLL_COMPARISON_TYPE cmp,
                                                                     UINT32 max_count,
                                                                     UINT32 *num_failed_polls,
                                                                     UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm4_reg_OPSM4_INT_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm4_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OPSM4_REG_OPSM4_INT_SUMMARY,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm4_reg_OPSM4_INT_SUMMARY1_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_OPSM4_INT_SUMMARY1_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_OPSM4_INT_SUMMARY1);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_OPSM4_INT_SUMMARY1_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_OPSM4_INT_SUMMARY1_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_OPSM4_INT_SUMMARY1_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 mask,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm4_reg_OPSM4_INT_SUMMARY1_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm4_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OPSM4_REG_OPSM4_INT_SUMMARY1,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE void opsm4_reg_TX_INT_REG_write( opsm4_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_INT_REG_write( opsm4_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_TX_INT_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_TX_INT_REG,
                     value);
}

static INLINE void opsm4_reg_TX_INT_REG_action_on_write_field_set( opsm4_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_TX_INT_REG_action_on_write_field_set( opsm4_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 ofs,
                                                                   UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_TX_INT_REG_action_on_write_field_set", mask, ofs, value );
    opsm4_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OPSM4_REG_TX_INT_REG,
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 opsm4_reg_TX_INT_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_INT_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_TX_INT_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_TX_INT_REG_poll( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_TX_INT_REG_poll( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 mask,
                                                              UINT32 value,
                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                              UINT32 max_count,
                                                              UINT32 *num_failed_polls,
                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm4_reg_TX_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm4_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OPSM4_REG_TX_INT_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE void opsm4_reg_RX_OTU4_INT_REG_write( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_OTU4_INT_REG_write( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_OTU4_INT_REG_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_RX_OTU4_INT_REG,
                     value);
}

static INLINE void opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 mask,
                                                                        UINT32 ofs,
                                                                        UINT32 value )
{
    IOLOG( "%s <- mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set", mask, ofs, value );
    opsm4_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OPSM4_REG_RX_OTU4_INT_REG,
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 opsm4_reg_RX_OTU4_INT_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_OTU4_INT_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_RX_OTU4_INT_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_RX_OTU4_INT_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_RX_OTU4_INT_REG_poll( opsm4_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_RX_OTU4_INT_REG_poll( opsm4_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm4_reg_RX_OTU4_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm4_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OPSM4_REG_RX_OTU4_INT_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE void opsm4_reg_RX_LANE_INT_REG_array_write( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_LANE_INT_REG_array_write( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm4_reg_RX_LANE_INT_REG_array_write", value );
    opsm4_reg_write( b_ptr,
                     h_ptr,
                     MEM_TYPE_STATUS,
                     PMC_OPSM4_REG_RX_LANE_INT_REG(N),
                     value);
}

static INLINE void opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( opsm4_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( opsm4_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 ofs,
                                                                              UINT32 value )
{
    IOLOG( "%s <- N=%d mask=0x%08x ofs=%d value=0x%08x", "opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set", N, mask, ofs, value );
    opsm4_action_on_write_field_set( b_ptr,
                                     h_ptr,
                                     MEM_TYPE_STATUS,
                                     PMC_OPSM4_REG_RX_LANE_INT_REG(N),
                                     mask,
                                     ofs,
                                     value);

}

static INLINE UINT32 opsm4_reg_RX_LANE_INT_REG_array_read( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_LANE_INT_REG_array_read( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_RX_LANE_INT_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "opsm4_reg_RX_LANE_INT_REG_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_RX_LANE_INT_REG_array_poll( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_RX_LANE_INT_REG_array_poll( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32  N,
                                                                         UINT32 mask,
                                                                         UINT32 value,
                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                         UINT32 max_count,
                                                                         UINT32 *num_failed_polls,
                                                                         UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm4_reg_RX_LANE_INT_REG_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm4_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OPSM4_REG_RX_LANE_INT_REG(N),
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm4_reg_TX_INT_VAL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_TX_INT_VAL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_TX_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_TX_INT_VAL_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_TX_INT_VAL_REG_poll( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_TX_INT_VAL_REG_poll( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 mask,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm4_reg_TX_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm4_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OPSM4_REG_TX_INT_VAL_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm4_reg_RX_OTU4_INT_VAL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_OTU4_INT_VAL_REG_read( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x;", "opsm4_reg_RX_OTU4_INT_VAL_REG_read", reg_value);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_RX_OTU4_INT_VAL_REG_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_RX_OTU4_INT_VAL_REG_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm4_reg_RX_OTU4_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm4_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG,
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm4_reg_RX_LANE_INT_VAL_REG_array_read( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_LANE_INT_VAL_REG_array_read( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_RX_LANE_INT_VAL_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "opsm4_reg_RX_LANE_INT_VAL_REG_array_read", reg_value, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( opsm4_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( opsm4_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32  N,
                                                                             UINT32 mask,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm4_reg_RX_LANE_INT_VAL_REG_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm4_poll( b_ptr,
                       h_ptr,
                       MEM_TYPE_STATUS,
                       PMC_OPSM4_REG_RX_LANE_INT_VAL_REG(N),
                       mask,
                       value,
                       cmp,
                       max_count,
                       num_failed_polls,
                       delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm4_reg_RX_LANE_ID_OR_SKEW_REG_array_read( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_reg_RX_LANE_ID_OR_SKEW_REG_array_read( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm4_reg_read( b_ptr,
                                h_ptr,
                                MEM_TYPE_STATUS,
                                PMC_OPSM4_REG_RX_LANE_ID_OR_SKEW_REG(N));

    IOLOG( "%s -> 0x%08x; N=%d", "opsm4_reg_RX_LANE_ID_OR_SKEW_REG_array_read", reg_value, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void opsm4_field_TX_SCRAMBLE_ENABLE_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SCRAMBLE_ENABLE_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SCRAMBLE_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SCRAMBLE_ENABLE_set", value );

    /* (0x00014004 bits 30) field TX_SCRAMBLE_ENABLE of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    opsm4_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_MSK,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_TX_SCRAMBLE_ENABLE_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SCRAMBLE_ENABLE_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014004 bits 30) field TX_SCRAMBLE_ENABLE of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_TX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_MSK) >> OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SCRAMBLE_ENABLE_get", value );

    return value;
}
static INLINE void opsm4_field_TX_HARDWARE_RESET_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_HARDWARE_RESET_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_HARDWARE_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_HARDWARE_RESET_set", value );

    /* (0x00014004 bits 29) field TX_HARDWARE_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    opsm4_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_MSK,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_TX_HARDWARE_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_HARDWARE_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014004 bits 29) field TX_HARDWARE_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_TX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_MSK) >> OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_HARDWARE_RESET_get", value );

    return value;
}
static INLINE void opsm4_field_TX_FIFO_RESET_set( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_FIFO_RESET_set( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_FIFO_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_FIFO_RESET_set", value );

    /* (0x00014004 bits 27) field TX_FIFO_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    opsm4_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_FIFO_RESET_MSK,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_FIFO_RESET_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_TX_FIFO_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_FIFO_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014004 bits 27) field TX_FIFO_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_TX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_FIFO_RESET_MSK) >> OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_FIFO_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_FIFO_RESET_get", value );

    return value;
}
static INLINE void opsm4_field_TX_DATA_IN_SCRAMBLED_set( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_DATA_IN_SCRAMBLED_set( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_DATA_IN_SCRAMBLED_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_DATA_IN_SCRAMBLED_set", value );

    /* (0x00014004 bits 25) field TX_DATA_IN_SCRAMBLED of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    opsm4_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_MSK,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_TX_DATA_IN_SCRAMBLED_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_DATA_IN_SCRAMBLED_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014004 bits 25) field TX_DATA_IN_SCRAMBLED of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_TX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_MSK) >> OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_DATA_IN_SCRAMBLED_get", value );

    return value;
}
static INLINE void opsm4_field_TX_LINE_SOFT_RESET_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LINE_SOFT_RESET_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LINE_SOFT_RESET_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LINE_SOFT_RESET_set", value );

    /* (0x00014004 bits 9:0) field TX_LINE_SOFT_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    opsm4_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK,
                                            OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_TX_LINE_SOFT_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LINE_SOFT_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014004 bits 9:0) field TX_LINE_SOFT_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_TX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK) >> OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LINE_SOFT_RESET_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_LINE_SOFT_RESET_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_LINE_SOFT_RESET_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LINE_SOFT_RESET_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LINE_SOFT_RESET_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LINE_SOFT_RESET_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014004 bits 9:0) field TX_LINE_SOFT_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
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
        /* (0x00014004 bits 9:0) field TX_LINE_SOFT_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
        opsm4_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                h_ptr,
                                                subfield_mask << (OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF + subfield_offset),
                                                OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF + subfield_offset,
                                                value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_LINE_SOFT_RESET_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LINE_SOFT_RESET_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LINE_SOFT_RESET_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LINE_SOFT_RESET_get", stop_bit, 9 );
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
    /* (0x00014004 bits 9:0) field TX_LINE_SOFT_RESET of register PMC_OPSM4_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_TX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK)
                  >> OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK, OPSM4_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LINE_SOFT_RESET_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_FORCE_LANE_OTUK_AIS_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_FORCE_LANE_OTUK_AIS_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_FORCE_LANE_OTUK_AIS_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_FORCE_LANE_OTUK_AIS_set", value );

    /* (0x00014008 bits 12) field TX_FORCE_LANE_OTUK_AIS of register PMC_OPSM4_REG_TX_OTUK_AIS_CTRL_REG */
    opsm4_reg_TX_OTUK_AIS_CTRL_REG_field_set( b_ptr,
                                              h_ptr,
                                              OPSM4_REG_TX_OTUK_AIS_CTRL_REG_BIT_TX_FORCE_LANE_OTUK_AIS_MSK,
                                              OPSM4_REG_TX_OTUK_AIS_CTRL_REG_BIT_TX_FORCE_LANE_OTUK_AIS_OFF,
                                              value);
}

static INLINE UINT32 opsm4_field_TX_FORCE_LANE_OTUK_AIS_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_FORCE_LANE_OTUK_AIS_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014008 bits 12) field TX_FORCE_LANE_OTUK_AIS of register PMC_OPSM4_REG_TX_OTUK_AIS_CTRL_REG */
    reg_value = opsm4_reg_TX_OTUK_AIS_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_OTUK_AIS_CTRL_REG_BIT_TX_FORCE_LANE_OTUK_AIS_MSK) >> OPSM4_REG_TX_OTUK_AIS_CTRL_REG_BIT_TX_FORCE_LANE_OTUK_AIS_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_FORCE_LANE_OTUK_AIS_get", value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_5_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_5_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_5_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_5_SELECT_set", value );

    /* (0x0001401c bits 29:25) field TX_PHYSICAL_LANE_5_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_5_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_5_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001401c bits 29:25) field TX_PHYSICAL_LANE_5_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_5_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001401c bits 29:25) field TX_PHYSICAL_LANE_5_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
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
        /* (0x0001401c bits 29:25) field TX_PHYSICAL_LANE_5_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_get", stop_bit, 4 );
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
    /* (0x0001401c bits 29:25) field TX_PHYSICAL_LANE_5_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_5_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_4_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_4_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_4_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_4_SELECT_set", value );

    /* (0x0001401c bits 24:20) field TX_PHYSICAL_LANE_4_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_4_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_4_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001401c bits 24:20) field TX_PHYSICAL_LANE_4_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_4_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001401c bits 24:20) field TX_PHYSICAL_LANE_4_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
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
        /* (0x0001401c bits 24:20) field TX_PHYSICAL_LANE_4_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_get", stop_bit, 4 );
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
    /* (0x0001401c bits 24:20) field TX_PHYSICAL_LANE_4_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_4_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_3_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_3_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_3_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_3_SELECT_set", value );

    /* (0x0001401c bits 19:15) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_3_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_3_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001401c bits 19:15) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_3_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001401c bits 19:15) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
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
        /* (0x0001401c bits 19:15) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_get", stop_bit, 4 );
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
    /* (0x0001401c bits 19:15) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_2_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_2_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_2_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_2_SELECT_set", value );

    /* (0x0001401c bits 14:10) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_2_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_2_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001401c bits 14:10) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_2_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001401c bits 14:10) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
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
        /* (0x0001401c bits 14:10) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_get", stop_bit, 4 );
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
    /* (0x0001401c bits 14:10) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_1_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_1_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_1_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_1_SELECT_set", value );

    /* (0x0001401c bits 9:5) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_1_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_1_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001401c bits 9:5) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_1_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001401c bits 9:5) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
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
        /* (0x0001401c bits 9:5) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_get", stop_bit, 4 );
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
    /* (0x0001401c bits 9:5) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_0_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_0_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_0_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_0_SELECT_set", value );

    /* (0x0001401c bits 4:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_0_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_0_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001401c bits 4:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_0_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001401c bits 4:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
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
        /* (0x0001401c bits 4:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_get", stop_bit, 4 );
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
    /* (0x0001401c bits 4:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING0_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING0_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_11_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_11_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_11_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_11_SELECT_set", value );

    /* (0x00014020 bits 29:25) field TX_PHYSICAL_LANE_11_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_11_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_11_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014020 bits 29:25) field TX_PHYSICAL_LANE_11_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_11_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014020 bits 29:25) field TX_PHYSICAL_LANE_11_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
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
        /* (0x00014020 bits 29:25) field TX_PHYSICAL_LANE_11_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_get", stop_bit, 4 );
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
    /* (0x00014020 bits 29:25) field TX_PHYSICAL_LANE_11_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_11_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_10_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_10_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_10_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_10_SELECT_set", value );

    /* (0x00014020 bits 24:20) field TX_PHYSICAL_LANE_10_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_10_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_10_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014020 bits 24:20) field TX_PHYSICAL_LANE_10_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_10_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014020 bits 24:20) field TX_PHYSICAL_LANE_10_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
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
        /* (0x00014020 bits 24:20) field TX_PHYSICAL_LANE_10_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_get", stop_bit, 4 );
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
    /* (0x00014020 bits 24:20) field TX_PHYSICAL_LANE_10_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_10_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_9_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_9_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_9_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_9_SELECT_set", value );

    /* (0x00014020 bits 19:15) field TX_PHYSICAL_LANE_9_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_9_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_9_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014020 bits 19:15) field TX_PHYSICAL_LANE_9_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_9_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014020 bits 19:15) field TX_PHYSICAL_LANE_9_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
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
        /* (0x00014020 bits 19:15) field TX_PHYSICAL_LANE_9_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_get", stop_bit, 4 );
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
    /* (0x00014020 bits 19:15) field TX_PHYSICAL_LANE_9_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_9_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_8_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_8_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_8_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_8_SELECT_set", value );

    /* (0x00014020 bits 14:10) field TX_PHYSICAL_LANE_8_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_8_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_8_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014020 bits 14:10) field TX_PHYSICAL_LANE_8_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_8_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014020 bits 14:10) field TX_PHYSICAL_LANE_8_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
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
        /* (0x00014020 bits 14:10) field TX_PHYSICAL_LANE_8_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_get", stop_bit, 4 );
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
    /* (0x00014020 bits 14:10) field TX_PHYSICAL_LANE_8_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_8_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_7_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_7_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_7_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_7_SELECT_set", value );

    /* (0x00014020 bits 9:5) field TX_PHYSICAL_LANE_7_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_7_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_7_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014020 bits 9:5) field TX_PHYSICAL_LANE_7_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_7_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014020 bits 9:5) field TX_PHYSICAL_LANE_7_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
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
        /* (0x00014020 bits 9:5) field TX_PHYSICAL_LANE_7_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_get", stop_bit, 4 );
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
    /* (0x00014020 bits 9:5) field TX_PHYSICAL_LANE_7_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_7_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_6_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_6_SELECT_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_6_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_6_SELECT_set", value );

    /* (0x00014020 bits 4:0) field TX_PHYSICAL_LANE_6_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_6_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_6_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014020 bits 4:0) field TX_PHYSICAL_LANE_6_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_6_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014020 bits 4:0) field TX_PHYSICAL_LANE_6_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
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
        /* (0x00014020 bits 4:0) field TX_PHYSICAL_LANE_6_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_get", stop_bit, 4 );
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
    /* (0x00014020 bits 4:0) field TX_PHYSICAL_LANE_6_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING1_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING1_REG_BIT_TX_PHYSICAL_LANE_6_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_17_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_17_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_17_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_17_SELECT_set", value );

    /* (0x00014024 bits 29:25) field TX_PHYSICAL_LANE_17_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_17_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_17_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014024 bits 29:25) field TX_PHYSICAL_LANE_17_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_17_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014024 bits 29:25) field TX_PHYSICAL_LANE_17_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
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
        /* (0x00014024 bits 29:25) field TX_PHYSICAL_LANE_17_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_get", stop_bit, 4 );
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
    /* (0x00014024 bits 29:25) field TX_PHYSICAL_LANE_17_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_17_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_16_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_16_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_16_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_16_SELECT_set", value );

    /* (0x00014024 bits 24:20) field TX_PHYSICAL_LANE_16_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_16_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_16_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014024 bits 24:20) field TX_PHYSICAL_LANE_16_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_16_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014024 bits 24:20) field TX_PHYSICAL_LANE_16_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
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
        /* (0x00014024 bits 24:20) field TX_PHYSICAL_LANE_16_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_get", stop_bit, 4 );
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
    /* (0x00014024 bits 24:20) field TX_PHYSICAL_LANE_16_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_16_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_15_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_15_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_15_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_15_SELECT_set", value );

    /* (0x00014024 bits 19:15) field TX_PHYSICAL_LANE_15_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_15_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_15_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014024 bits 19:15) field TX_PHYSICAL_LANE_15_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_15_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014024 bits 19:15) field TX_PHYSICAL_LANE_15_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
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
        /* (0x00014024 bits 19:15) field TX_PHYSICAL_LANE_15_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_get", stop_bit, 4 );
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
    /* (0x00014024 bits 19:15) field TX_PHYSICAL_LANE_15_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_15_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_14_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_14_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_14_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_14_SELECT_set", value );

    /* (0x00014024 bits 14:10) field TX_PHYSICAL_LANE_14_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_14_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_14_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014024 bits 14:10) field TX_PHYSICAL_LANE_14_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_14_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014024 bits 14:10) field TX_PHYSICAL_LANE_14_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
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
        /* (0x00014024 bits 14:10) field TX_PHYSICAL_LANE_14_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_get", stop_bit, 4 );
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
    /* (0x00014024 bits 14:10) field TX_PHYSICAL_LANE_14_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_14_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_13_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_13_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_13_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_13_SELECT_set", value );

    /* (0x00014024 bits 9:5) field TX_PHYSICAL_LANE_13_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_13_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_13_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014024 bits 9:5) field TX_PHYSICAL_LANE_13_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_13_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014024 bits 9:5) field TX_PHYSICAL_LANE_13_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
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
        /* (0x00014024 bits 9:5) field TX_PHYSICAL_LANE_13_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_get", stop_bit, 4 );
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
    /* (0x00014024 bits 9:5) field TX_PHYSICAL_LANE_13_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_13_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_12_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_12_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_12_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_12_SELECT_set", value );

    /* (0x00014024 bits 4:0) field TX_PHYSICAL_LANE_12_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_12_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_12_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014024 bits 4:0) field TX_PHYSICAL_LANE_12_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_12_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014024 bits 4:0) field TX_PHYSICAL_LANE_12_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
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
        /* (0x00014024 bits 4:0) field TX_PHYSICAL_LANE_12_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_get", stop_bit, 4 );
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
    /* (0x00014024 bits 4:0) field TX_PHYSICAL_LANE_12_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING2_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING2_REG_BIT_TX_PHYSICAL_LANE_12_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_19_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_19_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_19_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_19_SELECT_set", value );

    /* (0x00014028 bits 9:5) field TX_PHYSICAL_LANE_19_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING3_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_19_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_19_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014028 bits 9:5) field TX_PHYSICAL_LANE_19_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING3_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_19_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014028 bits 9:5) field TX_PHYSICAL_LANE_19_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
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
        /* (0x00014028 bits 9:5) field TX_PHYSICAL_LANE_19_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING3_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_get", stop_bit, 4 );
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
    /* (0x00014028 bits 9:5) field TX_PHYSICAL_LANE_19_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING3_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_19_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_PHYSICAL_LANE_18_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_PHYSICAL_LANE_18_SELECT_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_18_SELECT_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_18_SELECT_set", value );

    /* (0x00014028 bits 4:0) field TX_PHYSICAL_LANE_18_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
    opsm4_reg_TX_L2P_LANE_MAPPING3_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_MSK,
                                                  OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_18_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_PHYSICAL_LANE_18_SELECT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014028 bits 4:0) field TX_PHYSICAL_LANE_18_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING3_REG_read( b_ptr,
                                                         h_ptr);
    value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_MSK) >> OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_PHYSICAL_LANE_18_SELECT_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_set( opsm4_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014028 bits 4:0) field TX_PHYSICAL_LANE_18_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
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
        /* (0x00014028 bits 4:0) field TX_PHYSICAL_LANE_18_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
        opsm4_reg_TX_L2P_LANE_MAPPING3_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      subfield_mask << (OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_OFF + subfield_offset),
                                                      OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_get", stop_bit, 4 );
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
    /* (0x00014028 bits 4:0) field TX_PHYSICAL_LANE_18_SELECT of register PMC_OPSM4_REG_TX_L2P_LANE_MAPPING3_REG */
    reg_value = opsm4_reg_TX_L2P_LANE_MAPPING3_REG_read( b_ptr,
                                                         h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_MSK)
                  >> OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_MSK, OPSM4_REG_TX_L2P_LANE_MAPPING3_REG_BIT_TX_PHYSICAL_LANE_18_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_set", value );

    /* (0x00014030 bits 14:10) field TX_LINE_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
    opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_MSK,
                                                   OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_OFF,
                                                   value);
}

static INLINE UINT32 opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014030 bits 14:10) field TX_LINE_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
    reg_value = opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_MSK) >> OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_set( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_set( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014030 bits 14:10) field TX_LINE_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
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
        /* (0x00014030 bits 14:10) field TX_LINE_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
        opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_OFF + subfield_offset),
                                                       OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_get( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_get( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_get", stop_bit, 4 );
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
    /* (0x00014030 bits 14:10) field TX_LINE_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
    reg_value = opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_MSK)
                  >> OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_MSK, OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_FIFO_XOFF_FILL_LEVEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_LINE_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LINE_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LINE_DEQUEUE_EN_set", value, 15);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LINE_DEQUEUE_EN_set", value );

    /* (0x00014030 bits 9:6) field TX_LINE_DEQUEUE_EN of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
    opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_field_set( b_ptr,
                                                   h_ptr,
                                                   OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_MSK,
                                                   OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_OFF,
                                                   value);
}

static INLINE UINT32 opsm4_field_TX_LINE_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LINE_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014030 bits 9:6) field TX_LINE_DEQUEUE_EN of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
    reg_value = opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_read( b_ptr,
                                                          h_ptr);
    value = (reg_value & OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_MSK) >> OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LINE_DEQUEUE_EN_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_LINE_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_LINE_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LINE_DEQUEUE_EN_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LINE_DEQUEUE_EN_set", stop_bit, 3 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LINE_DEQUEUE_EN_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014030 bits 9:6) field TX_LINE_DEQUEUE_EN of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
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
        /* (0x00014030 bits 9:6) field TX_LINE_DEQUEUE_EN of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
        opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_field_set( b_ptr,
                                                       h_ptr,
                                                       subfield_mask << (OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_OFF + subfield_offset),
                                                       OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_OFF + subfield_offset,
                                                       value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_LINE_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LINE_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LINE_DEQUEUE_EN_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LINE_DEQUEUE_EN_get", stop_bit, 3 );
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
    /* (0x00014030 bits 9:6) field TX_LINE_DEQUEUE_EN of register PMC_OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG */
    reg_value = opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_read( b_ptr,
                                                          h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_MSK)
                  >> OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_MSK, OPSM4_REG_TX_ASYNC_FIFO_SETTING_REG_BIT_TX_LINE_DEQUEUE_EN_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LINE_DEQUEUE_EN_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_FP_MFAS_INVERT_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_FP_MFAS_INVERT_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_FP_MFAS_INVERT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_FP_MFAS_INVERT_set", value );

    /* (0x00014034 bits 13) field TX_FP_MFAS_INVERT of register PMC_OPSM4_REG_TX_CONFIG_REG1 */
    opsm4_reg_TX_CONFIG_REG1_field_set( b_ptr,
                                        h_ptr,
                                        OPSM4_REG_TX_CONFIG_REG1_BIT_TX_FP_MFAS_INVERT_MSK,
                                        OPSM4_REG_TX_CONFIG_REG1_BIT_TX_FP_MFAS_INVERT_OFF,
                                        value);
}

static INLINE UINT32 opsm4_field_TX_FP_MFAS_INVERT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_FP_MFAS_INVERT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014034 bits 13) field TX_FP_MFAS_INVERT of register PMC_OPSM4_REG_TX_CONFIG_REG1 */
    reg_value = opsm4_reg_TX_CONFIG_REG1_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_CONFIG_REG1_BIT_TX_FP_MFAS_INVERT_MSK) >> OPSM4_REG_TX_CONFIG_REG1_BIT_TX_FP_MFAS_INVERT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_FP_MFAS_INVERT_get", value );

    return value;
}
static INLINE void opsm4_field_RX_OTL_4_10_ENABLE_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_OTL_4_10_ENABLE_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_OTL_4_10_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_OTL_4_10_ENABLE_set", value );

    /* (0x00014074 bits 13) field RX_OTL_4_10_ENABLE of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    opsm4_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_4_10_ENABLE_MSK,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_4_10_ENABLE_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_OTL_4_10_ENABLE_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_OTL_4_10_ENABLE_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014074 bits 13) field RX_OTL_4_10_ENABLE of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_RX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_4_10_ENABLE_MSK) >> OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_4_10_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_OTL_4_10_ENABLE_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SCRAMBLE_ENABLE_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SCRAMBLE_ENABLE_set( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SCRAMBLE_ENABLE_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SCRAMBLE_ENABLE_set", value );

    /* (0x00014074 bits 12) field RX_SCRAMBLE_ENABLE of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    opsm4_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_MSK,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SCRAMBLE_ENABLE_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SCRAMBLE_ENABLE_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014074 bits 12) field RX_SCRAMBLE_ENABLE of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_RX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_MSK) >> OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SCRAMBLE_ENABLE_get", value );

    return value;
}
static INLINE void opsm4_field_RX_HARDWARE_RESET_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_HARDWARE_RESET_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_HARDWARE_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_HARDWARE_RESET_set", value );

    /* (0x00014074 bits 11) field RX_HARDWARE_RESET of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    opsm4_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_MSK,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_HARDWARE_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_HARDWARE_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014074 bits 11) field RX_HARDWARE_RESET of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_RX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_MSK) >> OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_HARDWARE_RESET_get", value );

    return value;
}
static INLINE void opsm4_field_RX_FIFO_RESET_set( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FIFO_RESET_set( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FIFO_RESET_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_FIFO_RESET_set", value );

    /* (0x00014074 bits 9) field RX_FIFO_RESET of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    opsm4_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_FIFO_RESET_MSK,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_FIFO_RESET_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_FIFO_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FIFO_RESET_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014074 bits 9) field RX_FIFO_RESET of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_RX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_FIFO_RESET_MSK) >> OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_FIFO_RESET_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_FIFO_RESET_get", value );

    return value;
}
static INLINE void opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set", value );

    /* (0x00014074 bits 6) field RX_DATA_IN_NOT_SCRAMBLED of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    opsm4_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_MSK,
                                            OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014074 bits 6) field RX_DATA_IN_NOT_SCRAMBLED of register PMC_OPSM4_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm4_reg_RX_GLOBAL_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_MSK) >> OPSM4_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_get", value );

    return value;
}
static INLINE void opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_set", value, 255);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_set", value );

    /* (0x00014078 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM4_REG_RX_OTU4_CTRL_REG */
    opsm4_reg_RX_OTU4_CTRL_REG_field_set( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK,
                                          OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF,
                                          value);
}

static INLINE UINT32 opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014078 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM4_REG_RX_OTU4_CTRL_REG */
    reg_value = opsm4_reg_RX_OTU4_CTRL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK) >> OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_get", value );

    return value;
}
static INLINE void opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_set( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_set( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_set", stop_bit, 7 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014078 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM4_REG_RX_OTU4_CTRL_REG */
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
        /* (0x00014078 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM4_REG_RX_OTU4_CTRL_REG */
        opsm4_reg_RX_OTU4_CTRL_REG_field_set( b_ptr,
                                              h_ptr,
                                              subfield_mask << (OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF + subfield_offset),
                                              OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF + subfield_offset,
                                              value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_get", stop_bit, 7 );
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
    /* (0x00014078 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM4_REG_RX_OTU4_CTRL_REG */
    reg_value = opsm4_reg_RX_OTU4_CTRL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK)
                  >> OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK, OPSM4_REG_RX_OTU4_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_RX_FP_MFAS_GEN_INVERT_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FP_MFAS_GEN_INVERT_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FP_MFAS_GEN_INVERT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_FP_MFAS_GEN_INVERT_set", value );

    /* (0x00014320 bits 30) field RX_FP_MFAS_GEN_INVERT of register PMC_OPSM4_REG_RX_PTP_CONFIG */
    opsm4_reg_RX_PTP_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_RX_PTP_CONFIG_BIT_RX_FP_MFAS_GEN_INVERT_MSK,
                                       OPSM4_REG_RX_PTP_CONFIG_BIT_RX_FP_MFAS_GEN_INVERT_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_RX_FP_MFAS_GEN_INVERT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FP_MFAS_GEN_INVERT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014320 bits 30) field RX_FP_MFAS_GEN_INVERT of register PMC_OPSM4_REG_RX_PTP_CONFIG */
    reg_value = opsm4_reg_RX_PTP_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_PTP_CONFIG_BIT_RX_FP_MFAS_GEN_INVERT_MSK) >> OPSM4_REG_RX_PTP_CONFIG_BIT_RX_FP_MFAS_GEN_INVERT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_FP_MFAS_GEN_INVERT_get", value );

    return value;
}
static INLINE void opsm4_field_RX_FP_MFAS_INVERT_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FP_MFAS_INVERT_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FP_MFAS_INVERT_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_FP_MFAS_INVERT_set", value );

    /* (0x00014320 bits 29) field RX_FP_MFAS_INVERT of register PMC_OPSM4_REG_RX_PTP_CONFIG */
    opsm4_reg_RX_PTP_CONFIG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_RX_PTP_CONFIG_BIT_RX_FP_MFAS_INVERT_MSK,
                                       OPSM4_REG_RX_PTP_CONFIG_BIT_RX_FP_MFAS_INVERT_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_RX_FP_MFAS_INVERT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FP_MFAS_INVERT_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014320 bits 29) field RX_FP_MFAS_INVERT of register PMC_OPSM4_REG_RX_PTP_CONFIG */
    reg_value = opsm4_reg_RX_PTP_CONFIG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_PTP_CONFIG_BIT_RX_FP_MFAS_INVERT_MSK) >> OPSM4_REG_RX_PTP_CONFIG_BIT_RX_FP_MFAS_INVERT_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_FP_MFAS_INVERT_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SYS_LOOPBACK_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SYS_LOOPBACK_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SYS_LOOPBACK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SYS_LOOPBACK_set", value );

    /* (0x00014324 bits 1) field RX_SYS_LOOPBACK of register PMC_OPSM4_REG_LPBK_CTRL */
    opsm4_reg_LPBK_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   OPSM4_REG_LPBK_CTRL_BIT_RX_SYS_LOOPBACK_MSK,
                                   OPSM4_REG_LPBK_CTRL_BIT_RX_SYS_LOOPBACK_OFF,
                                   value);
}

static INLINE UINT32 opsm4_field_RX_SYS_LOOPBACK_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SYS_LOOPBACK_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014324 bits 1) field RX_SYS_LOOPBACK of register PMC_OPSM4_REG_LPBK_CTRL */
    reg_value = opsm4_reg_LPBK_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_LPBK_CTRL_BIT_RX_SYS_LOOPBACK_MSK) >> OPSM4_REG_LPBK_CTRL_BIT_RX_SYS_LOOPBACK_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SYS_LOOPBACK_get", value );

    return value;
}
static INLINE void opsm4_field_TX_SYS_LOOPBACK_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SYS_LOOPBACK_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SYS_LOOPBACK_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SYS_LOOPBACK_set", value );

    /* (0x00014324 bits 0) field TX_SYS_LOOPBACK of register PMC_OPSM4_REG_LPBK_CTRL */
    opsm4_reg_LPBK_CTRL_field_set( b_ptr,
                                   h_ptr,
                                   OPSM4_REG_LPBK_CTRL_BIT_TX_SYS_LOOPBACK_MSK,
                                   OPSM4_REG_LPBK_CTRL_BIT_TX_SYS_LOOPBACK_OFF,
                                   value);
}

static INLINE UINT32 opsm4_field_TX_SYS_LOOPBACK_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SYS_LOOPBACK_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014324 bits 0) field TX_SYS_LOOPBACK of register PMC_OPSM4_REG_LPBK_CTRL */
    reg_value = opsm4_reg_LPBK_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_LPBK_CTRL_BIT_TX_SYS_LOOPBACK_MSK) >> OPSM4_REG_LPBK_CTRL_BIT_TX_SYS_LOOPBACK_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SYS_LOOPBACK_get", value );

    return value;
}
static INLINE void opsm4_field_TX_SYS_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SYS_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SYS_DEQUEUE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SYS_DEQUEUE_EN_set", value );

    /* (0x00014328 bits 15) field TX_SYS_DEQUEUE_EN of register PMC_OPSM4_REG_TX_SYS_CTRL */
    opsm4_reg_TX_SYS_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     OPSM4_REG_TX_SYS_CTRL_BIT_TX_SYS_DEQUEUE_EN_MSK,
                                     OPSM4_REG_TX_SYS_CTRL_BIT_TX_SYS_DEQUEUE_EN_OFF,
                                     value);
}

static INLINE UINT32 opsm4_field_TX_SYS_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SYS_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014328 bits 15) field TX_SYS_DEQUEUE_EN of register PMC_OPSM4_REG_TX_SYS_CTRL */
    reg_value = opsm4_reg_TX_SYS_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_SYS_CTRL_BIT_TX_SYS_DEQUEUE_EN_MSK) >> OPSM4_REG_TX_SYS_CTRL_BIT_TX_SYS_DEQUEUE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SYS_DEQUEUE_EN_get", value );

    return value;
}
static INLINE void opsm4_field_SFI_TX_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_SFI_TX_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_SFI_TX_DEQUEUE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_SFI_TX_DEQUEUE_EN_set", value );

    /* (0x00014328 bits 14) field SFI_TX_DEQUEUE_EN of register PMC_OPSM4_REG_TX_SYS_CTRL */
    opsm4_reg_TX_SYS_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_DEQUEUE_EN_MSK,
                                     OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_DEQUEUE_EN_OFF,
                                     value);
}

static INLINE UINT32 opsm4_field_SFI_TX_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_SFI_TX_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014328 bits 14) field SFI_TX_DEQUEUE_EN of register PMC_OPSM4_REG_TX_SYS_CTRL */
    reg_value = opsm4_reg_TX_SYS_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_DEQUEUE_EN_MSK) >> OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_DEQUEUE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_SFI_TX_DEQUEUE_EN_get", value );

    return value;
}
static INLINE void opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 31)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_set", value, 31);
    IOLOG( "%s <= 0x%08x", "opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_set", value );

    /* (0x00014328 bits 4:0) field SFI_TX_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_SYS_CTRL */
    opsm4_reg_TX_SYS_CTRL_field_set( b_ptr,
                                     h_ptr,
                                     OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_MSK,
                                     OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_OFF,
                                     value);
}

static INLINE UINT32 opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014328 bits 4:0) field SFI_TX_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_SYS_CTRL */
    reg_value = opsm4_reg_TX_SYS_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_MSK) >> OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_get", value );

    return value;
}
static INLINE void opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_set( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_set( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_set", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_set", stop_bit, 4 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014328 bits 4:0) field SFI_TX_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_SYS_CTRL */
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
        /* (0x00014328 bits 4:0) field SFI_TX_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_SYS_CTRL */
        opsm4_reg_TX_SYS_CTRL_field_set( b_ptr,
                                         h_ptr,
                                         subfield_mask << (OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_OFF + subfield_offset),
                                         OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_OFF + subfield_offset,
                                         value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_get", stop_bit, 4 );
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
    /* (0x00014328 bits 4:0) field SFI_TX_FIFO_XOFF_FILL_LEVEL of register PMC_OPSM4_REG_TX_SYS_CTRL */
    reg_value = opsm4_reg_TX_SYS_CTRL_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_MSK)
                  >> OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_MSK, OPSM4_REG_TX_SYS_CTRL_BIT_SFI_TX_FIFO_XOFF_FILL_LEVEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_SFI_RX_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_SFI_RX_DEQUEUE_EN_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_SFI_RX_DEQUEUE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_SFI_RX_DEQUEUE_EN_set", value );

    /* (0x0001432c bits 17) field SFI_RX_DEQUEUE_EN of register PMC_OPSM4_REG_RX_CTRL */
    opsm4_reg_RX_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 OPSM4_REG_RX_CTRL_BIT_SFI_RX_DEQUEUE_EN_MSK,
                                 OPSM4_REG_RX_CTRL_BIT_SFI_RX_DEQUEUE_EN_OFF,
                                 value);
}

static INLINE UINT32 opsm4_field_SFI_RX_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_SFI_RX_DEQUEUE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001432c bits 17) field SFI_RX_DEQUEUE_EN of register PMC_OPSM4_REG_RX_CTRL */
    reg_value = opsm4_reg_RX_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_CTRL_BIT_SFI_RX_DEQUEUE_EN_MSK) >> OPSM4_REG_RX_CTRL_BIT_SFI_RX_DEQUEUE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_SFI_RX_DEQUEUE_EN_get", value );

    return value;
}
static INLINE void opsm4_field_RX_LINE_EN_set( opsm4_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LINE_EN_set( opsm4_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LINE_EN_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_LINE_EN_set", value );

    /* (0x0001432c bits 16) field RX_LINE_EN of register PMC_OPSM4_REG_RX_CTRL */
    opsm4_reg_RX_CTRL_field_set( b_ptr,
                                 h_ptr,
                                 OPSM4_REG_RX_CTRL_BIT_RX_LINE_EN_MSK,
                                 OPSM4_REG_RX_CTRL_BIT_RX_LINE_EN_OFF,
                                 value);
}

static INLINE UINT32 opsm4_field_RX_LINE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LINE_EN_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001432c bits 16) field RX_LINE_EN of register PMC_OPSM4_REG_RX_CTRL */
    reg_value = opsm4_reg_RX_CTRL_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_CTRL_BIT_RX_LINE_EN_MSK) >> OPSM4_REG_RX_CTRL_BIT_RX_LINE_EN_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_LINE_EN_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size20
 * ==================================================================================
 */
static INLINE void opsm4_field_RX_SSF_LANE_MI_ACTIVE_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SSF_LANE_MI_ACTIVE_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_MI_ACTIVE_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SSF_LANE_MI_ACTIVE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_MI_ACTIVE_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 15) field RX_SSF_LANE_MI_ACTIVE of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_SSF_LANE_MI_ACTIVE_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SSF_LANE_MI_ACTIVE_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_MI_ACTIVE_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 15) field RX_SSF_LANE_MI_ACTIVE of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_MI_ACTIVE_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_SSF_LANE_LOS_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SSF_LANE_LOS_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_LOS_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SSF_LANE_LOS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_LOS_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 14) field RX_SSF_LANE_LOS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_SSF_LANE_LOS_get( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SSF_LANE_LOS_get( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_LOS_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 14) field RX_SSF_LANE_LOS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_LOS_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 13) field RX_SSF_LANE_LOF_LOFLANE of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_SSF_LANE_LOF_LOFLANE_get( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SSF_LANE_LOF_LOFLANE_get( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_LOF_LOFLANE_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 13) field RX_SSF_LANE_LOF_LOFLANE of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_LOF_LOFLANE_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_SSF_LANE_OTUK_AIS_set( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SSF_LANE_OTUK_AIS_set( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_OTUK_AIS_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SSF_LANE_OTUK_AIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_OTUK_AIS_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 11) field RX_SSF_LANE_OTUK_AIS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_SSF_LANE_OTUK_AIS_get( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SSF_LANE_OTUK_AIS_get( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_OTUK_AIS_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 11) field RX_SSF_LANE_OTUK_AIS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_OTUK_AIS_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_SSF_LANE_OTL_AIS_set( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SSF_LANE_OTL_AIS_set( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N,
                                                        UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_OTL_AIS_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SSF_LANE_OTL_AIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_OTL_AIS_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 10) field RX_SSF_LANE_OTL_AIS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_SSF_LANE_OTL_AIS_get( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SSF_LANE_OTL_AIS_get( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LANE_OTL_AIS_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 10) field RX_SSF_LANE_OTL_AIS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_SSF_LANE_OTL_AIS_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_SSF_LOL_set( opsm4_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SSF_LOL_set( opsm4_buffer_t *b_ptr,
                                               lineotn_handle_t *h_ptr,
                                               UINT32  N,
                                               UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LOL_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SSF_LOL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_SSF_LOL_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 9) field RX_SSF_LOL of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_SSF_LOL_get( opsm4_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SSF_LOL_get( opsm4_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_LOL_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 9) field RX_SSF_LOL of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_SSF_LOL_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_SSF_OTU4_LOF_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SSF_OTU4_LOF_set( opsm4_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32  N,
                                                    UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_OTU4_LOF_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SSF_OTU4_LOF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_SSF_OTU4_LOF_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 8) field RX_SSF_OTU4_LOF of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU4_LOF_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU4_LOF_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_SSF_OTU4_LOF_get( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SSF_OTU4_LOF_get( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_SSF_OTU4_LOF_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 8) field RX_SSF_OTU4_LOF of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU4_LOF_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU4_LOF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_SSF_OTU4_LOF_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_FEC_LANE_MI_ACTIVE_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FEC_LANE_MI_ACTIVE_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FEC_LANE_MI_ACTIVE_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FEC_LANE_MI_ACTIVE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_FEC_LANE_MI_ACTIVE_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 7) field RX_FEC_LANE_MI_ACTIVE of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_FEC_LANE_MI_ACTIVE_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FEC_LANE_MI_ACTIVE_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FEC_LANE_MI_ACTIVE_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 7) field RX_FEC_LANE_MI_ACTIVE of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_FEC_LANE_MI_ACTIVE_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_FAIL_LANE_LOS_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FAIL_LANE_LOS_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LANE_LOS_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FAIL_LANE_LOS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_FAIL_LANE_LOS_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 6) field RX_FAIL_LANE_LOS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_FAIL_LANE_LOS_get( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FAIL_LANE_LOS_get( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LANE_LOS_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 6) field RX_FAIL_LANE_LOS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_FAIL_LANE_LOS_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 5) field RX_FAIL_LANE_LOF_LOFLANE of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 5) field RX_FAIL_LANE_LOF_LOFLANE of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_FAIL_LANE_OTUK_AIS_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FAIL_LANE_OTUK_AIS_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LANE_OTUK_AIS_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FAIL_LANE_OTUK_AIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_FAIL_LANE_OTUK_AIS_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 3) field RX_FAIL_LANE_OTUK_AIS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_FAIL_LANE_OTUK_AIS_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FAIL_LANE_OTUK_AIS_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LANE_OTUK_AIS_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 3) field RX_FAIL_LANE_OTUK_AIS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_FAIL_LANE_OTUK_AIS_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_FAIL_LANE_OTL_AIS_set( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FAIL_LANE_OTL_AIS_set( opsm4_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LANE_OTL_AIS_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FAIL_LANE_OTL_AIS_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_FAIL_LANE_OTL_AIS_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 2) field RX_FAIL_LANE_OTL_AIS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_FAIL_LANE_OTL_AIS_get( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FAIL_LANE_OTL_AIS_get( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LANE_OTL_AIS_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 2) field RX_FAIL_LANE_OTL_AIS of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_FAIL_LANE_OTL_AIS_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_FAIL_LOL_set( opsm4_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FAIL_LOL_set( opsm4_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32  N,
                                                UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LOL_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FAIL_LOL_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_FAIL_LOL_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 1) field RX_FAIL_LOL of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_FAIL_LOL_get( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FAIL_LOL_get( opsm4_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_LOL_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 1) field RX_FAIL_LOL of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_FAIL_LOL_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_FAIL_OTU4_LOF_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_FAIL_OTU4_LOF_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_OTU4_LOF_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_FAIL_OTU4_LOF_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_FAIL_OTU4_LOF_set", N, value );

    /* ((0x000140ec + (N) * 0x4) bits 0) field RX_FAIL_OTU4_LOF of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                            h_ptr,
                                                            N,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU4_LOF_MSK,
                                                            OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU4_LOF_OFF,
                                                            value);
}

static INLINE UINT32 opsm4_field_RX_FAIL_OTU4_LOF_get( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_FAIL_OTU4_LOF_get( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_FAIL_OTU4_LOF_get", N, 19);
    /* ((0x000140ec + (N) * 0x4) bits 0) field RX_FAIL_OTU4_LOF of register PMC_OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                   h_ptr,
                                                                   N);
    value = (reg_value & OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU4_LOF_MSK) >> OPSM4_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU4_LOF_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_FAIL_OTU4_LOF_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_set", value );

    /* (0x00014010 bits 26) field TX_SFI_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 26) field TX_SFI_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set", value );

    /* (0x00014010 bits 25) field TX_SFI_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 25) field TX_SFI_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_set", value );

    /* (0x00014010 bits 24) field TX_SYS_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 24) field TX_SYS_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set", value );

    /* (0x00014010 bits 23) field TX_SYS_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 23) field TX_SYS_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_TX_LANE_FAS_IAE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LANE_FAS_IAE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_E_set", value );

    /* (0x00014010 bits 22) field TX_LANE_FAS_IAE_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FAS_IAE_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FAS_IAE_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_LANE_FAS_IAE_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FAS_IAE_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 22) field TX_LANE_FAS_IAE_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FAS_IAE_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FAS_IAE_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_TX_LANE_MFAS_IAE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LANE_MFAS_IAE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_E_set", value );

    /* (0x00014010 bits 21) field TX_LANE_MFAS_IAE_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_MFAS_IAE_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_MFAS_IAE_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_LANE_MFAS_IAE_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_MFAS_IAE_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 21) field TX_LANE_MFAS_IAE_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_MFAS_IAE_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_MFAS_IAE_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_set", value );

    /* (0x00014010 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014010 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
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
        /* (0x00014010 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
        opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF + subfield_offset),
                                           OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get", stop_bit, 9 );
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
    /* (0x00014010 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK)
                  >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK, OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set", value, 1023);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set", value );

    /* (0x00014010 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014010 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
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
        /* (0x00014010 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
        opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                           h_ptr,
                                           subfield_mask << (OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF + subfield_offset),
                                           OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF + subfield_offset,
                                           value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get", stop_bit, 9 );
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
    /* (0x00014010 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK)
                  >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK, OPSM4_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get", start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm4_field_TX_OTUK_AIS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_OTUK_AIS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_OTUK_AIS_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_OTUK_AIS_INT_E_set", value );

    /* (0x00014010 bits 0) field TX_OTUK_AIS_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    opsm4_reg_TX_INT_EN_REG_field_set( b_ptr,
                                       h_ptr,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_OTUK_AIS_INT_E_MSK,
                                       OPSM4_REG_TX_INT_EN_REG_BIT_TX_OTUK_AIS_INT_E_OFF,
                                       value);
}

static INLINE UINT32 opsm4_field_TX_OTUK_AIS_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_OTUK_AIS_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014010 bits 0) field TX_OTUK_AIS_INT_E of register PMC_OPSM4_REG_TX_INT_EN_REG */
    reg_value = opsm4_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_EN_REG_BIT_TX_OTUK_AIS_INT_E_MSK) >> OPSM4_REG_TX_INT_EN_REG_BIT_TX_OTUK_AIS_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_OTUK_AIS_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SFI_LOS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_LOS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_LOS_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_LOS_INT_E_set", value );

    /* (0x00014080 bits 15) field RX_SFI_LOS_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SFI_LOS_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_LOS_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 15) field RX_SFI_LOS_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_LOS_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SFI_OOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_OOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_OOF_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_OOF_INT_E_set", value );

    /* (0x00014080 bits 14) field RX_SFI_OOF_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SFI_OOF_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_OOF_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 14) field RX_SFI_OOF_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_OOF_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SFI_LOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_LOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_LOF_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_LOF_INT_E_set", value );

    /* (0x00014080 bits 13) field RX_SFI_LOF_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SFI_LOF_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_LOF_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 13) field RX_SFI_LOF_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_LOF_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_OTU_OOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_OTU_OOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_OTU_OOF_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_OTU_OOF_INT_E_set", value );

    /* (0x00014080 bits 12) field RX_OTU_OOF_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_OTU_OOF_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_OTU_OOF_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 12) field RX_OTU_OOF_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_OTU_OOF_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_OTU_LOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_OTU_LOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_OTU_LOF_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_OTU_LOF_INT_E_set", value );

    /* (0x00014080 bits 11) field RX_OTU_LOF_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_OTU_LOF_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_OTU_LOF_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 11) field RX_OTU_LOF_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_OTU_LOF_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SFI_AIS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_AIS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_AIS_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_AIS_INT_E_set", value );

    /* (0x00014080 bits 9) field RX_SFI_AIS_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SFI_AIS_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_AIS_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 9) field RX_SFI_AIS_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_AIS_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_LOL_INT_E_set( opsm4_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LOL_INT_E_set( opsm4_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LOL_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_LOL_INT_E_set", value );

    /* (0x00014080 bits 8) field RX_LOL_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_LOL_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_LOL_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_LOL_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LOL_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 8) field RX_LOL_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_LOL_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_LOL_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_LOL_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_set", value );

    /* (0x00014080 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_set", value );

    /* (0x00014080 bits 6) field RX_SFI_ALIGN_CHANGE_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 6) field RX_SFI_ALIGN_CHANGE_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_DESKEW_CHANGE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_DESKEW_CHANGE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_E_set", value );

    /* (0x00014080 bits 5) field RX_DESKEW_CHANGE_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_DESKEW_CHANGE_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_DESKEW_CHANGE_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 5) field RX_DESKEW_CHANGE_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_set", value );

    /* (0x00014080 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_get", value );

    return value;
}
static INLINE void opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set", value );

    /* (0x00014080 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    opsm4_reg_RX_OTU4_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_MSK,
                                            OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_OFF,
                                            value);
}

static INLINE UINT32 opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014080 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_E of register PMC_OPSM4_REG_RX_OTU4_INT_EN_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_EN_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_MSK) >> OPSM4_REG_RX_OTU4_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get", value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_N_size20
 * ==================================================================================
 */
static INLINE void opsm4_field_RX_LANE_LOS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_LOS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOS_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_LOS_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_LOS_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_LOS_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOS_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOS_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOS_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_OOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OOF_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOF_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OOF_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OOF_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOF_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOF_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOF_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOF_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_get( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_get( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_OOM_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OOM_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOM_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OOM_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OOM_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 12) field RX_LANE_OOM_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOM_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOM_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOM_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOM_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOM_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 12) field RX_LANE_OOM_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOM_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOM_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOM_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_OTUK_AIS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OTUK_AIS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTUK_AIS_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OTUK_AIS_INT_E_get( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OTUK_AIS_INT_E_get( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTUK_AIS_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_OTL_AIS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OTL_AIS_INT_E_set( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTL_AIS_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OTL_AIS_INT_E_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OTL_AIS_INT_E_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTL_AIS_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_set( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_get( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_get( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_FIFO_ERROR_INT_E_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_FIFO_ERROR_INT_E_set( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_FIFO_ERROR_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_FIFO_ERROR_INT_E_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_FIFO_ERROR_INT_E_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_FIFO_ERROR_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_OOR_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OOR_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOR_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OOR_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OOR_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOR_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOR_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOR_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOR_INT_E_get", N, value );

    return value;
}
static INLINE void opsm4_field_RX_LANE_LOR_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_LOR_INT_E_set( opsm4_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOR_INT_E_set", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_LOR_INT_E_set", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_LOR_INT_E_set", N, value );

    /* ((0x000141dc + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                  h_ptr,
                                                  N,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_MSK,
                                                  OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_OFF,
                                                  value);
}

static INLINE UINT32 opsm4_field_RX_LANE_LOR_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOR_INT_E_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOR_INT_E_get", N, 19);
    /* ((0x000141dc + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_E of register PMC_OPSM4_REG_RX_LANE_INT_EN_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                         h_ptr,
                                                         N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_MSK) >> OPSM4_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOR_INT_E_get", N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE void opsm4_lfield_INTERRUPT_SUMMARY_get( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value[2] ) ALWAYS_INLINE;
static INLINE void opsm4_lfield_INTERRUPT_SUMMARY_get( opsm4_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 value[2] )
{
    UINT32 i;
    UINT32 field_value;
    UINT32 reg_value;

    for (i=0;i<2;i++)
        value[i] = 0;

    /* (0x00014364 bits 31:0) bits 0:31 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY */
    reg_value = opsm4_reg_OPSM4_INT_SUMMARY_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK) >> OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF;
    value[0] |= field_value;

    /* (0x00014368 bits 12:0) bits 32:44 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY1 */
    reg_value = opsm4_reg_OPSM4_INT_SUMMARY1_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_MSK) >> OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_OFF;
    value[1] |= field_value;

    IOLOG( "%s -> 0x%08x 0x%08x", "opsm4_lfield_INTERRUPT_SUMMARY_get", value[1] , value[0] );

}
static INLINE UINT32 opsm4_lfield_range_INTERRUPT_SUMMARY_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_lfield_range_INTERRUPT_SUMMARY_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit )
{
    UINT32 value = 0;
    UINT32 field_value;
    UINT32 reg_value;

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_lfield_range_INTERRUPT_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 44) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_lfield_range_INTERRUPT_SUMMARY_get", stop_bit, 44 );
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
    /* (0x00014364 bits 31:0) bits 0:31 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY */
        reg_value = opsm4_reg_OPSM4_INT_SUMMARY_read(  b_ptr, h_ptr);
        field_value = (reg_value & OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK)
                      >> OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK, OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    if ((start_bit <= 44) && (stop_bit >= 32)) {
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
        if (stop_bit < 44) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 44;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
    /* (0x00014368 bits 12:0) bits 32:44 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY1 */
        reg_value = opsm4_reg_OPSM4_INT_SUMMARY1_read(  b_ptr, h_ptr);
        field_value = (reg_value & OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_MSK)
                      >> OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_OFF;
        IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_MSK, OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_OFF, field_value );
        value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
        IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    }

    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_lfield_range_INTERRUPT_SUMMARY_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_lfield_range_INTERRUPT_SUMMARY_poll( opsm4_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_lfield_range_INTERRUPT_SUMMARY_poll( opsm4_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_lfield_range_INTERRUPT_SUMMARY_poll", stop_bit, start_bit );
    if (stop_bit > 44) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_lfield_range_INTERRUPT_SUMMARY_poll", stop_bit, 44 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_lfield_range_INTERRUPT_SUMMARY_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014364 bits 31:0) bits 0:31 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY */
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
        /* (0x00014364 bits 31:0) bits 0:31 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY */
        return opsm4_reg_OPSM4_INT_SUMMARY_poll( b_ptr,
                                                 h_ptr,
                                                 subfield_mask << (OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF + subfield_offset),
                                                 value << (OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF + subfield_offset),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
    }

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014368 bits 12:0) bits 32:44 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY1 */
    if ((start_bit <= 44) && (stop_bit >= 32)) {
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
        if (stop_bit < 44) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 44;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00014368 bits 12:0) bits 32:44 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY1 */
        return opsm4_reg_OPSM4_INT_SUMMARY1_poll( b_ptr,
                                                  h_ptr,
                                                  subfield_mask << (OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_OFF + subfield_offset),
                                                  value << (OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_OFF + subfield_offset),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm4_lfield_INTERRUPT_SUMMARY_poll( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value[2],
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_lfield_INTERRUPT_SUMMARY_poll( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value[2],
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x 0x%08x", "opsm4_lfield_INTERRUPT_SUMMARY_poll", value[1] , value[0] );

    /* (0x00014364 bits 31:0) bits 0:31 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY */
    return opsm4_reg_OPSM4_INT_SUMMARY_poll( b_ptr,
                                             h_ptr,
                                             OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK,
                                             (value[0]<<OPSM4_REG_OPSM4_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF),
                                             cmp,
                                             max_count,
                                             num_failed_polls,
                                             delay_between_polls_in_microseconds);

    /* (0x00014368 bits 12:0) bits 32:44 use field INTERRUPT_SUMMARY of register PMC_OPSM4_REG_OPSM4_INT_SUMMARY1 */
    return opsm4_reg_OPSM4_INT_SUMMARY1_poll( b_ptr,
                                              h_ptr,
                                              OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_MSK,
                                              (value[1]<<OPSM4_REG_OPSM4_INT_SUMMARY1_BIT_INTERRUPT_SUMMARY_OFF),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync
 * ==================================================================================
 */
static INLINE void opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", value );

    /* (0x0001400c bits 26) field TX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 26) field TX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_poll", value );

    /* (0x0001400c bits 26) field TX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", value );

    /* (0x0001400c bits 25) field TX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 25) field TX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_poll", value );

    /* (0x0001400c bits 25) field TX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", value );

    /* (0x0001400c bits 24) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 24) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_poll", value );

    /* (0x0001400c bits 24) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", value );

    /* (0x0001400c bits 23) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 23) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_poll", value );

    /* (0x0001400c bits 23) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_TX_LANE_FAS_IAE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LANE_FAS_IAE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_I_set_to_clear", value );

    /* (0x0001400c bits 22) field TX_LANE_FAS_IAE_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FAS_IAE_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FAS_IAE_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_LANE_FAS_IAE_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FAS_IAE_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 22) field TX_LANE_FAS_IAE_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FAS_IAE_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FAS_IAE_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FAS_IAE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FAS_IAE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_I_poll", value );

    /* (0x0001400c bits 22) field TX_LANE_FAS_IAE_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FAS_IAE_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FAS_IAE_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_TX_LANE_MFAS_IAE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LANE_MFAS_IAE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_I_set_to_clear", value );

    /* (0x0001400c bits 21) field TX_LANE_MFAS_IAE_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_LANE_MFAS_IAE_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_LANE_MFAS_IAE_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_LANE_MFAS_IAE_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_MFAS_IAE_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 21) field TX_LANE_MFAS_IAE_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_LANE_MFAS_IAE_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_LANE_MFAS_IAE_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_MFAS_IAE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_MFAS_IAE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_I_poll", value );

    /* (0x0001400c bits 21) field TX_LANE_MFAS_IAE_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_LANE_MFAS_IAE_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_LANE_MFAS_IAE_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", value, 1023);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", value );

    /* (0x0001400c bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001400c bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
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
        /* (0x0001400c bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
        opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                                        OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get", stop_bit, 9 );
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
    /* (0x0001400c bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK)
                  >> OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK, OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                       lineotn_handle_t *h_ptr,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                       lineotn_handle_t *h_ptr,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001400c bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0001400c bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
        return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          subfield_mask << (OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                          value << (OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_poll", value );

    /* (0x0001400c bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 value )
{
    if (value > 1023)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", value, 1023);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", value );

    /* (0x0001400c bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get", value );

    return value;
}
static INLINE void opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001400c bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
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
        /* (0x0001400c bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
        opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                        h_ptr,
                                                        subfield_mask << (OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                                        OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset,
                                                        value >> subfield_shift);
    }
}

static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get", stop_bit, 9 );
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
    /* (0x0001400c bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK)
                  >> OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK, OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001400c bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x0001400c bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
        return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                          h_ptr,
                                          subfield_mask << (OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                          value << (OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", value );

    /* (0x0001400c bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_TX_OTUK_AIS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_TX_OTUK_AIS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_TX_OTUK_AIS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_TX_OTUK_AIS_INT_I_set_to_clear", value );

    /* (0x0001400c bits 0) field TX_OTUK_AIS_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    opsm4_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                    h_ptr,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_OTUK_AIS_INT_I_MSK,
                                                    OPSM4_REG_TX_INT_REG_BIT_TX_OTUK_AIS_INT_I_OFF,
                                                    value);
}

static INLINE UINT32 opsm4_field_TX_OTUK_AIS_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_OTUK_AIS_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001400c bits 0) field TX_OTUK_AIS_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    reg_value = opsm4_reg_TX_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_REG_BIT_TX_OTUK_AIS_INT_I_MSK) >> OPSM4_REG_TX_INT_REG_BIT_TX_OTUK_AIS_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_OTUK_AIS_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_OTUK_AIS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_OTUK_AIS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_OTUK_AIS_INT_I_poll", value );

    /* (0x0001400c bits 0) field TX_OTUK_AIS_INT_I of register PMC_OPSM4_REG_TX_INT_REG */
    return opsm4_reg_TX_INT_REG_poll( b_ptr,
                                      h_ptr,
                                      OPSM4_REG_TX_INT_REG_BIT_TX_OTUK_AIS_INT_I_MSK,
                                      (value<<OPSM4_REG_TX_INT_REG_BIT_TX_OTUK_AIS_INT_I_OFF),
                                      cmp,
                                      max_count,
                                      num_failed_polls,
                                      delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_SFI_LOS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_LOS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_LOS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_LOS_INT_I_set_to_clear", value );

    /* (0x0001407c bits 15) field RX_SFI_LOS_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOS_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOS_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_SFI_LOS_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_LOS_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 15) field RX_SFI_LOS_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOS_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOS_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_LOS_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_LOS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_LOS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_LOS_INT_I_poll", value );

    /* (0x0001407c bits 15) field RX_SFI_LOS_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOS_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOS_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_SFI_OOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_OOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_OOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_OOF_INT_I_set_to_clear", value );

    /* (0x0001407c bits 14) field RX_SFI_OOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_OOF_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_OOF_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_SFI_OOF_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_OOF_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 14) field RX_SFI_OOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_OOF_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_OOF_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_OOF_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_OOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_OOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_OOF_INT_I_poll", value );

    /* (0x0001407c bits 14) field RX_SFI_OOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_OOF_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_OOF_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_SFI_LOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_LOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_LOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_LOF_INT_I_set_to_clear", value );

    /* (0x0001407c bits 13) field RX_SFI_LOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOF_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOF_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_SFI_LOF_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_LOF_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 13) field RX_SFI_LOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOF_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOF_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_LOF_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_LOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_LOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_LOF_INT_I_poll", value );

    /* (0x0001407c bits 13) field RX_SFI_LOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOF_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_LOF_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_OTU_OOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_OTU_OOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_OTU_OOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_OTU_OOF_INT_I_set_to_clear", value );

    /* (0x0001407c bits 12) field RX_OTU_OOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_OOF_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_OOF_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_OTU_OOF_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_OTU_OOF_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 12) field RX_OTU_OOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_OOF_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_OOF_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_OTU_OOF_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_OTU_OOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_OTU_OOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_OTU_OOF_INT_I_poll", value );

    /* (0x0001407c bits 12) field RX_OTU_OOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_OOF_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_OOF_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_OTU_LOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_OTU_LOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_OTU_LOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_OTU_LOF_INT_I_set_to_clear", value );

    /* (0x0001407c bits 11) field RX_OTU_LOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_LOF_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_LOF_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_OTU_LOF_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_OTU_LOF_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 11) field RX_OTU_LOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_LOF_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_LOF_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_OTU_LOF_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_OTU_LOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_OTU_LOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_OTU_LOF_INT_I_poll", value );

    /* (0x0001407c bits 11) field RX_OTU_LOF_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_LOF_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_OTU_LOF_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_SFI_AIS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_AIS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_AIS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_AIS_INT_I_set_to_clear", value );

    /* (0x0001407c bits 9) field RX_SFI_AIS_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_AIS_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_AIS_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_SFI_AIS_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_AIS_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 9) field RX_SFI_AIS_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_AIS_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_AIS_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_AIS_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_AIS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_AIS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_AIS_INT_I_poll", value );

    /* (0x0001407c bits 9) field RX_SFI_AIS_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_AIS_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_AIS_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LOL_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LOL_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LOL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_LOL_INT_I_set_to_clear", value );

    /* (0x0001407c bits 8) field RX_LOL_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_LOL_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_LOL_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_LOL_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LOL_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 8) field RX_LOL_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_LOL_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_LOL_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_LOL_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LOL_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LOL_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_LOL_INT_I_poll", value );

    /* (0x0001407c bits 8) field RX_LOL_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_LOL_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_LOL_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear", value );

    /* (0x0001407c bits 7) field RX_SKEW_RANGE_VIOLATION_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 7) field RX_SKEW_RANGE_VIOLATION_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_poll", value );

    /* (0x0001407c bits 7) field RX_SKEW_RANGE_VIOLATION_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_set_to_clear", value );

    /* (0x0001407c bits 6) field RX_SFI_ALIGN_CHANGE_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 6) field RX_SFI_ALIGN_CHANGE_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_poll", value );

    /* (0x0001407c bits 6) field RX_SFI_ALIGN_CHANGE_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_DESKEW_CHANGE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_DESKEW_CHANGE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_I_set_to_clear", value );

    /* (0x0001407c bits 5) field RX_DESKEW_CHANGE_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_DESKEW_CHANGE_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_DESKEW_CHANGE_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 5) field RX_DESKEW_CHANGE_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_DESKEW_CHANGE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_DESKEW_CHANGE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_I_poll", value );

    /* (0x0001407c bits 5) field RX_DESKEW_CHANGE_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", value );

    /* (0x0001407c bits 4) field RX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 4) field RX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_poll", value );

    /* (0x0001407c bits 4) field RX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 value )
{
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= 0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", value );

    /* (0x0001407c bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_MSK,
                                                         OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_OFF,
                                                         value);
}

static INLINE UINT32 opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x0001407c bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_MSK) >> OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_poll", value );

    /* (0x0001407c bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM4_REG_RX_OTU4_INT_REG */
    return opsm4_reg_RX_OTU4_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_MSK,
                                           (value<<OPSM4_REG_RX_OTU4_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_size20
 * ==================================================================================
 */
static INLINE void opsm4_field_RX_LANE_LOS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_LOS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOS_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_LOS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_LOS_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_LOS_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOS_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOS_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOS_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_LOS_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_OOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OOF_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOF_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OOF_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOF_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOF_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOF_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOF_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOF_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OOF_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_get( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_get( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32  N,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32  N,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_OOM_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OOM_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOM_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OOM_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OOM_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 12) field RX_LANE_OOM_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOM_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOM_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOM_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOM_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOM_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 12) field RX_LANE_OOM_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOM_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOM_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOM_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOM_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOM_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OOM_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 12) field RX_LANE_OOM_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOM_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOM_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OTUK_AIS_INT_I_get( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OTUK_AIS_INT_I_get( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTUK_AIS_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OTUK_AIS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OTUK_AIS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_OTL_AIS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OTL_AIS_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32  N,
                                                                   UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTL_AIS_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OTL_AIS_INT_I_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OTL_AIS_INT_I_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTL_AIS_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OTL_AIS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OTL_AIS_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32  N,
                                                                        UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_get( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_get( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32  N,
                                                                      UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_FIFO_ERROR_INT_I_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_FIFO_ERROR_INT_I_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_FIFO_ERROR_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_FIFO_ERROR_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_FIFO_ERROR_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_OOR_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_OOR_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOR_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_OOR_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_OOR_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOR_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOR_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOR_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOR_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOR_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOR_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OOR_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_OFF),
                                                 cmp,
                                                 max_count,
                                                 num_failed_polls,
                                                 delay_between_polls_in_microseconds);
}

static INLINE void opsm4_field_RX_LANE_LOR_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm4_field_RX_LANE_LOR_INT_I_set_to_clear( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOR_INT_I_set_to_clear", N, 19);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm4_field_RX_LANE_LOR_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= N=%d 0x%08x", "opsm4_field_RX_LANE_LOR_INT_I_set_to_clear", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                               h_ptr,
                                                               N,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_MSK,
                                                               OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_OFF,
                                                               value);
}

static INLINE UINT32 opsm4_field_RX_LANE_LOR_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOR_INT_I_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOR_INT_I_get", N, 19);
    /* ((0x0001418c + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                      h_ptr,
                                                      N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_MSK) >> OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOR_INT_I_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOR_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOR_INT_I_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_LOR_INT_I_poll", N, value );

    /* ((0x0001418c + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_I of register PMC_OPSM4_REG_RX_LANE_INT_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                 h_ptr,
                                                 N,
                                                 OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_MSK,
                                                 (value<<OPSM4_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_OFF),
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
static INLINE UINT32 opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 26) field TX_SFI_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_V_poll", value );

    /* (0x00014014 bits 26) field TX_SFI_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 25) field TX_SFI_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_V_poll", value );

    /* (0x00014014 bits 25) field TX_SFI_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 24) field TX_SYS_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_V_poll", value );

    /* (0x00014014 bits 24) field TX_SYS_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 23) field TX_SYS_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_V_poll", value );

    /* (0x00014014 bits 23) field TX_SYS_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_TX_LANE_FAS_IAE_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FAS_IAE_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 22) field TX_LANE_FAS_IAE_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FAS_IAE_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FAS_IAE_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FAS_IAE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FAS_IAE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_LANE_FAS_IAE_INT_V_poll", value );

    /* (0x00014014 bits 22) field TX_LANE_FAS_IAE_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FAS_IAE_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FAS_IAE_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_TX_LANE_MFAS_IAE_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_MFAS_IAE_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 21) field TX_LANE_MFAS_IAE_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_MFAS_IAE_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_MFAS_IAE_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_MFAS_IAE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_MFAS_IAE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_LANE_MFAS_IAE_INT_V_poll", value );

    /* (0x00014014 bits 21) field TX_LANE_MFAS_IAE_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_MFAS_IAE_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_MFAS_IAE_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_V_get", value );

    return value;
}
static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get", stop_bit, 9 );
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
    /* (0x00014014 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK)
                  >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK, OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                       lineotn_handle_t *h_ptr,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                       lineotn_handle_t *h_ptr,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014014 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00014014 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
        return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              subfield_mask << (OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF + subfield_offset),
                                              value << (OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF + subfield_offset),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_V_poll", value );

    /* (0x00014014 bits 20:11) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get", value );

    return value;
}
static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
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

    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get", stop_bit, 9 );
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
    /* (0x00014014 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    field_value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK)
                  >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK, OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get", start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 9) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", stop_bit, 9 );
    IOLOG( "%s <= start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00014014 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
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
        if (stop_bit < 9) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 9;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00014014 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
        return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                              h_ptr,
                                              subfield_mask << (OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF + subfield_offset),
                                              value << (OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF + subfield_offset),
                                              cmp,
                                              max_count,
                                              num_failed_polls,
                                              delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", value );

    /* (0x00014014 bits 10:1) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_TX_OTUK_AIS_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_TX_OTUK_AIS_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014014 bits 0) field TX_OTUK_AIS_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    reg_value = opsm4_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_TX_INT_VAL_REG_BIT_TX_OTUK_AIS_INT_V_MSK) >> OPSM4_REG_TX_INT_VAL_REG_BIT_TX_OTUK_AIS_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_TX_OTUK_AIS_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_OTUK_AIS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_TX_OTUK_AIS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_TX_OTUK_AIS_INT_V_poll", value );

    /* (0x00014014 bits 0) field TX_OTUK_AIS_INT_V of register PMC_OPSM4_REG_TX_INT_VAL_REG */
    return opsm4_reg_TX_INT_VAL_REG_poll( b_ptr,
                                          h_ptr,
                                          OPSM4_REG_TX_INT_VAL_REG_BIT_TX_OTUK_AIS_INT_V_MSK,
                                          (value<<OPSM4_REG_TX_INT_VAL_REG_BIT_TX_OTUK_AIS_INT_V_OFF),
                                          cmp,
                                          max_count,
                                          num_failed_polls,
                                          delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_SFI_LOS_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_LOS_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 15) field RX_SFI_LOS_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_LOS_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_LOS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_LOS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_LOS_INT_V_poll", value );

    /* (0x00014084 bits 15) field RX_SFI_LOS_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_SFI_OOF_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_OOF_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 14) field RX_SFI_OOF_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_OOF_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_OOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_OOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_OOF_INT_V_poll", value );

    /* (0x00014084 bits 14) field RX_SFI_OOF_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_SFI_LOF_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_LOF_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 13) field RX_SFI_LOF_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_LOF_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_LOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_LOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_LOF_INT_V_poll", value );

    /* (0x00014084 bits 13) field RX_SFI_LOF_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_OTU_OOF_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_OTU_OOF_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 12) field RX_OTU_OOF_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_OTU_OOF_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_OTU_OOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_OTU_OOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_OTU_OOF_INT_V_poll", value );

    /* (0x00014084 bits 12) field RX_OTU_OOF_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_OTU_LOF_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_OTU_LOF_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 11) field RX_OTU_LOF_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_OTU_LOF_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_OTU_LOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_OTU_LOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_OTU_LOF_INT_V_poll", value );

    /* (0x00014084 bits 11) field RX_OTU_LOF_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_SFI_AIS_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_AIS_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 9) field RX_SFI_AIS_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_AIS_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_AIS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_AIS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 value,
                                                                      PMC_POLL_COMPARISON_TYPE cmp,
                                                                      UINT32 max_count,
                                                                      UINT32 *num_failed_polls,
                                                                      UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_AIS_INT_V_poll", value );

    /* (0x00014084 bits 9) field RX_SFI_AIS_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LOL_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LOL_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 8) field RX_LOL_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_LOL_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_LOL_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_LOL_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LOL_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LOL_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 value,
                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                  UINT32 max_count,
                                                                  UINT32 *num_failed_polls,
                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_LOL_INT_V_poll", value );

    /* (0x00014084 bits 8) field RX_LOL_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_LOL_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_LOL_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_V_poll", value );

    /* (0x00014084 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_SFI_ALIGN_CHANGE_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_ALIGN_CHANGE_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 6) field RX_SFI_ALIGN_CHANGE_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_ALIGN_CHANGE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_ALIGN_CHANGE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_ALIGN_CHANGE_INT_V_poll", value );

    /* (0x00014084 bits 6) field RX_SFI_ALIGN_CHANGE_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_ALIGN_CHANGE_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_DESKEW_CHANGE_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_DESKEW_CHANGE_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 5) field RX_DESKEW_CHANGE_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_DESKEW_CHANGE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_DESKEW_CHANGE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_DESKEW_CHANGE_INT_V_poll", value );

    /* (0x00014084 bits 5) field RX_DESKEW_CHANGE_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_V_poll", value );

    /* (0x00014084 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get( opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
{
    UINT32 value = 0;
    UINT32 reg_value;

    /* (0x00014084 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    reg_value = opsm4_reg_RX_OTU4_INT_VAL_REG_read(  b_ptr, h_ptr);
    value = (reg_value & OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_MSK) >> OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG( "%s -> 0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get", value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  0x%08x", "opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_V_poll", value );

    /* (0x00014084 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_V of register PMC_OPSM4_REG_RX_OTU4_INT_VAL_REG */
    return opsm4_reg_RX_OTU4_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_MSK,
                                               (value<<OPSM4_REG_RX_OTU4_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_OFF),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value_N_size20
 * ==================================================================================
 */
static INLINE UINT32 opsm4_field_RX_LANE_LOS_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOS_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOS_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOS_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_LOS_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOF_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOF_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOF_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOF_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOF_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OOF_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_get( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_get( opsm4_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32  N,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32  N,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOM_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOM_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOM_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 12) field RX_LANE_OOM_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOM_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOM_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOM_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOM_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOM_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OOM_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 12) field RX_LANE_OOM_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOM_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOM_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_OTUK_AIS_INT_V_get( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OTUK_AIS_INT_V_get( opsm4_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTUK_AIS_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OTUK_AIS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OTUK_AIS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OTUK_AIS_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_OTL_AIS_INT_V_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OTL_AIS_INT_V_get( opsm4_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OTL_AIS_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OTL_AIS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OTL_AIS_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32  N,
                                                                           UINT32 value,
                                                                           PMC_POLL_COMPARISON_TYPE cmp,
                                                                           UINT32 max_count,
                                                                           UINT32 *num_failed_polls,
                                                                           UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OTL_AIS_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_get( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_get( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_FIFO_ERROR_INT_V_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_FIFO_ERROR_INT_V_get( opsm4_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_FIFO_ERROR_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_FIFO_ERROR_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_FIFO_ERROR_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32  N,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_FIFO_ERROR_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_OOR_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_OOR_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_OOR_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_OOR_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOR_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_OOR_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_OOR_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm4_field_RX_LANE_LOR_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_LOR_INT_V_get( opsm4_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_LOR_INT_V_get", N, 19);
    /* ((0x0001422c + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          N);
    value = (reg_value & OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_MSK) >> OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_LOR_INT_V_get", N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOR_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm4_field_RX_LANE_LOR_INT_V_poll( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  N=%d 0x%08x", "opsm4_field_RX_LANE_LOR_INT_V_poll", N, value );

    /* ((0x0001422c + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_V of register PMC_OPSM4_REG_RX_LANE_INT_VAL_REG index N=0..19 */
    return opsm4_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                     h_ptr,
                                                     N,
                                                     OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_MSK,
                                                     (value<<OPSM4_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_OFF),
                                                     cmp,
                                                     max_count,
                                                     num_failed_polls,
                                                     delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size20
 * ==================================================================================
 */
static INLINE UINT32 opsm4_field_RX_LANE_SKEW_FIFO_LEVEL_get( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LANE_SKEW_FIFO_LEVEL_get( opsm4_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LANE_SKEW_FIFO_LEVEL_get", N, 19);
    /* ((0x0001413c + (N) * 0x4) bits 12:5) field RX_LANE_SKEW_FIFO_LEVEL of register PMC_OPSM4_REG_RX_LANE_ID_OR_SKEW_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_ID_OR_SKEW_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LANE_SKEW_FIFO_LEVEL_MSK) >> OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LANE_SKEW_FIFO_LEVEL_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LANE_SKEW_FIFO_LEVEL_get", N, value );

    return value;
}
static INLINE UINT32 opsm4_field_range_RX_LANE_SKEW_FIFO_LEVEL_get( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32  N,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_RX_LANE_SKEW_FIFO_LEVEL_get( opsm4_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
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

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_range_RX_LANE_SKEW_FIFO_LEVEL_get", N, 19);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_RX_LANE_SKEW_FIFO_LEVEL_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_RX_LANE_SKEW_FIFO_LEVEL_get", stop_bit, 7 );
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
    /* ((0x0001413c + (N) * 0x4) bits 12:5) field RX_LANE_SKEW_FIFO_LEVEL of register PMC_OPSM4_REG_RX_LANE_ID_OR_SKEW_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_ID_OR_SKEW_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LANE_SKEW_FIFO_LEVEL_MSK)
                  >> OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LANE_SKEW_FIFO_LEVEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LANE_SKEW_FIFO_LEVEL_MSK, OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LANE_SKEW_FIFO_LEVEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_RX_LANE_SKEW_FIFO_LEVEL_get", N, start_bit, stop_bit, value );

    return value;
}
static INLINE UINT32 opsm4_field_RX_LOGICAL_LANE_IDENTIFIER_get( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_RX_LOGICAL_LANE_IDENTIFIER_get( opsm4_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_RX_LOGICAL_LANE_IDENTIFIER_get", N, 19);
    /* ((0x0001413c + (N) * 0x4) bits 4:0) field RX_LOGICAL_LANE_IDENTIFIER of register PMC_OPSM4_REG_RX_LANE_ID_OR_SKEW_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_ID_OR_SKEW_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    value = (reg_value & OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_MSK) >> OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_OFF;
    IOLOG( "%s -> N=%d 0x%08x", "opsm4_field_RX_LOGICAL_LANE_IDENTIFIER_get", N, value );

    return value;
}
static INLINE UINT32 opsm4_field_range_RX_LOGICAL_LANE_IDENTIFIER_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32  N,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm4_field_range_RX_LOGICAL_LANE_IDENTIFIER_get( opsm4_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
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

    if (N > 19)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm4_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", N, 19);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm4_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", stop_bit, start_bit );
    if (stop_bit > 4) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm4_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", stop_bit, 4 );
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
    /* ((0x0001413c + (N) * 0x4) bits 4:0) field RX_LOGICAL_LANE_IDENTIFIER of register PMC_OPSM4_REG_RX_LANE_ID_OR_SKEW_REG index N=0..19 */
    reg_value = opsm4_reg_RX_LANE_ID_OR_SKEW_REG_array_read( b_ptr,
                                                             h_ptr,
                                                             N);
    field_value = (reg_value & OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_MSK)
                  >> OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_MSK, OPSM4_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> N=%d start_bit=%d stop_bit=%d 0x%08x", "opsm4_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OPSM4_IO_INLINE_H */
