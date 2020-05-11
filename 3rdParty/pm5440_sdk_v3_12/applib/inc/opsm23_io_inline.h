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
 *     and register accessor functions for the opsm23 block
 *****************************************************************************/
#ifndef _OPSM23_IO_INLINE_H
#define _OPSM23_IO_INLINE_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "opsm23_regs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define OPSM23_IO_VERSION 2

/*
 * ==================================================================================
 *  tsb level structure and access functions for opsm23
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
    UINT32 A;
    pmc_sys_handle_t * sys_handle;
    PMC_VAR_CONST UINT32 base_address;
    /* maybe eventually need to add some fields here per io handle */
} opsm23_buffer_t;
static INLINE void opsm23_buffer_init( opsm23_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A ) ALWAYS_INLINE;
static INLINE void opsm23_buffer_init( opsm23_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A )
{
    b_ptr->h_ptr                    = h_ptr;
    b_ptr->A                        = A;
    b_ptr->sys_handle               = ((pmc_handle_t *)h_ptr)->sys_handle;
    *(UINT32 *)&b_ptr->base_address = (((pmc_handle_t *)h_ptr)->base_address + ((0)*0x1000) );
    /* currently this just checks that the previous function remembered to flush. */
    l1sys_init( b_ptr->coalesce_handle, b_ptr->sys_handle );
    IOLOG( "%s A=%d", "opsm23_buffer_init", A);
    
}

/* flush any changed fields to the register file and invalidate the read cache. */
static INLINE void opsm23_buffer_flush( opsm23_buffer_t *b_ptr ) ALWAYS_INLINE;
static INLINE void opsm23_buffer_flush( opsm23_buffer_t *b_ptr )
{
    IOLOG( "opsm23_buffer_flush" );
    l1sys_flush( b_ptr->coalesce_handle );
}
static INLINE UINT32 opsm23_reg_read( opsm23_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg ) ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_read( opsm23_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg )
{
    UINT32 value;
    if (b_ptr != NULL)
        value = l1sys_reg_read( b_ptr->coalesce_handle,
                                mem_type,
                                (b_ptr->base_address + (b_ptr->A)*0x1000),
                                reg);
    else
        value = l2sys_reg_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                                mem_type,
                                (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg);
    return value;
}
static INLINE void opsm23_reg_write( opsm23_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_write( opsm23_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_reg_write( b_ptr->coalesce_handle,
                         mem_type,
                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                         reg,
                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value);
}

static INLINE void opsm23_field_set( opsm23_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
                                     UINT32 mem_type,
                                     UINT32 reg,
                                     UINT32 mask,
                                     UINT32 unused_mask,
                                     UINT32 ofs,
                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_set( opsm23_buffer_t *b_ptr,
                                     lineotn_handle_t *h_ptr,
                                     UINT32 A,
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
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
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
                             (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                             value<<ofs);
        }
        else
        {
            l2sys_reg_read_modify_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                                         mem_type,
                                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                                         mask,
                                         value<<ofs);
        }
    }
}

static INLINE void opsm23_action_on_write_field_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_action_on_write_field_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 mem_type,
                                                     UINT32 reg,
                                                     UINT32 mask,
                                                     UINT32 ofs,
                                                     UINT32 value )
{
    if (b_ptr != NULL)
        l1sys_action_on_write_field_set( b_ptr->coalesce_handle,
                                         mem_type,
                                         (b_ptr->base_address + (b_ptr->A)*0x1000),
                                         reg,
                                         mask,
                                         ofs,
                                         value);
    else
        l2sys_reg_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                         mem_type,
                         (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                         value<<ofs);
}

static INLINE void opsm23_burst_read( opsm23_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value ) ALWAYS_INLINE;
static INLINE void opsm23_burst_read( opsm23_buffer_t *b_ptr,
                                      lineotn_handle_t *h_ptr,
                                      UINT32 A,
                                      UINT32 mem_type,
                                      UINT32 reg,
                                      UINT32 len,
                                      UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_read( b_ptr->coalesce_handle,
                          mem_type,
                          (b_ptr->base_address + (b_ptr->A)*0x1000),
                          reg,
                          len,
                          value);
    else
        l2sys_burst_read( ((pmc_handle_t *)h_ptr)->sys_handle,
                          mem_type,
                          (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                          len,
                          value);
}

static INLINE void opsm23_burst_write( opsm23_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value ) ALWAYS_INLINE;
static INLINE void opsm23_burst_write( opsm23_buffer_t *b_ptr,
                                       lineotn_handle_t *h_ptr,
                                       UINT32 A,
                                       UINT32 mem_type,
                                       UINT32 reg,
                                       UINT32 len,
                                       UINT32 *value )
{
    if (b_ptr != NULL)
        l1sys_burst_write( b_ptr->coalesce_handle,
                           mem_type,
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
                           reg,
                           len,
                           value);
    else
        l2sys_burst_write( ((pmc_handle_t *)h_ptr)->sys_handle,
                           mem_type,
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           len,
                           value);
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_poll( opsm23_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 mem_type,
                                                UINT32 reg,
                                                UINT32 mask,
                                                UINT32 value,
                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                UINT32 max_count,
                                                UINT32 *num_failed_polls,
                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_poll( opsm23_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
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
                           (b_ptr->base_address + (b_ptr->A)*0x1000),
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
                           (((pmc_handle_t *)h_ptr)->base_address + (A)*0x1000) + reg,
                           mask,
                           value,
                           cmp,
                           max_count,
                           num_failed_polls,
                           delay_between_polls_in_microseconds);
}

/*
 * ==================================================================================
 *  register access functions for opsm23
 * ==================================================================================
 */

static INLINE void opsm23_reg_TX_GLOBAL_CTRL_REG_write( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_GLOBAL_CTRL_REG_write( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_TX_GLOBAL_CTRL_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG,
                      value);
}

static INLINE void opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_TX_GLOBAL_CTRL_REG_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG,
                      mask,
                      PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_TX_GLOBAL_CTRL_REG_read( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_TX_GLOBAL_CTRL_REG_read( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_TX_GLOBAL_CTRL_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_TX_L2P_LANE_MAPPING_REG_write( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_L2P_LANE_MAPPING_REG_write( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_TX_L2P_LANE_MAPPING_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG,
                      value);
}

static INLINE void opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 mask,
                                                                 UINT32 ofs,
                                                                 UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG,
                      mask,
                      PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_TX_L2P_LANE_MAPPING_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_TX_SYNC_FIFO_XOFF_TH_write( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_SYNC_FIFO_XOFF_TH_write( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_TX_SYNC_FIFO_XOFF_TH_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH,
                      value);
}

static INLINE void opsm23_reg_TX_SYNC_FIFO_XOFF_TH_field_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_SYNC_FIFO_XOFF_TH_field_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 mask,
                                                              UINT32 ofs,
                                                              UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_TX_SYNC_FIFO_XOFF_TH_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH,
                      mask,
                      PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_TX_SYNC_FIFO_XOFF_TH_read( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_TX_SYNC_FIFO_XOFF_TH_read( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_TX_SYNC_FIFO_XOFF_TH_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_write( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_write( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG,
                      value);
}

static INLINE void opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_field_set( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_field_set( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 ofs,
                                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG,
                      mask,
                      PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_read( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_read( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_RX_GLOBAL_CTRL_REG_write( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_GLOBAL_CTRL_REG_write( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_RX_GLOBAL_CTRL_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG,
                      value);
}

static INLINE void opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_RX_GLOBAL_CTRL_REG_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG,
                      mask,
                      PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_RX_GLOBAL_CTRL_REG_read( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_GLOBAL_CTRL_REG_read( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_RX_GLOBAL_CTRL_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_RX_OTU3_CTRL_REG_write( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_OTU3_CTRL_REG_write( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_RX_OTU3_CTRL_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_RX_OTU3_CTRL_REG,
                      value);
}

static INLINE void opsm23_reg_RX_OTU3_CTRL_REG_field_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_OTU3_CTRL_REG_field_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_RX_OTU3_CTRL_REG_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_RX_OTU3_CTRL_REG,
                      mask,
                      PMC_OPSM23_REG_RX_OTU3_CTRL_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_RX_OTU3_CTRL_REG_read( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_OTU3_CTRL_REG_read( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_OPSM23_REG_RX_OTU3_CTRL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_RX_OTU3_CTRL_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_RX_FP_CONFIG_REG_write( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_FP_CONFIG_REG_write( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_RX_FP_CONFIG_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_RX_FP_CONFIG_REG,
                      value);
}

static INLINE void opsm23_reg_RX_FP_CONFIG_REG_field_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_FP_CONFIG_REG_field_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 mask,
                                                          UINT32 ofs,
                                                          UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_RX_FP_CONFIG_REG_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_RX_FP_CONFIG_REG,
                      mask,
                      PMC_OPSM23_REG_RX_FP_CONFIG_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_RX_FP_CONFIG_REG_read( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_FP_CONFIG_REG_read( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_OPSM23_REG_RX_FP_CONFIG_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_RX_FP_CONFIG_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_write( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_write( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG(N),
                      value);
}

static INLINE void opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32  N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32  N,
                                                                            UINT32 mask,
                                                                            UINT32 ofs,
                                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set", A, N, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_CONFIG,
                      PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG(N),
                      mask,
                      PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_CONFIG,
                                 PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE void opsm23_reg_TX_INT_EN_REG_write( opsm23_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_INT_EN_REG_write( opsm23_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_TX_INT_EN_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_TX_INT_EN_REG,
                      value);
}

static INLINE void opsm23_reg_TX_INT_EN_REG_field_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_INT_EN_REG_field_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 mask,
                                                       UINT32 ofs,
                                                       UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_TX_INT_EN_REG_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_TX_INT_EN_REG,
                      mask,
                      PMC_OPSM23_REG_TX_INT_EN_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_TX_INT_EN_REG_read( opsm23_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_TX_INT_EN_REG_read( opsm23_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_TX_INT_EN_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_TX_INT_EN_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_RX_OTU3_INT_EN_REG_write( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_OTU3_INT_EN_REG_write( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_RX_OTU3_INT_EN_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_RX_OTU3_INT_EN_REG,
                      value);
}

static INLINE void opsm23_reg_RX_OTU3_INT_EN_REG_field_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_OTU3_INT_EN_REG_field_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 mask,
                                                            UINT32 ofs,
                                                            UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_RX_OTU3_INT_EN_REG_field_set", A, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_RX_OTU3_INT_EN_REG,
                      mask,
                      PMC_OPSM23_REG_RX_OTU3_INT_EN_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_RX_OTU3_INT_EN_REG_read( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_OTU3_INT_EN_REG_read( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_RX_OTU3_INT_EN_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_RX_OTU3_INT_EN_REG_read", reg_value, A);
    return reg_value;
}

static INLINE void opsm23_reg_RX_LANE_INT_EN_REG_array_write( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_LANE_INT_EN_REG_array_write( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_RX_LANE_INT_EN_REG_array_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_RX_LANE_INT_EN_REG(N),
                      value);
}

static INLINE void opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N,
                                                                  UINT32 mask,
                                                                  UINT32 ofs,
                                                                  UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_RX_LANE_INT_EN_REG_array_field_set", A, N, mask, ofs, value );
    opsm23_field_set( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_RX_LANE_INT_EN_REG(N),
                      mask,
                      PMC_OPSM23_REG_RX_LANE_INT_EN_REG_UNUSED_MASK,
                      ofs,
                      value);

}

static INLINE UINT32 opsm23_reg_RX_LANE_INT_EN_REG_array_read( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_LANE_INT_EN_REG_array_read( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_RX_LANE_INT_EN_REG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "opsm23_reg_RX_LANE_INT_EN_REG_array_read", reg_value, A, N);
    return reg_value;
}

static INLINE UINT32 opsm23_reg_OPSM23_INT_SUMMARY_read( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_OPSM23_INT_SUMMARY_read( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_OPSM23_INT_SUMMARY);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_OPSM23_INT_SUMMARY_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_OPSM23_INT_SUMMARY_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_OPSM23_INT_SUMMARY_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 mask,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm23_reg_OPSM23_INT_SUMMARY_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm23_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_OPSM23_REG_OPSM23_INT_SUMMARY,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void opsm23_reg_TX_INT_REG_write( opsm23_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_INT_REG_write( opsm23_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_TX_INT_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_TX_INT_REG,
                      value);
}

static INLINE void opsm23_reg_TX_INT_REG_action_on_write_field_set( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_TX_INT_REG_action_on_write_field_set( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 ofs,
                                                                    UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_TX_INT_REG_action_on_write_field_set", A, mask, ofs, value );
    opsm23_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_OPSM23_REG_TX_INT_REG,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 opsm23_reg_TX_INT_REG_read( opsm23_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_TX_INT_REG_read( opsm23_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_TX_INT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_TX_INT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_TX_INT_REG_poll( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_TX_INT_REG_poll( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 mask,
                                                               UINT32 value,
                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                               UINT32 max_count,
                                                               UINT32 *num_failed_polls,
                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm23_reg_TX_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm23_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_OPSM23_REG_TX_INT_REG,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void opsm23_reg_RX_OTU3_INT_REG_write( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_OTU3_INT_REG_write( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_RX_OTU3_INT_REG_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_RX_OTU3_INT_REG,
                      value);
}

static INLINE void opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 mask,
                                                                         UINT32 ofs,
                                                                         UINT32 value )
{
    IOLOG( "%s <- A=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set", A, mask, ofs, value );
    opsm23_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_OPSM23_REG_RX_OTU3_INT_REG,
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 opsm23_reg_RX_OTU3_INT_REG_read( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_OTU3_INT_REG_read( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_RX_OTU3_INT_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_RX_OTU3_INT_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_RX_OTU3_INT_REG_poll( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_RX_OTU3_INT_REG_poll( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 mask,
                                                                    UINT32 value,
                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                    UINT32 max_count,
                                                                    UINT32 *num_failed_polls,
                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm23_reg_RX_OTU3_INT_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm23_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_OPSM23_REG_RX_OTU3_INT_REG,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE void opsm23_reg_RX_LANE_INT_REG_array_write( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_LANE_INT_REG_array_write( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value )
{
    IOLOG( "%s <- 0x%08x", "opsm23_reg_RX_LANE_INT_REG_array_write", value );
    opsm23_reg_write( b_ptr,
                      h_ptr,
                      A,
                      MEM_TYPE_STATUS,
                      PMC_OPSM23_REG_RX_LANE_INT_REG(N),
                      value);
}

static INLINE void opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( opsm23_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32  N,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( opsm23_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32  N,
                                                                               UINT32 mask,
                                                                               UINT32 ofs,
                                                                               UINT32 value )
{
    IOLOG( "%s <- A=%d N=%d mask=0x%08x ofs=%d value=0x%08x", "opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set", A, N, mask, ofs, value );
    opsm23_action_on_write_field_set( b_ptr,
                                      h_ptr,
                                      A,
                                      MEM_TYPE_STATUS,
                                      PMC_OPSM23_REG_RX_LANE_INT_REG(N),
                                      mask,
                                      ofs,
                                      value);

}

static INLINE UINT32 opsm23_reg_RX_LANE_INT_REG_array_read( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_LANE_INT_REG_array_read( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_RX_LANE_INT_REG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "opsm23_reg_RX_LANE_INT_REG_array_read", reg_value, A, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_RX_LANE_INT_REG_array_poll( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_RX_LANE_INT_REG_array_poll( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32  N,
                                                                          UINT32 mask,
                                                                          UINT32 value,
                                                                          PMC_POLL_COMPARISON_TYPE cmp,
                                                                          UINT32 max_count,
                                                                          UINT32 *num_failed_polls,
                                                                          UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm23_reg_RX_LANE_INT_REG_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm23_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_OPSM23_REG_RX_LANE_INT_REG(N),
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm23_reg_TX_INT_VAL_REG_read( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_TX_INT_VAL_REG_read( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_TX_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_TX_INT_VAL_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_TX_INT_VAL_REG_poll( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_TX_INT_VAL_REG_poll( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 mask,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm23_reg_TX_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm23_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_OPSM23_REG_TX_INT_VAL_REG,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm23_reg_RX_OTU3_INT_VAL_REG_read( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_OTU3_INT_VAL_REG_read( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG);

    IOLOG( "%s -> 0x%08x; A=%d", "opsm23_reg_RX_OTU3_INT_VAL_REG_read", reg_value, A);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_RX_OTU3_INT_VAL_REG_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_RX_OTU3_INT_VAL_REG_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 mask,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm23_reg_RX_OTU3_INT_VAL_REG_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm23_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG,
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm23_reg_RX_LANE_INT_VAL_REG_array_read( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_LANE_INT_VAL_REG_array_read( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_RX_LANE_INT_VAL_REG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "opsm23_reg_RX_LANE_INT_VAL_REG_array_read", reg_value, A, N);
    return reg_value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( opsm23_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( opsm23_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32  N,
                                                                              UINT32 mask,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s mask=0x%08x, value=0x%08x, cmp=%d, max_count=%d, delay_between_polls_in_microseconds=%d", "opsm23_reg_RX_LANE_INT_VAL_REG_array_poll", mask, value, cmp, max_count, delay_between_polls_in_microseconds );
    return opsm23_poll( b_ptr,
                        h_ptr,
                        A,
                        MEM_TYPE_STATUS,
                        PMC_OPSM23_REG_RX_LANE_INT_VAL_REG(N),
                        mask,
                        value,
                        cmp,
                        max_count,
                        num_failed_polls,
                        delay_between_polls_in_microseconds);

}


static INLINE UINT32 opsm23_reg_RX_LANE_ID_OR_SKEW_REG_array_read( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_reg_RX_LANE_ID_OR_SKEW_REG_array_read( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32  N )
{
    UINT32 reg_value;

    reg_value = opsm23_reg_read( b_ptr,
                                 h_ptr,
                                 A,
                                 MEM_TYPE_STATUS,
                                 PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG(N));

    IOLOG( "%s -> 0x%08x; A=%d N=%d", "opsm23_reg_RX_LANE_ID_OR_SKEW_REG_array_read", reg_value, A, N);
    return reg_value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config
 * ==================================================================================
 */
static INLINE void opsm23_field_TX_DATA_4X10G_MODE_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_DATA_4X10G_MODE_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_DATA_4X10G_MODE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_DATA_4X10G_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_DATA_4X10G_MODE_set", A, value );

    /* (0x00011004 bits 20) field TX_DATA_4X10G_MODE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_4X10G_MODE_MSK,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_4X10G_MODE_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_TX_DATA_4X10G_MODE_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_DATA_4X10G_MODE_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_DATA_4X10G_MODE_get", A, 2);
    /* (0x00011004 bits 20) field TX_DATA_4X10G_MODE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_4X10G_MODE_MSK) >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_4X10G_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_DATA_4X10G_MODE_get", A, value );

    return value;
}
static INLINE void opsm23_field_TX_OTL_3_4_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_OTL_3_4_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_OTL_3_4_ENABLE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_OTL_3_4_ENABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_OTL_3_4_ENABLE_set", A, value );

    /* (0x00011004 bits 19) field TX_OTL_3_4_ENABLE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_OTL_3_4_ENABLE_MSK,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_OTL_3_4_ENABLE_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_TX_OTL_3_4_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_OTL_3_4_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_OTL_3_4_ENABLE_get", A, 2);
    /* (0x00011004 bits 19) field TX_OTL_3_4_ENABLE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_OTL_3_4_ENABLE_MSK) >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_OTL_3_4_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_OTL_3_4_ENABLE_get", A, value );

    return value;
}
static INLINE void opsm23_field_TX_SCRAMBLE_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SCRAMBLE_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SCRAMBLE_ENABLE_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SCRAMBLE_ENABLE_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SCRAMBLE_ENABLE_set", A, value );

    /* (0x00011004 bits 18:15) field TX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_MSK,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_TX_SCRAMBLE_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SCRAMBLE_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SCRAMBLE_ENABLE_get", A, 2);
    /* (0x00011004 bits 18:15) field TX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_MSK) >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SCRAMBLE_ENABLE_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_SCRAMBLE_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_SCRAMBLE_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SCRAMBLE_ENABLE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SCRAMBLE_ENABLE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SCRAMBLE_ENABLE_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SCRAMBLE_ENABLE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011004 bits 18:15) field TX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
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
        /* (0x00011004 bits 18:15) field TX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
        opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF + subfield_offset),
                                                 OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_SCRAMBLE_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SCRAMBLE_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SCRAMBLE_ENABLE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SCRAMBLE_ENABLE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SCRAMBLE_ENABLE_get", stop_bit, 3 );
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
    /* (0x00011004 bits 18:15) field TX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_MSK)
                  >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_MSK, OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SCRAMBLE_ENABLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SCRAMBLE_ENABLE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_HARDWARE_RESET_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_HARDWARE_RESET_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_HARDWARE_RESET_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_HARDWARE_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_HARDWARE_RESET_set", A, value );

    /* (0x00011004 bits 14) field TX_HARDWARE_RESET of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_MSK,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_TX_HARDWARE_RESET_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_HARDWARE_RESET_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_HARDWARE_RESET_get", A, 2);
    /* (0x00011004 bits 14) field TX_HARDWARE_RESET of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_MSK) >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_HARDWARE_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_HARDWARE_RESET_get", A, value );

    return value;
}
static INLINE void opsm23_field_TX_SYS_LOOPBACK_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SYS_LOOPBACK_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_LOOPBACK_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SYS_LOOPBACK_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SYS_LOOPBACK_set", A, value );

    /* (0x00011004 bits 13:10) field TX_SYS_LOOPBACK of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_MSK,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_TX_SYS_LOOPBACK_get( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SYS_LOOPBACK_get( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_LOOPBACK_get", A, 2);
    /* (0x00011004 bits 13:10) field TX_SYS_LOOPBACK of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_MSK) >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SYS_LOOPBACK_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_SYS_LOOPBACK_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_SYS_LOOPBACK_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_LOOPBACK_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_LOOPBACK_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_LOOPBACK_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_LOOPBACK_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011004 bits 13:10) field TX_SYS_LOOPBACK of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
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
        /* (0x00011004 bits 13:10) field TX_SYS_LOOPBACK of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
        opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_OFF + subfield_offset),
                                                 OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_SYS_LOOPBACK_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SYS_LOOPBACK_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_LOOPBACK_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_LOOPBACK_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_LOOPBACK_get", stop_bit, 3 );
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
    /* (0x00011004 bits 13:10) field TX_SYS_LOOPBACK of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_MSK)
                  >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_MSK, OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_SYS_LOOPBACK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_LOOPBACK_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_DATA_IN_SCRAMBLED_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_DATA_IN_SCRAMBLED_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_DATA_IN_SCRAMBLED_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_DATA_IN_SCRAMBLED_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_DATA_IN_SCRAMBLED_set", A, value );

    /* (0x00011004 bits 7:4) field TX_DATA_IN_SCRAMBLED of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_MSK,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_TX_DATA_IN_SCRAMBLED_get( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_DATA_IN_SCRAMBLED_get( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_DATA_IN_SCRAMBLED_get", A, 2);
    /* (0x00011004 bits 7:4) field TX_DATA_IN_SCRAMBLED of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_MSK) >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_DATA_IN_SCRAMBLED_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_DATA_IN_SCRAMBLED_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_DATA_IN_SCRAMBLED_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_DATA_IN_SCRAMBLED_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_DATA_IN_SCRAMBLED_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_DATA_IN_SCRAMBLED_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_DATA_IN_SCRAMBLED_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011004 bits 7:4) field TX_DATA_IN_SCRAMBLED of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
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
        /* (0x00011004 bits 7:4) field TX_DATA_IN_SCRAMBLED of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
        opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF + subfield_offset),
                                                 OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_DATA_IN_SCRAMBLED_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 start_bit,
                                                                  UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_DATA_IN_SCRAMBLED_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_DATA_IN_SCRAMBLED_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_DATA_IN_SCRAMBLED_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_DATA_IN_SCRAMBLED_get", stop_bit, 3 );
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
    /* (0x00011004 bits 7:4) field TX_DATA_IN_SCRAMBLED of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_MSK)
                  >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_MSK, OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_DATA_IN_SCRAMBLED_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_DATA_IN_SCRAMBLED_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_LINE_SOFT_RESET_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_LINE_SOFT_RESET_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LINE_SOFT_RESET_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_LINE_SOFT_RESET_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_LINE_SOFT_RESET_set", A, value );

    /* (0x00011004 bits 3:0) field TX_LINE_SOFT_RESET of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK,
                                             OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_TX_LINE_SOFT_RESET_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_LINE_SOFT_RESET_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LINE_SOFT_RESET_get", A, 2);
    /* (0x00011004 bits 3:0) field TX_LINE_SOFT_RESET of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK) >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_LINE_SOFT_RESET_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_LINE_SOFT_RESET_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_LINE_SOFT_RESET_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LINE_SOFT_RESET_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LINE_SOFT_RESET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LINE_SOFT_RESET_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LINE_SOFT_RESET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011004 bits 3:0) field TX_LINE_SOFT_RESET of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
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
        /* (0x00011004 bits 3:0) field TX_LINE_SOFT_RESET of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
        opsm23_reg_TX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF + subfield_offset),
                                                 OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_LINE_SOFT_RESET_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_LINE_SOFT_RESET_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LINE_SOFT_RESET_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LINE_SOFT_RESET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LINE_SOFT_RESET_get", stop_bit, 3 );
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
    /* (0x00011004 bits 3:0) field TX_LINE_SOFT_RESET of register PMC_OPSM23_REG_TX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_TX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK)
                  >> OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_MSK, OPSM23_REG_TX_GLOBAL_CTRL_REG_BIT_TX_LINE_SOFT_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LINE_SOFT_RESET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_PHYSICAL_LANE_3_SELECT_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_PHYSICAL_LANE_3_SELECT_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_PHYSICAL_LANE_3_SELECT_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_3_SELECT_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_3_SELECT_set", A, value );

    /* (0x0001101c bits 7:6) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK,
                                                  OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm23_field_TX_PHYSICAL_LANE_3_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_PHYSICAL_LANE_3_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_PHYSICAL_LANE_3_SELECT_get", A, 2);
    /* (0x0001101c bits 7:6) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    reg_value = opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK) >> OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_3_SELECT_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001101c bits 7:6) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
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
        /* (0x0001101c bits 7:6) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
        opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      subfield_mask << (OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF + subfield_offset),
                                                      OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_get", stop_bit, 1 );
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
    /* (0x0001101c bits 7:6) field TX_PHYSICAL_LANE_3_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    reg_value = opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK)
                  >> OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_MSK, OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_3_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_PHYSICAL_LANE_2_SELECT_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_PHYSICAL_LANE_2_SELECT_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_PHYSICAL_LANE_2_SELECT_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_2_SELECT_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_2_SELECT_set", A, value );

    /* (0x0001101c bits 5:4) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK,
                                                  OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm23_field_TX_PHYSICAL_LANE_2_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_PHYSICAL_LANE_2_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_PHYSICAL_LANE_2_SELECT_get", A, 2);
    /* (0x0001101c bits 5:4) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    reg_value = opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK) >> OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_2_SELECT_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001101c bits 5:4) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
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
        /* (0x0001101c bits 5:4) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
        opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      subfield_mask << (OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF + subfield_offset),
                                                      OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_get", stop_bit, 1 );
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
    /* (0x0001101c bits 5:4) field TX_PHYSICAL_LANE_2_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    reg_value = opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK)
                  >> OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_MSK, OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_2_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_PHYSICAL_LANE_1_SELECT_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_PHYSICAL_LANE_1_SELECT_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_PHYSICAL_LANE_1_SELECT_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_1_SELECT_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_1_SELECT_set", A, value );

    /* (0x0001101c bits 3:2) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK,
                                                  OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm23_field_TX_PHYSICAL_LANE_1_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_PHYSICAL_LANE_1_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_PHYSICAL_LANE_1_SELECT_get", A, 2);
    /* (0x0001101c bits 3:2) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    reg_value = opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK) >> OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_1_SELECT_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001101c bits 3:2) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
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
        /* (0x0001101c bits 3:2) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
        opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      subfield_mask << (OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF + subfield_offset),
                                                      OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_get", stop_bit, 1 );
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
    /* (0x0001101c bits 3:2) field TX_PHYSICAL_LANE_1_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    reg_value = opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK)
                  >> OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_MSK, OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_1_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_PHYSICAL_LANE_0_SELECT_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_PHYSICAL_LANE_0_SELECT_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_PHYSICAL_LANE_0_SELECT_set", A, 2);
    if (value > 3)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_0_SELECT_set", value, 3);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_0_SELECT_set", A, value );

    /* (0x0001101c bits 1:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK,
                                                  OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF,
                                                  value);
}

static INLINE UINT32 opsm23_field_TX_PHYSICAL_LANE_0_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_PHYSICAL_LANE_0_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_PHYSICAL_LANE_0_SELECT_get", A, 2);
    /* (0x0001101c bits 1:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    reg_value = opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    value = (reg_value & OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK) >> OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_PHYSICAL_LANE_0_SELECT_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 start_bit,
                                                                     UINT32 stop_bit,
                                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_set", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_set", stop_bit, 1 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001101c bits 1:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
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
        /* (0x0001101c bits 1:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
        opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      subfield_mask << (OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF + subfield_offset),
                                                      OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF + subfield_offset,
                                                      value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_get", stop_bit, 1 );
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
    /* (0x0001101c bits 1:0) field TX_PHYSICAL_LANE_0_SELECT of register PMC_OPSM23_REG_TX_L2P_LANE_MAPPING_REG */
    reg_value = opsm23_reg_TX_L2P_LANE_MAPPING_REG_read( b_ptr,
                                                         h_ptr,
                                                         A);
    field_value = (reg_value & OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK)
                  >> OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_MSK, OPSM23_REG_TX_L2P_LANE_MAPPING_REG_BIT_TX_PHYSICAL_LANE_0_SELECT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set", A, 2);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set", A, value );

    /* (0x00011020 bits 15:8) field TX_SYS_FIFO_XOFF_SET_FILL_LEVEL of register PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH */
    opsm23_reg_TX_SYNC_FIFO_XOFF_TH_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_MSK,
                                               OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_OFF,
                                               value);
}

static INLINE UINT32 opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get", A, 2);
    /* (0x00011020 bits 15:8) field TX_SYS_FIFO_XOFF_SET_FILL_LEVEL of register PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH */
    reg_value = opsm23_reg_TX_SYNC_FIFO_XOFF_TH_read( b_ptr,
                                                      h_ptr,
                                                      A);
    value = (reg_value & OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_MSK) >> OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set( opsm23_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set( opsm23_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 start_bit,
                                                                           UINT32 stop_bit,
                                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011020 bits 15:8) field TX_SYS_FIFO_XOFF_SET_FILL_LEVEL of register PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH */
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
        /* (0x00011020 bits 15:8) field TX_SYS_FIFO_XOFF_SET_FILL_LEVEL of register PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH */
        opsm23_reg_TX_SYNC_FIFO_XOFF_TH_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_OFF + subfield_offset),
                                                   OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_OFF + subfield_offset,
                                                   value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 start_bit,
                                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get", stop_bit, 7 );
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
    /* (0x00011020 bits 15:8) field TX_SYS_FIFO_XOFF_SET_FILL_LEVEL of register PMC_OPSM23_REG_TX_SYNC_FIFO_XOFF_TH */
    reg_value = opsm23_reg_TX_SYNC_FIFO_XOFF_TH_read( b_ptr,
                                                      h_ptr,
                                                      A);
    field_value = (reg_value & OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_MSK)
                  >> OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_MSK, OPSM23_REG_TX_SYNC_FIFO_XOFF_TH_BIT_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_FP_MFAS_INVERT_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_FP_MFAS_INVERT_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_FP_MFAS_INVERT_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_FP_MFAS_INVERT_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_FP_MFAS_INVERT_set", A, value );

    /* (0x0001102c bits 19:16) field TX_FP_MFAS_INVERT of register PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG */
    opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_field_set( b_ptr,
                                                        h_ptr,
                                                        A,
                                                        OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_MSK,
                                                        OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_OFF,
                                                        value);
}

static INLINE UINT32 opsm23_field_TX_FP_MFAS_INVERT_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_FP_MFAS_INVERT_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_FP_MFAS_INVERT_get", A, 2);
    /* (0x0001102c bits 19:16) field TX_FP_MFAS_INVERT of register PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG */
    reg_value = opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_read( b_ptr,
                                                               h_ptr,
                                                               A);
    value = (reg_value & OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_MSK) >> OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_FP_MFAS_INVERT_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_FP_MFAS_INVERT_set( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_FP_MFAS_INVERT_set( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_FP_MFAS_INVERT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_FP_MFAS_INVERT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_FP_MFAS_INVERT_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_FP_MFAS_INVERT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001102c bits 19:16) field TX_FP_MFAS_INVERT of register PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG */
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
        /* (0x0001102c bits 19:16) field TX_FP_MFAS_INVERT of register PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG */
        opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_field_set( b_ptr,
                                                            h_ptr,
                                                            A,
                                                            subfield_mask << (OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_OFF + subfield_offset),
                                                            OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_OFF + subfield_offset,
                                                            value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_FP_MFAS_INVERT_get( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_FP_MFAS_INVERT_get( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_FP_MFAS_INVERT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_FP_MFAS_INVERT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_FP_MFAS_INVERT_get", stop_bit, 3 );
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
    /* (0x0001102c bits 19:16) field TX_FP_MFAS_INVERT of register PMC_OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG */
    reg_value = opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_read( b_ptr,
                                                               h_ptr,
                                                               A);
    field_value = (reg_value & OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_MSK)
                  >> OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_MSK, OPSM23_REG_TX_FRAME_PULSE_GEN_CONFIG_REG_BIT_TX_FP_MFAS_INVERT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_FP_MFAS_INVERT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_RX_DATA_4X10G_MODE_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_DATA_4X10G_MODE_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DATA_4X10G_MODE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_DATA_4X10G_MODE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_DATA_4X10G_MODE_set", A, value );

    /* (0x00011080 bits 20) field RX_DATA_4X10G_MODE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_4X10G_MODE_MSK,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_4X10G_MODE_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_DATA_4X10G_MODE_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_DATA_4X10G_MODE_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DATA_4X10G_MODE_get", A, 2);
    /* (0x00011080 bits 20) field RX_DATA_4X10G_MODE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_4X10G_MODE_MSK) >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_4X10G_MODE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_DATA_4X10G_MODE_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_OTL_3_4_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_OTL_3_4_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTL_3_4_ENABLE_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_OTL_3_4_ENABLE_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_OTL_3_4_ENABLE_set", A, value );

    /* (0x00011080 bits 19) field RX_OTL_3_4_ENABLE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_3_4_ENABLE_MSK,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_3_4_ENABLE_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_OTL_3_4_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_OTL_3_4_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTL_3_4_ENABLE_get", A, 2);
    /* (0x00011080 bits 19) field RX_OTL_3_4_ENABLE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_3_4_ENABLE_MSK) >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_OTL_3_4_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_OTL_3_4_ENABLE_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_SCRAMBLE_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SCRAMBLE_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SCRAMBLE_ENABLE_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SCRAMBLE_ENABLE_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SCRAMBLE_ENABLE_set", A, value );

    /* (0x00011080 bits 18:15) field RX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_MSK,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SCRAMBLE_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SCRAMBLE_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SCRAMBLE_ENABLE_get", A, 2);
    /* (0x00011080 bits 18:15) field RX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_MSK) >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SCRAMBLE_ENABLE_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_RX_SCRAMBLE_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_RX_SCRAMBLE_ENABLE_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_SCRAMBLE_ENABLE_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_SCRAMBLE_ENABLE_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_SCRAMBLE_ENABLE_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_SCRAMBLE_ENABLE_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011080 bits 18:15) field RX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
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
        /* (0x00011080 bits 18:15) field RX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
        opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF + subfield_offset),
                                                 OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_RX_SCRAMBLE_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_RX_SCRAMBLE_ENABLE_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_SCRAMBLE_ENABLE_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_SCRAMBLE_ENABLE_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_SCRAMBLE_ENABLE_get", stop_bit, 3 );
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
    /* (0x00011080 bits 18:15) field RX_SCRAMBLE_ENABLE of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_MSK)
                  >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_MSK, OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SCRAMBLE_ENABLE_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_SCRAMBLE_ENABLE_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_RX_HARDWARE_RESET_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_HARDWARE_RESET_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_HARDWARE_RESET_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_HARDWARE_RESET_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_HARDWARE_RESET_set", A, value );

    /* (0x00011080 bits 14) field RX_HARDWARE_RESET of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_MSK,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_HARDWARE_RESET_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_HARDWARE_RESET_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_HARDWARE_RESET_get", A, 2);
    /* (0x00011080 bits 14) field RX_HARDWARE_RESET of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_MSK) >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_HARDWARE_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_HARDWARE_RESET_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_SYS_LOOPBACK_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SYS_LOOPBACK_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SYS_LOOPBACK_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SYS_LOOPBACK_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SYS_LOOPBACK_set", A, value );

    /* (0x00011080 bits 13:10) field RX_SYS_LOOPBACK of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_MSK,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SYS_LOOPBACK_get( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SYS_LOOPBACK_get( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SYS_LOOPBACK_get", A, 2);
    /* (0x00011080 bits 13:10) field RX_SYS_LOOPBACK of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_MSK) >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SYS_LOOPBACK_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_RX_SYS_LOOPBACK_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_RX_SYS_LOOPBACK_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 start_bit,
                                                           UINT32 stop_bit,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_SYS_LOOPBACK_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_SYS_LOOPBACK_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_SYS_LOOPBACK_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_SYS_LOOPBACK_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011080 bits 13:10) field RX_SYS_LOOPBACK of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
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
        /* (0x00011080 bits 13:10) field RX_SYS_LOOPBACK of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
        opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_OFF + subfield_offset),
                                                 OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_RX_SYS_LOOPBACK_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_RX_SYS_LOOPBACK_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_SYS_LOOPBACK_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_SYS_LOOPBACK_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_SYS_LOOPBACK_get", stop_bit, 3 );
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
    /* (0x00011080 bits 13:10) field RX_SYS_LOOPBACK of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_MSK)
                  >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_MSK, OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_SYS_LOOPBACK_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_SYS_LOOPBACK_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_set", A, value );

    /* (0x00011080 bits 9:6) field RX_DATA_IN_NOT_SCRAMBLED of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_MSK,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_get", A, 2);
    /* (0x00011080 bits 9:6) field RX_DATA_IN_NOT_SCRAMBLED of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_MSK) >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 start_bit,
                                                                    UINT32 stop_bit,
                                                                    UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011080 bits 9:6) field RX_DATA_IN_NOT_SCRAMBLED of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
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
        /* (0x00011080 bits 9:6) field RX_DATA_IN_NOT_SCRAMBLED of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
        opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF + subfield_offset),
                                                 OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_get( opsm23_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_get( opsm23_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_get", stop_bit, 3 );
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
    /* (0x00011080 bits 9:6) field RX_DATA_IN_NOT_SCRAMBLED of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_MSK)
                  >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_MSK, OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_DATA_IN_NOT_SCRAMBLED_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_RX_LINE_SOFT_RESET_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LINE_SOFT_RESET_set( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LINE_SOFT_RESET_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LINE_SOFT_RESET_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_LINE_SOFT_RESET_set", A, value );

    /* (0x00011080 bits 3:0) field RX_LINE_SOFT_RESET of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_MSK,
                                             OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_LINE_SOFT_RESET_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LINE_SOFT_RESET_get( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LINE_SOFT_RESET_get", A, 2);
    /* (0x00011080 bits 3:0) field RX_LINE_SOFT_RESET of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_MSK) >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_LINE_SOFT_RESET_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_RX_LINE_SOFT_RESET_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_RX_LINE_SOFT_RESET_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32 start_bit,
                                                              UINT32 stop_bit,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_LINE_SOFT_RESET_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_LINE_SOFT_RESET_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_LINE_SOFT_RESET_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_LINE_SOFT_RESET_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011080 bits 3:0) field RX_LINE_SOFT_RESET of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
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
        /* (0x00011080 bits 3:0) field RX_LINE_SOFT_RESET of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
        opsm23_reg_RX_GLOBAL_CTRL_REG_field_set( b_ptr,
                                                 h_ptr,
                                                 A,
                                                 subfield_mask << (OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_OFF + subfield_offset),
                                                 OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_OFF + subfield_offset,
                                                 value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_RX_LINE_SOFT_RESET_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 start_bit,
                                                                UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_RX_LINE_SOFT_RESET_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_LINE_SOFT_RESET_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_LINE_SOFT_RESET_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_LINE_SOFT_RESET_get", stop_bit, 3 );
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
    /* (0x00011080 bits 3:0) field RX_LINE_SOFT_RESET of register PMC_OPSM23_REG_RX_GLOBAL_CTRL_REG */
    reg_value = opsm23_reg_RX_GLOBAL_CTRL_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_MSK)
                  >> OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_MSK, OPSM23_REG_RX_GLOBAL_CTRL_REG_BIT_RX_LINE_SOFT_RESET_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_LINE_SOFT_RESET_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_set", A, 2);
    if (value > 255)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_set", value, 255);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_set", A, value );

    /* (0x00011084 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM23_REG_RX_OTU3_CTRL_REG */
    opsm23_reg_RX_OTU3_CTRL_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK,
                                           OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF,
                                           value);
}

static INLINE UINT32 opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_get", A, 2);
    /* (0x00011084 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM23_REG_RX_OTU3_CTRL_REG */
    reg_value = opsm23_reg_RX_OTU3_CTRL_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK) >> OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_set( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_set( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_set", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_set", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011084 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM23_REG_RX_OTU3_CTRL_REG */
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
        /* (0x00011084 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM23_REG_RX_OTU3_CTRL_REG */
        opsm23_reg_RX_OTU3_CTRL_REG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF + subfield_offset),
                                               OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_get", stop_bit, 7 );
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
    /* (0x00011084 bits 7:0) field RX_LOF_TRANSITION_OUT_COUNT of register PMC_OPSM23_REG_RX_OTU3_CTRL_REG */
    reg_value = opsm23_reg_RX_OTU3_CTRL_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK)
                  >> OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_MSK, OPSM23_REG_RX_OTU3_CTRL_REG_BIT_RX_LOF_TRANSITION_OUT_COUNT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_RX_FP_MFAS_GEN_INVERT_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FP_MFAS_GEN_INVERT_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FP_MFAS_GEN_INVERT_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FP_MFAS_GEN_INVERT_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_FP_MFAS_GEN_INVERT_set", A, value );

    /* (0x000110e4 bits 12:9) field RX_FP_MFAS_GEN_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
    opsm23_reg_RX_FP_CONFIG_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_MSK,
                                           OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_OFF,
                                           value);
}

static INLINE UINT32 opsm23_field_RX_FP_MFAS_GEN_INVERT_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FP_MFAS_GEN_INVERT_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FP_MFAS_GEN_INVERT_get", A, 2);
    /* (0x000110e4 bits 12:9) field RX_FP_MFAS_GEN_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
    reg_value = opsm23_reg_RX_FP_CONFIG_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_MSK) >> OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_FP_MFAS_GEN_INVERT_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 start_bit,
                                                                 UINT32 stop_bit,
                                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000110e4 bits 12:9) field RX_FP_MFAS_GEN_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
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
        /* (0x000110e4 bits 12:9) field RX_FP_MFAS_GEN_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
        opsm23_reg_RX_FP_CONFIG_REG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_OFF + subfield_offset),
                                               OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_RX_FP_MFAS_GEN_INVERT_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 start_bit,
                                                                   UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_RX_FP_MFAS_GEN_INVERT_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_FP_MFAS_GEN_INVERT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_FP_MFAS_GEN_INVERT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_FP_MFAS_GEN_INVERT_get", stop_bit, 3 );
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
    /* (0x000110e4 bits 12:9) field RX_FP_MFAS_GEN_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
    reg_value = opsm23_reg_RX_FP_CONFIG_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_MSK)
                  >> OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_MSK, OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_GEN_INVERT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_FP_MFAS_GEN_INVERT_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_RX_FP_MFAS_INVERT_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FP_MFAS_INVERT_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FP_MFAS_INVERT_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FP_MFAS_INVERT_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_FP_MFAS_INVERT_set", A, value );

    /* (0x000110e4 bits 8:5) field RX_FP_MFAS_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
    opsm23_reg_RX_FP_CONFIG_REG_field_set( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_MSK,
                                           OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_OFF,
                                           value);
}

static INLINE UINT32 opsm23_field_RX_FP_MFAS_INVERT_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FP_MFAS_INVERT_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FP_MFAS_INVERT_get", A, 2);
    /* (0x000110e4 bits 8:5) field RX_FP_MFAS_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
    reg_value = opsm23_reg_RX_FP_CONFIG_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_MSK) >> OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_FP_MFAS_INVERT_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_RX_FP_MFAS_INVERT_set( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_RX_FP_MFAS_INVERT_set( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32 start_bit,
                                                             UINT32 stop_bit,
                                                             UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_FP_MFAS_INVERT_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_FP_MFAS_INVERT_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_FP_MFAS_INVERT_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_FP_MFAS_INVERT_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x000110e4 bits 8:5) field RX_FP_MFAS_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
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
        /* (0x000110e4 bits 8:5) field RX_FP_MFAS_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
        opsm23_reg_RX_FP_CONFIG_REG_field_set( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_OFF + subfield_offset),
                                               OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_OFF + subfield_offset,
                                               value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_RX_FP_MFAS_INVERT_get( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_RX_FP_MFAS_INVERT_get( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_FP_MFAS_INVERT_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_FP_MFAS_INVERT_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_FP_MFAS_INVERT_get", stop_bit, 3 );
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
    /* (0x000110e4 bits 8:5) field RX_FP_MFAS_INVERT of register PMC_OPSM23_REG_RX_FP_CONFIG_REG */
    reg_value = opsm23_reg_RX_FP_CONFIG_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_MSK)
                  >> OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_MSK, OPSM23_REG_RX_FP_CONFIG_REG_BIT_RX_FP_MFAS_INVERT_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_FP_MFAS_INVERT_get", A, start_bit, stop_bit, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset config_N_size4
 * ==================================================================================
 */
static INLINE void opsm23_field_RX_SSF_LANE_MI_ACTIVE_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SSF_LANE_MI_ACTIVE_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_MI_ACTIVE_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_MI_ACTIVE_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SSF_LANE_MI_ACTIVE_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_MI_ACTIVE_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 15) field RX_SSF_LANE_MI_ACTIVE of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_SSF_LANE_MI_ACTIVE_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SSF_LANE_MI_ACTIVE_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_MI_ACTIVE_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_MI_ACTIVE_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 15) field RX_SSF_LANE_MI_ACTIVE of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_MI_ACTIVE_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_MI_ACTIVE_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_SSF_LANE_LOS_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SSF_LANE_LOS_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_LOS_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_LOS_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SSF_LANE_LOS_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_LOS_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 14) field RX_SSF_LANE_LOS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_SSF_LANE_LOS_get( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SSF_LANE_LOS_get( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_LOS_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_LOS_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 14) field RX_SSF_LANE_LOS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOS_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_LOS_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N,
                                                             UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 13) field RX_SSF_LANE_LOF_LOFLANE of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 13) field RX_SSF_LANE_LOF_LOFLANE of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_LOF_LOFLANE_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_SSF_LANE_OTUK_AIS_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SSF_LANE_OTUK_AIS_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_OTUK_AIS_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_OTUK_AIS_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SSF_LANE_OTUK_AIS_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_OTUK_AIS_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 11) field RX_SSF_LANE_OTUK_AIS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_SSF_LANE_OTUK_AIS_get( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SSF_LANE_OTUK_AIS_get( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_OTUK_AIS_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_OTUK_AIS_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 11) field RX_SSF_LANE_OTUK_AIS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTUK_AIS_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_OTUK_AIS_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_SSF_LANE_OTL_AIS_set( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SSF_LANE_OTL_AIS_set( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N,
                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_OTL_AIS_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_OTL_AIS_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SSF_LANE_OTL_AIS_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_OTL_AIS_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 10) field RX_SSF_LANE_OTL_AIS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_SSF_LANE_OTL_AIS_get( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SSF_LANE_OTL_AIS_get( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LANE_OTL_AIS_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LANE_OTL_AIS_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 10) field RX_SSF_LANE_OTL_AIS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LANE_OTL_AIS_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LANE_OTL_AIS_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_SSF_LOL_set( opsm23_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32  N,
                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SSF_LOL_set( opsm23_buffer_t *b_ptr,
                                                lineotn_handle_t *h_ptr,
                                                UINT32 A,
                                                UINT32  N,
                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LOL_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LOL_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SSF_LOL_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LOL_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 9) field RX_SSF_LOL of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_SSF_LOL_get( opsm23_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SSF_LOL_get( opsm23_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_LOL_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_LOL_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 9) field RX_SSF_LOL of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_LOL_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_LOL_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_SSF_OTU3_LOF_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SSF_OTU3_LOF_set( opsm23_buffer_t *b_ptr,
                                                     lineotn_handle_t *h_ptr,
                                                     UINT32 A,
                                                     UINT32  N,
                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_OTU3_LOF_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_OTU3_LOF_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SSF_OTU3_LOF_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_OTU3_LOF_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 8) field RX_SSF_OTU3_LOF of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU3_LOF_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU3_LOF_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_SSF_OTU3_LOF_get( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SSF_OTU3_LOF_get( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SSF_OTU3_LOF_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_SSF_OTU3_LOF_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 8) field RX_SSF_OTU3_LOF of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU3_LOF_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_SSF_OTU3_LOF_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_SSF_OTU3_LOF_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_FEC_LANE_MI_ACTIVE_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FEC_LANE_MI_ACTIVE_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FEC_LANE_MI_ACTIVE_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FEC_LANE_MI_ACTIVE_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FEC_LANE_MI_ACTIVE_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_FEC_LANE_MI_ACTIVE_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 7) field RX_FEC_LANE_MI_ACTIVE of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_FEC_LANE_MI_ACTIVE_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FEC_LANE_MI_ACTIVE_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FEC_LANE_MI_ACTIVE_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FEC_LANE_MI_ACTIVE_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 7) field RX_FEC_LANE_MI_ACTIVE of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FEC_LANE_MI_ACTIVE_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_FEC_LANE_MI_ACTIVE_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_FAIL_LANE_LOS_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FAIL_LANE_LOS_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LANE_LOS_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LANE_LOS_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FAIL_LANE_LOS_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LANE_LOS_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 6) field RX_FAIL_LANE_LOS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_FAIL_LANE_LOS_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FAIL_LANE_LOS_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LANE_LOS_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LANE_LOS_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 6) field RX_FAIL_LANE_LOS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOS_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LANE_LOS_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 5) field RX_FAIL_LANE_LOF_LOFLANE of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 5) field RX_FAIL_LANE_LOF_LOFLANE of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_LOF_LOFLANE_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_FAIL_LANE_OTUK_AIS_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FAIL_LANE_OTUK_AIS_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LANE_OTUK_AIS_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LANE_OTUK_AIS_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FAIL_LANE_OTUK_AIS_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LANE_OTUK_AIS_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 3) field RX_FAIL_LANE_OTUK_AIS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_FAIL_LANE_OTUK_AIS_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FAIL_LANE_OTUK_AIS_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LANE_OTUK_AIS_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LANE_OTUK_AIS_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 3) field RX_FAIL_LANE_OTUK_AIS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTUK_AIS_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LANE_OTUK_AIS_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_FAIL_LANE_OTL_AIS_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FAIL_LANE_OTL_AIS_set( opsm23_buffer_t *b_ptr,
                                                          lineotn_handle_t *h_ptr,
                                                          UINT32 A,
                                                          UINT32  N,
                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LANE_OTL_AIS_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LANE_OTL_AIS_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FAIL_LANE_OTL_AIS_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LANE_OTL_AIS_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 2) field RX_FAIL_LANE_OTL_AIS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_FAIL_LANE_OTL_AIS_get( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FAIL_LANE_OTL_AIS_get( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LANE_OTL_AIS_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LANE_OTL_AIS_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 2) field RX_FAIL_LANE_OTL_AIS of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LANE_OTL_AIS_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LANE_OTL_AIS_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_FAIL_LOL_set( opsm23_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FAIL_LOL_set( opsm23_buffer_t *b_ptr,
                                                 lineotn_handle_t *h_ptr,
                                                 UINT32 A,
                                                 UINT32  N,
                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LOL_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LOL_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FAIL_LOL_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LOL_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 1) field RX_FAIL_LOL of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_FAIL_LOL_get( opsm23_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FAIL_LOL_get( opsm23_buffer_t *b_ptr,
                                                   lineotn_handle_t *h_ptr,
                                                   UINT32 A,
                                                   UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_LOL_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_LOL_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 1) field RX_FAIL_LOL of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_LOL_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_LOL_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_FAIL_OTU3_LOF_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_FAIL_OTU3_LOF_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32  N,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_OTU3_LOF_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_OTU3_LOF_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_FAIL_OTU3_LOF_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_OTU3_LOF_set", A, N, value );

    /* ((0x00011094 + (N) * 0x4) bits 0) field RX_FAIL_OTU3_LOF of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set( b_ptr,
                                                             h_ptr,
                                                             A,
                                                             N,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU3_LOF_MSK,
                                                             OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU3_LOF_OFF,
                                                             value);
}

static INLINE UINT32 opsm23_field_RX_FAIL_OTU3_LOF_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_FAIL_OTU3_LOF_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A,
                                                        UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_FAIL_OTU3_LOF_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_FAIL_OTU3_LOF_get", N, 3);
    /* ((0x00011094 + (N) * 0x4) bits 0) field RX_FAIL_OTU3_LOF of register PMC_OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read( b_ptr,
                                                                    h_ptr,
                                                                    A,
                                                                    N);
    value = (reg_value & OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU3_LOF_MSK) >> OPSM23_REG_RX_LANE_SSF_FAIL_OR_CTRL_REG_BIT_RX_FAIL_OTU3_LOF_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_FAIL_OTU3_LOF_get", A, N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable
 * ==================================================================================
 */
static INLINE void opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_set", A, value );

    /* (0x00011010 bits 25) field TX_SFI_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_MSK,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_OFF,
                                        value);
}

static INLINE UINT32 opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_get", A, 2);
    /* (0x00011010 bits 25) field TX_SFI_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_MSK) >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set", A, value );

    /* (0x00011010 bits 24) field TX_SFI_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_MSK,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_OFF,
                                        value);
}

static INLINE UINT32 opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get", A, 2);
    /* (0x00011010 bits 24) field TX_SFI_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_MSK) >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_set", A, value );

    /* (0x00011010 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_MSK,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF,
                                        value);
}

static INLINE UINT32 opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_get", A, 2);
    /* (0x00011010 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_MSK) >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                      lineotn_handle_t *h_ptr,
                                                                      UINT32 A,
                                                                      UINT32 start_bit,
                                                                      UINT32 stop_bit,
                                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011010 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
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
        /* (0x00011010 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
        opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF + subfield_offset),
                                            OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_get", stop_bit, 3 );
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
    /* (0x00011010 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_MSK)
                  >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_MSK, OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set", A, value );

    /* (0x00011010 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_MSK,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF,
                                        value);
}

static INLINE UINT32 opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get", A, 2);
    /* (0x00011010 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_MSK) >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011010 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
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
        /* (0x00011010 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
        opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF + subfield_offset),
                                            OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_get", stop_bit, 3 );
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
    /* (0x00011010 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_MSK)
                  >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_MSK, OPSM23_REG_TX_INT_EN_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_set", A, value );

    /* (0x00011010 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF,
                                        value);
}

static INLINE UINT32 opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_get", A, 2);
    /* (0x00011010 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK) >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 start_bit,
                                                                       UINT32 stop_bit,
                                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011010 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
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
        /* (0x00011010 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
        opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF + subfield_offset),
                                            OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get", stop_bit, 3 );
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
    /* (0x00011010 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK)
                  >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_MSK, OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set", A, value );

    /* (0x00011010 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                        h_ptr,
                                        A,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK,
                                        OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF,
                                        value);
}

static INLINE UINT32 opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get", A, 2);
    /* (0x00011010 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK) >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit,
                                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011010 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
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
        /* (0x00011010 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
        opsm23_reg_TX_INT_EN_REG_field_set( b_ptr,
                                            h_ptr,
                                            A,
                                            subfield_mask << (OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF + subfield_offset),
                                            OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF + subfield_offset,
                                            value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get", stop_bit, 3 );
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
    /* (0x00011010 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_TX_INT_EN_REG */
    reg_value = opsm23_reg_TX_INT_EN_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK)
                  >> OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_MSK, OPSM23_REG_TX_INT_EN_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_E_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE void opsm23_field_RX_SFI_LOS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_LOS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOS_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_LOS_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_LOS_INT_E_set", A, value );

    /* (0x0001108c bits 15) field RX_SFI_LOS_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SFI_LOS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_LOS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOS_INT_E_get", A, 2);
    /* (0x0001108c bits 15) field RX_SFI_LOS_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOS_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_LOS_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_SFI_OOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_OOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_OOF_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_OOF_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_OOF_INT_E_set", A, value );

    /* (0x0001108c bits 14) field RX_SFI_OOF_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SFI_OOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_OOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_OOF_INT_E_get", A, 2);
    /* (0x0001108c bits 14) field RX_SFI_OOF_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_OOF_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_OOF_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_SFI_LOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_LOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOF_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_LOF_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_LOF_INT_E_set", A, value );

    /* (0x0001108c bits 13) field RX_SFI_LOF_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SFI_LOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_LOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOF_INT_E_get", A, 2);
    /* (0x0001108c bits 13) field RX_SFI_LOF_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_LOF_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_LOF_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_OTU_OOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_OTU_OOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_OOF_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_OTU_OOF_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_OTU_OOF_INT_E_set", A, value );

    /* (0x0001108c bits 12) field RX_OTU_OOF_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_OTU_OOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_OTU_OOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_OOF_INT_E_get", A, 2);
    /* (0x0001108c bits 12) field RX_OTU_OOF_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_OOF_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_OTU_OOF_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_OTU_LOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_OTU_LOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_LOF_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_OTU_LOF_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_OTU_LOF_INT_E_set", A, value );

    /* (0x0001108c bits 11) field RX_OTU_LOF_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_OTU_LOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_OTU_LOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_LOF_INT_E_get", A, 2);
    /* (0x0001108c bits 11) field RX_OTU_LOF_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_OTU_LOF_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_OTU_LOF_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_SFI_AIS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_AIS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                      lineotn_handle_t *h_ptr,
                                                      UINT32 A,
                                                      UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_AIS_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_AIS_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_AIS_INT_E_set", A, value );

    /* (0x0001108c bits 9) field RX_SFI_AIS_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SFI_AIS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_AIS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_AIS_INT_E_get", A, 2);
    /* (0x0001108c bits 9) field RX_SFI_AIS_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_AIS_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_AIS_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_LOL_INT_E_set( opsm23_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LOL_INT_E_set( opsm23_buffer_t *b_ptr,
                                                  lineotn_handle_t *h_ptr,
                                                  UINT32 A,
                                                  UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LOL_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LOL_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_LOL_INT_E_set", A, value );

    /* (0x0001108c bits 8) field RX_LOL_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_LOL_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_LOL_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_LOL_INT_E_get( opsm23_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LOL_INT_E_get( opsm23_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LOL_INT_E_get", A, 2);
    /* (0x0001108c bits 8) field RX_LOL_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_LOL_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_LOL_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_LOL_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_set", A, value );

    /* (0x0001108c bits 7) field RX_SKEW_RANGE_VIOLATION_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_get", A, 2);
    /* (0x0001108c bits 7) field RX_SKEW_RANGE_VIOLATION_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_ALIGN_CHANGE_INT_E_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_ALIGN_CHANGE_INT_E_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_ALIGN_CHANGE_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_E_set", A, value );

    /* (0x0001108c bits 6) field RX_ALIGN_CHANGE_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_ALIGN_CHANGE_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_ALIGN_CHANGE_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_ALIGN_CHANGE_INT_E_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_ALIGN_CHANGE_INT_E_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_ALIGN_CHANGE_INT_E_get", A, 2);
    /* (0x0001108c bits 6) field RX_ALIGN_CHANGE_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_ALIGN_CHANGE_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_ALIGN_CHANGE_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_DESKEW_CHANGE_INT_E_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_DESKEW_CHANGE_INT_E_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DESKEW_CHANGE_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_E_set", A, value );

    /* (0x0001108c bits 5) field RX_DESKEW_CHANGE_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_DESKEW_CHANGE_INT_E_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_DESKEW_CHANGE_INT_E_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DESKEW_CHANGE_INT_E_get", A, 2);
    /* (0x0001108c bits 5) field RX_DESKEW_CHANGE_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_DESKEW_CHANGE_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_set", A, value );

    /* (0x0001108c bits 4) field RX_SFI_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_get", A, 2);
    /* (0x0001108c bits 4) field RX_SFI_FIFO_OVERFLOW_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_get", A, value );

    return value;
}
static INLINE void opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set", A, value );

    /* (0x0001108c bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    opsm23_reg_RX_OTU3_INT_EN_REG_field_set( b_ptr,
                                             h_ptr,
                                             A,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_MSK,
                                             OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_OFF,
                                             value);
}

static INLINE UINT32 opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get", A, 2);
    /* (0x0001108c bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_E of register PMC_OPSM23_REG_RX_OTU3_INT_EN_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_EN_REG_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_MSK) >> OPSM23_REG_RX_OTU3_INT_EN_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_E_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get", A, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_enable_N_size4
 * ==================================================================================
 */
static INLINE void opsm23_field_RX_LANE_LOS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_LOS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_LOS_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOS_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_LOS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOS_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOS_INT_E_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_LANE_OOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_OOF_INT_E_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_OOF_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOF_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_OOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OOF_INT_E_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOF_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOF_INT_E_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_set( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32  N,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOF_LOFLANE_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_LANE_OTUK_AIS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_OTUK_AIS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                            lineotn_handle_t *h_ptr,
                                                            UINT32 A,
                                                            UINT32  N,
                                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_OTUK_AIS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OTUK_AIS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTUK_AIS_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_E_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_LANE_OTL_AIS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_OTL_AIS_INT_E_set( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32  N,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_OTL_AIS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OTL_AIS_INT_E_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OTL_AIS_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_E_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_set( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_LANE_FIFO_ERROR_INT_E_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_FIFO_ERROR_INT_E_set( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N,
                                                              UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_FIFO_ERROR_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_FIFO_ERROR_INT_E_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_FIFO_ERROR_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_E_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_LANE_OOR_INT_E_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_OOR_INT_E_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_OOR_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOR_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_OOR_INT_E_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OOR_INT_E_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_OOR_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOR_INT_E_get", A, N, value );

    return value;
}
static INLINE void opsm23_field_RX_LANE_LOR_INT_E_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_LOR_INT_E_set( opsm23_buffer_t *b_ptr,
                                                       lineotn_handle_t *h_ptr,
                                                       UINT32 A,
                                                       UINT32  N,
                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_E_set", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_E_set", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_LOR_INT_E_set", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOR_INT_E_set", A, N, value );

    /* ((0x000110c4 + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_EN_REG_array_field_set( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   N,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_MSK,
                                                   OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_OFF,
                                                   value);
}

static INLINE UINT32 opsm23_field_RX_LANE_LOR_INT_E_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOR_INT_E_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_E_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_E_get", N, 3);
    /* ((0x000110c4 + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_E of register PMC_OPSM23_REG_RX_LANE_INT_EN_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_EN_REG_array_read( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_MSK) >> OPSM23_REG_RX_LANE_INT_EN_REG_BIT_RX_LANE_LOR_INT_E_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOR_INT_E_get", A, N, value );

    return value;
}

/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_summary
 * ==================================================================================
 */
static INLINE UINT32 opsm23_field_INTERRUPT_SUMMARY_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_INTERRUPT_SUMMARY_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_INTERRUPT_SUMMARY_get", A, 2);
    /* (0x00011118 bits 7:0) field INTERRUPT_SUMMARY of register PMC_OPSM23_REG_OPSM23_INT_SUMMARY */
    reg_value = opsm23_reg_OPSM23_INT_SUMMARY_read( b_ptr,
                                                    h_ptr,
                                                    A);
    value = (reg_value & OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK) >> OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_INTERRUPT_SUMMARY_get", A, value );

    return value;
}
static INLINE UINT32 opsm23_field_range_INTERRUPT_SUMMARY_get( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 start_bit,
                                                               UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_INTERRUPT_SUMMARY_get( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_INTERRUPT_SUMMARY_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_INTERRUPT_SUMMARY_get", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_INTERRUPT_SUMMARY_get", stop_bit, 7 );
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
    /* (0x00011118 bits 7:0) field INTERRUPT_SUMMARY of register PMC_OPSM23_REG_OPSM23_INT_SUMMARY */
    reg_value = opsm23_reg_OPSM23_INT_SUMMARY_read( b_ptr,
                                                    h_ptr,
                                                    A);
    field_value = (reg_value & OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK)
                  >> OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK, OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_INTERRUPT_SUMMARY_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_INTERRUPT_SUMMARY_poll( opsm23_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_INTERRUPT_SUMMARY_poll( opsm23_buffer_t *b_ptr,
                                                                              lineotn_handle_t *h_ptr,
                                                                              UINT32 A,
                                                                              UINT32 start_bit,
                                                                              UINT32 stop_bit,
                                                                              UINT32 value,
                                                                              PMC_POLL_COMPARISON_TYPE cmp,
                                                                              UINT32 max_count,
                                                                              UINT32 *num_failed_polls,
                                                                              UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_INTERRUPT_SUMMARY_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_INTERRUPT_SUMMARY_poll", stop_bit, start_bit );
    if (stop_bit > 7) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_INTERRUPT_SUMMARY_poll", stop_bit, 7 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_INTERRUPT_SUMMARY_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011118 bits 7:0) field INTERRUPT_SUMMARY of register PMC_OPSM23_REG_OPSM23_INT_SUMMARY */
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
        if (stop_bit < 7) {
            subfield_stop_bit = stop_bit;
        } else {
            subfield_stop_bit = 7;
        }
        subfield_num_bits = subfield_stop_bit - subfield_start_bit + 1;
        subfield_mask = 0xffffffff >> (32-subfield_num_bits);
        /* (0x00011118 bits 7:0) field INTERRUPT_SUMMARY of register PMC_OPSM23_REG_OPSM23_INT_SUMMARY */
        return opsm23_reg_OPSM23_INT_SUMMARY_poll( b_ptr,
                                                   h_ptr,
                                                   A,
                                                   subfield_mask << (OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF + subfield_offset),
                                                   value << (OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF + subfield_offset),
                                                   cmp,
                                                   max_count,
                                                   num_failed_polls,
                                                   delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_INTERRUPT_SUMMARY_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_INTERRUPT_SUMMARY_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_INTERRUPT_SUMMARY_poll", A, value );

    /* (0x00011118 bits 7:0) field INTERRUPT_SUMMARY of register PMC_OPSM23_REG_OPSM23_INT_SUMMARY */
    return opsm23_reg_OPSM23_INT_SUMMARY_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_MSK,
                                               (value<<OPSM23_REG_OPSM23_INT_SUMMARY_BIT_INTERRUPT_SUMMARY_OFF),
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
static INLINE void opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", A, value );

    /* (0x0001100c bits 25) field TX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_MSK,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_OFF,
                                                     value);
}

static INLINE UINT32 opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_get", A, 2);
    /* (0x0001100c bits 25) field TX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_MSK) >> OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_poll", A, value );

    /* (0x0001100c bits 25) field TX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_MSK,
                                       (value<<OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_I_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", A, value );

    /* (0x0001100c bits 24) field TX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_MSK,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_OFF,
                                                     value);
}

static INLINE UINT32 opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get", A, 2);
    /* (0x0001100c bits 24) field TX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_MSK) >> OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_poll", A, value );

    /* (0x0001100c bits 24) field TX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_MSK,
                                       (value<<OPSM23_REG_TX_INT_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_I_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", A, value );

    /* (0x0001100c bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF,
                                                     value);
}

static INLINE UINT32 opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_get", A, 2);
    /* (0x0001100c bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK) >> OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32 start_bit,
                                                                               UINT32 stop_bit,
                                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001100c bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
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
        /* (0x0001100c bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
        opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                                         OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_get", stop_bit, 3 );
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
    /* (0x0001100c bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK)
                  >> OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK, OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                       lineotn_handle_t *h_ptr,
                                                                                       UINT32 A,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                       lineotn_handle_t *h_ptr,
                                                                                       UINT32 A,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001100c bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
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
        /* (0x0001100c bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
        return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                           value << (OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_poll", A, value );

    /* (0x0001100c bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_MSK,
                                       (value<<OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_I_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", A, value );

    /* (0x0001100c bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF,
                                                     value);
}

static INLINE UINT32 opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get", A, 2);
    /* (0x0001100c bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK) >> OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001100c bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
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
        /* (0x0001100c bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
        opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                                         OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_get", stop_bit, 3 );
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
    /* (0x0001100c bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK)
                  >> OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK, OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 A,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 A,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001100c bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
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
        /* (0x0001100c bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
        return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                           value << (OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_poll", A, value );

    /* (0x0001100c bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_MSK,
                                       (value<<OPSM23_REG_TX_INT_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_I_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", A, value );

    /* (0x0001100c bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF,
                                                     value);
}

static INLINE UINT32 opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_get", A, 2);
    /* (0x0001100c bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK) >> OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32 start_bit,
                                                                                UINT32 stop_bit,
                                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001100c bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
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
        /* (0x0001100c bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
        opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                                         OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get", stop_bit, 3 );
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
    /* (0x0001100c bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK)
                  >> OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK, OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 A,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 A,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001100c bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
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
        /* (0x0001100c bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
        return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                           value << (OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_poll", A, value );

    /* (0x0001100c bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_MSK,
                                       (value<<OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_I_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                           lineotn_handle_t *h_ptr,
                                                                           UINT32 A,
                                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", A, 2);
    if (value > 15)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", value, 15);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", A, value );

    /* (0x0001100c bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                     h_ptr,
                                                     A,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK,
                                                     OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF,
                                                     value);
}

static INLINE UINT32 opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get", A, 2);
    /* (0x0001100c bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK) >> OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get", A, value );

    return value;
}
static INLINE void opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 start_bit,
                                                                                 UINT32 stop_bit,
                                                                                 UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001100c bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
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
        /* (0x0001100c bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
        opsm23_reg_TX_INT_REG_action_on_write_field_set( b_ptr,
                                                         h_ptr,
                                                         A,
                                                         subfield_mask << (OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                                         OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset,
                                                         value >> subfield_shift);
    }
}

static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get", stop_bit, 3 );
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
    /* (0x0001100c bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    reg_value = opsm23_reg_TX_INT_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK)
                  >> OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK, OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                         lineotn_handle_t *h_ptr,
                                                                                         UINT32 A,
                                                                                         UINT32 start_bit,
                                                                                         UINT32 stop_bit,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                         lineotn_handle_t *h_ptr,
                                                                                         UINT32 A,
                                                                                         UINT32 start_bit,
                                                                                         UINT32 stop_bit,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x0001100c bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
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
        /* (0x0001100c bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
        return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           subfield_mask << (OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                           value << (OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF + subfield_offset),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_poll", A, value );

    /* (0x0001100c bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_TX_INT_REG */
    return opsm23_reg_TX_INT_REG_poll( b_ptr,
                                       h_ptr,
                                       A,
                                       OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_MSK,
                                       (value<<OPSM23_REG_TX_INT_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_I_OFF),
                                       cmp,
                                       max_count,
                                       num_failed_polls,
                                       delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_SFI_LOS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_LOS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOS_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_LOS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_LOS_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 15) field RX_SFI_LOS_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOS_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOS_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_SFI_LOS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_LOS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOS_INT_I_get", A, 2);
    /* (0x00011088 bits 15) field RX_SFI_LOS_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOS_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOS_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_LOS_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_LOS_INT_I_poll", A, value );

    /* (0x00011088 bits 15) field RX_SFI_LOS_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOS_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOS_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_SFI_OOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_OOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_OOF_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_OOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_OOF_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 14) field RX_SFI_OOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_OOF_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_OOF_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_SFI_OOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_OOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_OOF_INT_I_get", A, 2);
    /* (0x00011088 bits 14) field RX_SFI_OOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_OOF_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_OOF_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_OOF_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_OOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_OOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_OOF_INT_I_poll", A, value );

    /* (0x00011088 bits 14) field RX_SFI_OOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_OOF_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_OOF_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_SFI_LOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_LOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOF_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_LOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_LOF_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 13) field RX_SFI_LOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOF_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOF_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_SFI_LOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_LOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOF_INT_I_get", A, 2);
    /* (0x00011088 bits 13) field RX_SFI_LOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOF_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOF_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_LOF_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_LOF_INT_I_poll", A, value );

    /* (0x00011088 bits 13) field RX_SFI_LOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOF_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_LOF_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_OTU_OOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_OTU_OOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_OOF_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_OTU_OOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_OTU_OOF_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 12) field RX_OTU_OOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_OOF_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_OOF_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_OTU_OOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_OTU_OOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_OOF_INT_I_get", A, 2);
    /* (0x00011088 bits 12) field RX_OTU_OOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_OOF_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_OOF_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_OTU_OOF_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_OOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_OOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_OTU_OOF_INT_I_poll", A, value );

    /* (0x00011088 bits 12) field RX_OTU_OOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_OOF_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_OOF_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_OTU_LOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_OTU_LOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_LOF_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_OTU_LOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_OTU_LOF_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 11) field RX_OTU_LOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_LOF_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_LOF_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_OTU_LOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_OTU_LOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_LOF_INT_I_get", A, 2);
    /* (0x00011088 bits 11) field RX_OTU_LOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_LOF_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_LOF_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_OTU_LOF_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_LOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_LOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_OTU_LOF_INT_I_poll", A, value );

    /* (0x00011088 bits 11) field RX_OTU_LOF_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_LOF_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_OTU_LOF_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_SFI_AIS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_AIS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                               lineotn_handle_t *h_ptr,
                                                               UINT32 A,
                                                               UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_AIS_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_AIS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_AIS_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 9) field RX_SFI_AIS_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_AIS_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_AIS_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_SFI_AIS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_AIS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_AIS_INT_I_get", A, 2);
    /* (0x00011088 bits 9) field RX_SFI_AIS_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_AIS_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_AIS_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_AIS_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_AIS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_AIS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_AIS_INT_I_poll", A, value );

    /* (0x00011088 bits 9) field RX_SFI_AIS_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_AIS_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_AIS_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LOL_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LOL_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                           lineotn_handle_t *h_ptr,
                                                           UINT32 A,
                                                           UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LOL_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LOL_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_LOL_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 8) field RX_LOL_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_LOL_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_LOL_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_LOL_INT_I_get( opsm23_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LOL_INT_I_get( opsm23_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LOL_INT_I_get", A, 2);
    /* (0x00011088 bits 8) field RX_LOL_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_LOL_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_LOL_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_LOL_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LOL_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LOL_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_LOL_INT_I_poll", A, value );

    /* (0x00011088 bits 8) field RX_LOL_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_LOL_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_LOL_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_get", A, 2);
    /* (0x00011088 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                    lineotn_handle_t *h_ptr,
                                                                                    UINT32 A,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                    lineotn_handle_t *h_ptr,
                                                                                    UINT32 A,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_poll", A, value );

    /* (0x00011088 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_ALIGN_CHANGE_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_ALIGN_CHANGE_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_ALIGN_CHANGE_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 6) field RX_ALIGN_CHANGE_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_ALIGN_CHANGE_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_ALIGN_CHANGE_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_ALIGN_CHANGE_INT_I_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_ALIGN_CHANGE_INT_I_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_ALIGN_CHANGE_INT_I_get", A, 2);
    /* (0x00011088 bits 6) field RX_ALIGN_CHANGE_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_ALIGN_CHANGE_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_ALIGN_CHANGE_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_ALIGN_CHANGE_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_ALIGN_CHANGE_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_I_poll", A, value );

    /* (0x00011088 bits 6) field RX_ALIGN_CHANGE_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_ALIGN_CHANGE_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_ALIGN_CHANGE_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_DESKEW_CHANGE_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_DESKEW_CHANGE_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DESKEW_CHANGE_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 5) field RX_DESKEW_CHANGE_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_DESKEW_CHANGE_INT_I_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_DESKEW_CHANGE_INT_I_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DESKEW_CHANGE_INT_I_get", A, 2);
    /* (0x00011088 bits 5) field RX_DESKEW_CHANGE_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_DESKEW_CHANGE_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_DESKEW_CHANGE_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_I_poll", A, value );

    /* (0x00011088 bits 5) field RX_DESKEW_CHANGE_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_DESKEW_CHANGE_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_get", A, 2);
    /* (0x00011088 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_poll", A, value );

    /* (0x00011088 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", A, 2);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear", A, value );

    /* (0x00011088 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set( b_ptr,
                                                          h_ptr,
                                                          A,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_MSK,
                                                          OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_OFF,
                                                          value);
}

static INLINE UINT32 opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get", A, 2);
    /* (0x00011088 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_MSK) >> OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_poll", A, value );

    /* (0x00011088 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_I of register PMC_OPSM23_REG_RX_OTU3_INT_REG */
    return opsm23_reg_RX_OTU3_INT_REG_poll( b_ptr,
                                            h_ptr,
                                            A,
                                            OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_MSK,
                                            (value<<OPSM23_REG_RX_OTU3_INT_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_I_OFF),
                                            cmp,
                                            max_count,
                                            num_failed_polls,
                                            delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_sync_N_size4
 * ==================================================================================
 */
static INLINE void opsm23_field_RX_LANE_LOS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_LOS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_LOS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOS_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_LOS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOS_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOS_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOS_INT_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LANE_OOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_OOF_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_OOF_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOF_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_OOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OOF_INT_I_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOF_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOF_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOF_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOF_INT_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOF_LOFLANE_INT_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32  N,
                                                                     UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A,
                                                                     UINT32  N,
                                                                     UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_OTUK_AIS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OTUK_AIS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTUK_AIS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32  N,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTUK_AIS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32  N,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTUK_AIS_INT_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LANE_OTL_AIS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_OTL_AIS_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A,
                                                                    UINT32  N,
                                                                    UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_OTL_AIS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OTL_AIS_INT_I_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTL_AIS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTL_AIS_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OTL_AIS_INT_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N,
                                                                         UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32  N,
                                                                         UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32  N,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32  N,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32  N,
                                                                       UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32  N,
                                                                       UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_FIFO_ERROR_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_FIFO_ERROR_INT_I_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_FIFO_ERROR_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32  N,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_FIFO_ERROR_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32  N,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_FIFO_ERROR_INT_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LANE_OOR_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_OOR_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_OOR_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOR_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_OOR_INT_I_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OOR_INT_I_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOR_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOR_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOR_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOR_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_OOR_INT_I_OFF),
                                                  cmp,
                                                  max_count,
                                                  num_failed_polls,
                                                  delay_between_polls_in_microseconds);
}

static INLINE void opsm23_field_RX_LANE_LOR_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value ) ALWAYS_INLINE;
static INLINE void opsm23_field_RX_LANE_LOR_INT_I_set_to_clear( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N,
                                                                UINT32 value )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_I_set_to_clear", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_I_set_to_clear", N, 3);
    if (value > 1)
        IO_RANGE_CHECK("%s value is 0x%08x but max is 0x%08x", "opsm23_field_RX_LANE_LOR_INT_I_set_to_clear", value, 1);
    IOLOG( "%s <= A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOR_INT_I_set_to_clear", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set( b_ptr,
                                                                h_ptr,
                                                                A,
                                                                N,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_MSK,
                                                                OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_OFF,
                                                                value);
}

static INLINE UINT32 opsm23_field_RX_LANE_LOR_INT_I_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOR_INT_I_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_I_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_I_get", N, 3);
    /* ((0x000110b4 + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_REG_array_read( b_ptr,
                                                       h_ptr,
                                                       A,
                                                       N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_MSK) >> OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOR_INT_I_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOR_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOR_INT_I_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOR_INT_I_poll", A, N, value );

    /* ((0x000110b4 + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_I of register PMC_OPSM23_REG_RX_LANE_INT_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_REG_array_poll( b_ptr,
                                                  h_ptr,
                                                  A,
                                                  N,
                                                  OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_MSK,
                                                  (value<<OPSM23_REG_RX_LANE_INT_REG_BIT_RX_LANE_LOR_INT_I_OFF),
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
static INLINE UINT32 opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_get", A, 2);
    /* (0x00011014 bits 25) field TX_SFI_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_MSK) >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_poll", A, value );

    /* (0x00011014 bits 25) field TX_SFI_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_MSK,
                                           (value<<OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_OVERFLOW_INT_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get", A, 2);
    /* (0x00011014 bits 24) field TX_SFI_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_MSK) >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_poll", A, value );

    /* (0x00011014 bits 24) field TX_SFI_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_MSK,
                                           (value<<OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SFI_FIFO_UNDERFLOW_INT_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_get", A, 2);
    /* (0x00011014 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_MSK) >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_get", A, value );

    return value;
}
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_get", stop_bit, 3 );
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
    /* (0x00011014 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_MSK)
                  >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_MSK, OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                       lineotn_handle_t *h_ptr,
                                                                                       UINT32 A,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                       lineotn_handle_t *h_ptr,
                                                                                       UINT32 A,
                                                                                       UINT32 start_bit,
                                                                                       UINT32 stop_bit,
                                                                                       UINT32 value,
                                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                                       UINT32 max_count,
                                                                                       UINT32 *num_failed_polls,
                                                                                       UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011014 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
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
        /* (0x00011014 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
        return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF + subfield_offset),
                                               value << (OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF + subfield_offset),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_poll", A, value );

    /* (0x00011014 bits 23:20) field TX_SYS_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_MSK,
                                           (value<<OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_OVERFLOW_INT_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get", A, 2);
    /* (0x00011014 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_MSK) >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get", A, value );

    return value;
}
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_get", stop_bit, 3 );
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
    /* (0x00011014 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_MSK)
                  >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_MSK, OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 A,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 A,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011014 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
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
        /* (0x00011014 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
        return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF + subfield_offset),
                                               value << (OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF + subfield_offset),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_poll", A, value );

    /* (0x00011014 bits 19:16) field TX_SYS_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_MSK,
                                           (value<<OPSM23_REG_TX_INT_VAL_REG_BIT_TX_SYS_FIFO_UNDERFLOW_INT_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_get", A, 2);
    /* (0x00011014 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK) >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_get", A, value );

    return value;
}
static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
                                                                         UINT32 start_bit,
                                                                         UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                         lineotn_handle_t *h_ptr,
                                                                         UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get", stop_bit, 3 );
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
    /* (0x00011014 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK)
                  >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK, OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 A,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                        lineotn_handle_t *h_ptr,
                                                                                        UINT32 A,
                                                                                        UINT32 start_bit,
                                                                                        UINT32 stop_bit,
                                                                                        UINT32 value,
                                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                                        UINT32 max_count,
                                                                                        UINT32 *num_failed_polls,
                                                                                        UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011014 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
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
        /* (0x00011014 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
        return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF + subfield_offset),
                                               value << (OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF + subfield_offset),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_poll", A, value );

    /* (0x00011014 bits 7:4) field TX_LANE_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_MSK,
                                           (value<<OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_OVERFLOW_INT_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                    lineotn_handle_t *h_ptr,
                                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get", A, 2);
    /* (0x00011014 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK) >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get", A, value );

    return value;
}
static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
                                                                          UINT32 start_bit,
                                                                          UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                          lineotn_handle_t *h_ptr,
                                                                          UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get", stop_bit, 3 );
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
    /* (0x00011014 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    reg_value = opsm23_reg_TX_INT_VAL_REG_read(  b_ptr, h_ptr, A);
    field_value = (reg_value & OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK)
                  >> OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK, OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get", A, start_bit, stop_bit, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                         lineotn_handle_t *h_ptr,
                                                                                         UINT32 A,
                                                                                         UINT32 start_bit,
                                                                                         UINT32 stop_bit,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                         lineotn_handle_t *h_ptr,
                                                                                         UINT32 A,
                                                                                         UINT32 start_bit,
                                                                                         UINT32 stop_bit,
                                                                                         UINT32 value,
                                                                                         PMC_POLL_COMPARISON_TYPE cmp,
                                                                                         UINT32 max_count,
                                                                                         UINT32 *num_failed_polls,
                                                                                         UINT32 delay_between_polls_in_microseconds )
{
    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", A, 2);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", stop_bit, start_bit );
    if (stop_bit > 3) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", stop_bit, 3 );
    IOLOG( "%s <= A=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", A, start_bit, stop_bit, value );

    /* don't work hard trying to figure out if this can be changed from a read_modify_write into a pure write */
    if (b_ptr != NULL) {
        l1sys_force_read_modify_write( b_ptr->coalesce_handle);
    }

    /* (0x00011014 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
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
        /* (0x00011014 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
        return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                               h_ptr,
                                               A,
                                               subfield_mask << (OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF + subfield_offset),
                                               value << (OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF + subfield_offset),
                                               cmp,
                                               max_count,
                                               num_failed_polls,
                                               delay_between_polls_in_microseconds);
    }
    return PMC_ERR_INVALID_PARAMETERS;
}

static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                   lineotn_handle_t *h_ptr,
                                                                                   UINT32 A,
                                                                                   UINT32 value,
                                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                                   UINT32 max_count,
                                                                                   UINT32 *num_failed_polls,
                                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_poll", A, value );

    /* (0x00011014 bits 3:0) field TX_LANE_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_TX_INT_VAL_REG */
    return opsm23_reg_TX_INT_VAL_REG_poll( b_ptr,
                                           h_ptr,
                                           A,
                                           OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_MSK,
                                           (value<<OPSM23_REG_TX_INT_VAL_REG_BIT_TX_LANE_FIFO_UNDERFLOW_INT_V_OFF),
                                           cmp,
                                           max_count,
                                           num_failed_polls,
                                           delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_SFI_LOS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_LOS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOS_INT_V_get", A, 2);
    /* (0x00011090 bits 15) field RX_SFI_LOS_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_LOS_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_LOS_INT_V_poll", A, value );

    /* (0x00011090 bits 15) field RX_SFI_LOS_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOS_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_SFI_OOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_OOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_OOF_INT_V_get", A, 2);
    /* (0x00011090 bits 14) field RX_SFI_OOF_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_OOF_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_OOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_OOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_OOF_INT_V_poll", A, value );

    /* (0x00011090 bits 14) field RX_SFI_OOF_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_OOF_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_SFI_LOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_LOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_LOF_INT_V_get", A, 2);
    /* (0x00011090 bits 13) field RX_SFI_LOF_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_LOF_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_LOF_INT_V_poll", A, value );

    /* (0x00011090 bits 13) field RX_SFI_LOF_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_LOF_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_OTU_OOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_OTU_OOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_OOF_INT_V_get", A, 2);
    /* (0x00011090 bits 12) field RX_OTU_OOF_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_OTU_OOF_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_OOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_OOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_OTU_OOF_INT_V_poll", A, value );

    /* (0x00011090 bits 12) field RX_OTU_OOF_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_OOF_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_OTU_LOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_OTU_LOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_OTU_LOF_INT_V_get", A, 2);
    /* (0x00011090 bits 11) field RX_OTU_LOF_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_OTU_LOF_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_LOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_LOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_OTU_LOF_INT_V_poll", A, value );

    /* (0x00011090 bits 11) field RX_OTU_LOF_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_OTU_LOF_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_SFI_AIS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_AIS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                        lineotn_handle_t *h_ptr,
                                                        UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_AIS_INT_V_get", A, 2);
    /* (0x00011090 bits 9) field RX_SFI_AIS_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_AIS_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_AIS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_AIS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                       lineotn_handle_t *h_ptr,
                                                                       UINT32 A,
                                                                       UINT32 value,
                                                                       PMC_POLL_COMPARISON_TYPE cmp,
                                                                       UINT32 max_count,
                                                                       UINT32 *num_failed_polls,
                                                                       UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_AIS_INT_V_poll", A, value );

    /* (0x00011090 bits 9) field RX_SFI_AIS_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_AIS_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LOL_INT_V_get( opsm23_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LOL_INT_V_get( opsm23_buffer_t *b_ptr,
                                                    lineotn_handle_t *h_ptr,
                                                    UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LOL_INT_V_get", A, 2);
    /* (0x00011090 bits 8) field RX_LOL_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_LOL_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_LOL_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_LOL_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LOL_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LOL_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A,
                                                                   UINT32 value,
                                                                   PMC_POLL_COMPARISON_TYPE cmp,
                                                                   UINT32 max_count,
                                                                   UINT32 *num_failed_polls,
                                                                   UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_LOL_INT_V_poll", A, value );

    /* (0x00011090 bits 8) field RX_LOL_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_LOL_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_LOL_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                     lineotn_handle_t *h_ptr,
                                                                     UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_get", A, 2);
    /* (0x00011090 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                    lineotn_handle_t *h_ptr,
                                                                                    UINT32 A,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                    lineotn_handle_t *h_ptr,
                                                                                    UINT32 A,
                                                                                    UINT32 value,
                                                                                    PMC_POLL_COMPARISON_TYPE cmp,
                                                                                    UINT32 max_count,
                                                                                    UINT32 *num_failed_polls,
                                                                                    UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_poll", A, value );

    /* (0x00011090 bits 7) field RX_SKEW_RANGE_VIOLATION_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SKEW_RANGE_VIOLATION_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_ALIGN_CHANGE_INT_V_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_ALIGN_CHANGE_INT_V_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_ALIGN_CHANGE_INT_V_get", A, 2);
    /* (0x00011090 bits 6) field RX_ALIGN_CHANGE_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_ALIGN_CHANGE_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_ALIGN_CHANGE_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_ALIGN_CHANGE_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_ALIGN_CHANGE_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_ALIGN_CHANGE_INT_V_poll", A, value );

    /* (0x00011090 bits 6) field RX_ALIGN_CHANGE_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_ALIGN_CHANGE_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_ALIGN_CHANGE_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_DESKEW_CHANGE_INT_V_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_DESKEW_CHANGE_INT_V_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_DESKEW_CHANGE_INT_V_get", A, 2);
    /* (0x00011090 bits 5) field RX_DESKEW_CHANGE_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_DESKEW_CHANGE_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_DESKEW_CHANGE_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_DESKEW_CHANGE_INT_V_poll", A, value );

    /* (0x00011090 bits 5) field RX_DESKEW_CHANGE_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_DESKEW_CHANGE_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_get", A, 2);
    /* (0x00011090 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_poll", A, value );

    /* (0x00011090 bits 4) field RX_SFI_FIFO_OVERFLOW_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_OVERFLOW_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                   lineotn_handle_t *h_ptr,
                                                                   UINT32 A )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get", A, 2);
    /* (0x00011090 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    reg_value = opsm23_reg_RX_OTU3_INT_VAL_REG_read( b_ptr,
                                                     h_ptr,
                                                     A);
    value = (reg_value & OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_MSK) >> OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_OFF;
    IOLOG( "%s -> A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get", A, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                  lineotn_handle_t *h_ptr,
                                                                                  UINT32 A,
                                                                                  UINT32 value,
                                                                                  PMC_POLL_COMPARISON_TYPE cmp,
                                                                                  UINT32 max_count,
                                                                                  UINT32 *num_failed_polls,
                                                                                  UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d 0x%08x", "opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_poll", A, value );

    /* (0x00011090 bits 3) field RX_SFI_FIFO_UNDERFLOW_INT_V of register PMC_OPSM23_REG_RX_OTU3_INT_VAL_REG */
    return opsm23_reg_RX_OTU3_INT_VAL_REG_poll( b_ptr,
                                                h_ptr,
                                                A,
                                                OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_MSK,
                                                (value<<OPSM23_REG_RX_OTU3_INT_VAL_REG_BIT_RX_SFI_FIFO_UNDERFLOW_INT_V_OFF),
                                                cmp,
                                                max_count,
                                                num_failed_polls,
                                                delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset int_value_N_size4
 * ==================================================================================
 */
static INLINE UINT32 opsm23_field_RX_LANE_LOS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOS_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOS_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOS_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 15) field RX_LANE_LOS_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOS_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LANE_OOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OOF_INT_V_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOF_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOF_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOF_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOF_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 14) field RX_LANE_OOF_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOF_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                 lineotn_handle_t *h_ptr,
                                                                 UINT32 A,
                                                                 UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                lineotn_handle_t *h_ptr,
                                                                                UINT32 A,
                                                                                UINT32  N,
                                                                                UINT32 value,
                                                                                PMC_POLL_COMPARISON_TYPE cmp,
                                                                                UINT32 max_count,
                                                                                UINT32 *num_failed_polls,
                                                                                UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 13) field RX_LANE_LOF_LOFLANE_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOF_LOFLANE_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LANE_OTUK_AIS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OTUK_AIS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                              lineotn_handle_t *h_ptr,
                                                              UINT32 A,
                                                              UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTUK_AIS_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTUK_AIS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32  N,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTUK_AIS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                             lineotn_handle_t *h_ptr,
                                                                             UINT32 A,
                                                                             UINT32  N,
                                                                             UINT32 value,
                                                                             PMC_POLL_COMPARISON_TYPE cmp,
                                                                             UINT32 max_count,
                                                                             UINT32 *num_failed_polls,
                                                                             UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTUK_AIS_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 10) field RX_LANE_OTUK_AIS_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTUK_AIS_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LANE_OTL_AIS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OTL_AIS_INT_V_get( opsm23_buffer_t *b_ptr,
                                                             lineotn_handle_t *h_ptr,
                                                             UINT32 A,
                                                             UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OTL_AIS_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTL_AIS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTL_AIS_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                            lineotn_handle_t *h_ptr,
                                                                            UINT32 A,
                                                                            UINT32  N,
                                                                            UINT32 value,
                                                                            PMC_POLL_COMPARISON_TYPE cmp,
                                                                            UINT32 max_count,
                                                                            UINT32 *num_failed_polls,
                                                                            UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OTL_AIS_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 9) field RX_LANE_OTL_AIS_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OTL_AIS_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32  N,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                                 lineotn_handle_t *h_ptr,
                                                                                 UINT32 A,
                                                                                 UINT32  N,
                                                                                 UINT32 value,
                                                                                 PMC_POLL_COMPARISON_TYPE cmp,
                                                                                 UINT32 max_count,
                                                                                 UINT32 *num_failed_polls,
                                                                                 UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 7) field RX_LANE_ALIGN_CHANGE_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_ALIGN_CHANGE_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LANE_FIFO_ERROR_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_FIFO_ERROR_INT_V_get( opsm23_buffer_t *b_ptr,
                                                                lineotn_handle_t *h_ptr,
                                                                UINT32 A,
                                                                UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_FIFO_ERROR_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_FIFO_ERROR_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32  N,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_FIFO_ERROR_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                               lineotn_handle_t *h_ptr,
                                                                               UINT32 A,
                                                                               UINT32  N,
                                                                               UINT32 value,
                                                                               PMC_POLL_COMPARISON_TYPE cmp,
                                                                               UINT32 max_count,
                                                                               UINT32 *num_failed_polls,
                                                                               UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_FIFO_ERROR_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 5) field RX_LANE_FIFO_ERROR_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_FIFO_ERROR_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LANE_OOR_INT_V_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_OOR_INT_V_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_OOR_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOR_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOR_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOR_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_OOR_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 2) field RX_LANE_OOR_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_OOR_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}

static INLINE UINT32 opsm23_field_RX_LANE_LOR_INT_V_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LANE_LOR_INT_V_get( opsm23_buffer_t *b_ptr,
                                                         lineotn_handle_t *h_ptr,
                                                         UINT32 A,
                                                         UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_V_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LANE_LOR_INT_V_get", N, 3);
    /* ((0x000110d4 + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_INT_VAL_REG_array_read( b_ptr,
                                                           h_ptr,
                                                           A,
                                                           N);
    value = (reg_value & OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_MSK) >> OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOR_INT_V_get", A, N, value );

    return value;
}
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOR_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
static INLINE PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOR_INT_V_poll( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 value,
                                                                        PMC_POLL_COMPARISON_TYPE cmp,
                                                                        UINT32 max_count,
                                                                        UINT32 *num_failed_polls,
                                                                        UINT32 delay_between_polls_in_microseconds )
{
    IOLOG( "%s wait until value =  A=%d N=%d 0x%08x", "opsm23_field_RX_LANE_LOR_INT_V_poll", A, N, value );

    /* ((0x000110d4 + (N) * 0x4) bits 1) field RX_LANE_LOR_INT_V of register PMC_OPSM23_REG_RX_LANE_INT_VAL_REG index N=0..3 */
    return opsm23_reg_RX_LANE_INT_VAL_REG_array_poll( b_ptr,
                                                      h_ptr,
                                                      A,
                                                      N,
                                                      OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_MSK,
                                                      (value<<OPSM23_REG_RX_LANE_INT_VAL_REG_BIT_RX_LANE_LOR_INT_V_OFF),
                                                      cmp,
                                                      max_count,
                                                      num_failed_polls,
                                                      delay_between_polls_in_microseconds);
}


/*
 * ==================================================================================
 * Parameter Access Functions for Paramset status_N_size4
 * ==================================================================================
 */
static INLINE UINT32 opsm23_field_RX_LOGICAL_LANE_IDENTIFIER_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_RX_LOGICAL_LANE_IDENTIFIER_get( opsm23_buffer_t *b_ptr,
                                                                  lineotn_handle_t *h_ptr,
                                                                  UINT32 A,
                                                                  UINT32  N )
{
    UINT32 value = 0;
    UINT32 reg_value;

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_RX_LOGICAL_LANE_IDENTIFIER_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_RX_LOGICAL_LANE_IDENTIFIER_get", N, 3);
    /* ((0x000110a4 + (N) * 0x4) bits 1:0) field RX_LOGICAL_LANE_IDENTIFIER of register PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_ID_OR_SKEW_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              N);
    value = (reg_value & OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_MSK) >> OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_OFF;
    IOLOG( "%s -> A=%d N=%d 0x%08x", "opsm23_field_RX_LOGICAL_LANE_IDENTIFIER_get", A, N, value );

    return value;
}
static INLINE UINT32 opsm23_field_range_RX_LOGICAL_LANE_IDENTIFIER_get( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
                                                                        UINT32  N,
                                                                        UINT32 start_bit,
                                                                        UINT32 stop_bit ) ATTRIBUTE_WARN_UNUSED_RESULT ALWAYS_INLINE;
static INLINE UINT32 opsm23_field_range_RX_LOGICAL_LANE_IDENTIFIER_get( opsm23_buffer_t *b_ptr,
                                                                        lineotn_handle_t *h_ptr,
                                                                        UINT32 A,
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

    if (A > 2)
        IO_RANGE_CHECK("%s A is %d but max is %d", "opsm23_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", A, 2);
    if (N > 3)
        IO_RANGE_CHECK("%s N is %d but max is %d", "opsm23_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", N, 3);
    if (start_bit > stop_bit) IO_RANGE_CHECK("%s error: stop_bit:%d > start_bit:%d", "opsm23_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", stop_bit, start_bit );
    if (stop_bit > 1) IO_RANGE_CHECK("%s error: stop_bit:%d > %d", "opsm23_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", stop_bit, 1 );
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
    /* ((0x000110a4 + (N) * 0x4) bits 1:0) field RX_LOGICAL_LANE_IDENTIFIER of register PMC_OPSM23_REG_RX_LANE_ID_OR_SKEW_REG index N=0..3 */
    reg_value = opsm23_reg_RX_LANE_ID_OR_SKEW_REG_array_read( b_ptr,
                                                              h_ptr,
                                                              A,
                                                              N);
    field_value = (reg_value & OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_MSK)
                  >> OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_OFF;
    IOLOG(  "field_value = (%08x & 0x%x) >> %d; // (%08x)", reg_value, OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_MSK, OPSM23_REG_RX_LANE_ID_OR_SKEW_REG_BIT_RX_LOGICAL_LANE_IDENTIFIER_OFF, field_value );
    value |= ((field_value >> subfield_offset) & subfield_mask) << subfield_shift;
    IOLOG(  "value |= ((%08x >> %d) & 0x%x << %d); // (%08x)", field_value, subfield_offset, subfield_mask, subfield_shift, ((field_value >> subfield_offset) & subfield_mask) << subfield_shift );
    IOLOG( "%s -> A=%d N=%d start_bit=%d stop_bit=%d 0x%08x", "opsm23_field_range_RX_LOGICAL_LANE_IDENTIFIER_get", A, N, start_bit, stop_bit, value );

    return value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _OPSM23_IO_INLINE_H */
